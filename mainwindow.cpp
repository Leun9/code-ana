#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QFont>
#include <thread>
#include "kernel/calc_similarity.h"
#include "kernel/lexical_analyzer.h"
#include "kernel/cfg_dfs.h"
#include "kernel/func_scanner.h"
#include "kernel/vuln_scan.h"

#define S2QS(x) (QString::fromStdString(x))
#define NUM2QS(...) (QString::number(__VA_ARGS__))

using std::vector;
using std::string;
using std::pair;
using std::sort;
using std::thread;
using std::count;

using namespace codeana::kernel;

static vector<QString> unsign2str({"", "unsigned "});
static vector<QString> type2str({"unknown", "void", "_Bool", "char", "wchar_t", "short", "int", "long","float", "long long", "double"});
static vector<QString> functype2qstr(VULN_FUNC_LIST);
static vector<QString> errlevel2qstr({"UNKNOWN", "LOW", "MIDDLE", "HIGH"});
static vector<QString> errtype2qstr({"缓冲区溢出", "栈缓冲区溢出", "堆缓冲区溢出", "格式化字符串", "宽度溢出", "符号溢出"});

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    // Copyright
    statusBar()->addPermanentWidget(new QLabel("Copyright © 2020 u201814846.    ", this));

    // Init Lex
    LexicalAnalyzer::Init();

    // Init LineEdit
    ui->leLexSrcPath->setReadOnly(true); // TODO : input path in lineEdit

    // Connect Signals and Slots
    QObject::connect(this, SIGNAL(__HomUpdateInfo(const QString&)), this, SLOT(HomUpdateInfo(const QString&)), Qt::BlockingQueuedConnection);
    QObject::connect(this, SIGNAL(__HomUpdateInfo2(const QString&)), this, SLOT(HomUpdateInfo2(const QString&)), Qt::BlockingQueuedConnection);
    QObject::connect(this, SIGNAL(__HomUpdateTe(const QString&)), this, SLOT(HomUpdateTe(const QString&)), Qt::BlockingQueuedConnection);

    // DEBUG
    ui->stackedWidget->setCurrentIndex(4);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HomologyDetectionThread(QString hom_dst_path, int mode) {
    QFile hom_dst_file(hom_dst_path);
    if (!hom_dst_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO : Warn
        return ;
    }

    string dst_str = hom_dst_file.readAll().toStdString();
    hom_dst_file.close();
    string src_str = hom_src_.toStdString(); // FIXME

    QString info_buf = hom_dst_path + ":\n";
    QString info2_buf = hom_dst_path + ":\n";

    /*** 根据token特征计算相似度 ***/
    {
        vector<string> src_tokens, dst_tokens;
        LexicalAnalyzer::GetStringTokens(src_tokens, src_str);
        LexicalAnalyzer::GetStringTokens(dst_tokens, dst_str);

        vector<size_t> src_pos;
        vector<size_t> dst_pos;
        vector<size_t> len;
        //qDebug() << src_tokens.size() << dst_tokens.size();
        CalcTokensSimlarity(src_tokens, dst_tokens, kHOM_TOKENMINSIZE, src_pos, dst_pos, len);

        size_t len_sum = 0;
        auto size = dst_pos.size();
        if (dst_pos.size()) {
            info_buf.append("  相似代码块: \n");
            for (size_t i = 0; i < size; ++i) {
                info_buf.append("    目标文件行: " + NUM2QS(dst_pos[i] + 1) +
                                ", 源文件行：" + NUM2QS(src_pos[i] + 1) +
                                ", 行数： " + NUM2QS(len[i]) + "\n");
                len_sum += len[i];
            }
        }
        double rate = 0;
        //qDebug() << len_sum << dst_tokens.size();
        if (dst_tokens.size() | src_tokens.size()) {
            rate = 100 * ((double)(len_sum << 1) / (src_tokens.size() + dst_tokens.size()));
            if (rate > 100) rate = 100;
        }
        //qDebug() << rate;
        info_buf.append("  相似度（Token）: " + NUM2QS(rate, 'f', 1) + "%\n\n");
        //qDebug() << hom_dst_path;
    }


    /*** 根据cfg计算相似度 ***/
    {
        unordered_map<string, string> src_func2tokens, dst_func2tokens;
        unordered_map<string, pair<size_t, size_t>> src_func_pos, dst_func_pos;
        unordered_map<string, vector<string>> src_func2subfunc, dst_func2subfunc;
        LexicalAnalyzer::GetStringFuncTokens(src_func2tokens, src_func_pos, src_func2subfunc, src_str);
        LexicalAnalyzer::GetStringFuncTokens(dst_func2tokens, dst_func_pos, dst_func2subfunc, dst_str);
        vector<string> src_dfs, dst_dfs;
        if (DfsCfg(src_dfs, src_func2subfunc)) return ; // FIXME : no main
        if (DfsCfg(dst_dfs, dst_func2subfunc)) return ; // FIXME : no main

        src_str.clear();
        dst_str.clear();
        for (auto &i : src_dfs) src_str.append(src_func2tokens[i]);
        for (auto &i : dst_dfs) dst_str.append(dst_func2tokens[i]);
        vector<size_t> src_pos;
        vector<size_t> dst_pos;
        vector<size_t> len;

        CalcStringSimlarity(src_str, dst_str, kHOM_CFGMINSIZE, src_pos, dst_pos, len);
        size_t len_sum = 0;
        for (auto i : len) len_sum += i;
        double rate = 0;
        if (dst_str.size() | src_str.size()) {
            rate = 100 * ((double) (len_sum << 1) / (src_str.size() + dst_str.size()));
        }
        info2_buf.append("  相似度（CFG）: " + NUM2QS(rate, 'f', 1) + "%\n\n");
    }

    emit __HomUpdateInfo(info_buf);
    emit __HomUpdateInfo2(info2_buf);

    if (mode == 1) emit __HomUpdateTe(QString("[分析完成] " + hom_dst_path));
}

void MainWindow::HomologyDetection() {
    if (hom_dst_path_list_.isEmpty()) return ;
    if (hom_src_.isEmpty()) return ;
    ui->teHomInfo->clear();
    ui->teHomInfo2->clear();
    if (hom_dst_path_list_.size() == 1) {
        std::thread thr(&MainWindow::HomologyDetectionThread, this, hom_dst_path_list_[0], 0);
        thr.detach();
        return ;
    }
    ui->teHomDst->clear();
    hom_timer_.start();
    hom_timer_cnt_ = hom_dst_path_list_.size() << 1;
    for (auto &path : hom_dst_path_list_) {
        emit HomUpdateTe(QString("[开始分析]" + path));
        std::thread thr(&MainWindow::HomologyDetectionThread, this, path, 1);
        //qDebug() << path;
        thr.detach();
    }
}

void MainWindow::HomUpdateInfo(const QString &qstr) {
    ui->teHomInfo->append(qstr);
}

void MainWindow::HomUpdateInfo2(const QString &qstr) {
    ui->teHomInfo2->append(qstr);
}

void MainWindow::HomUpdateTe(const QString &qstr) {
    ui->teHomDst->append(qstr);
    --hom_timer_cnt_;
    if (hom_timer_cnt_ == 0) {
        ui->teHomDst->append("\n分析完毕，耗时" + NUM2QS(hom_timer_.elapsed()) + "ms。");
    }
}

void MainWindow::on_btnLexSrcPath_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this); // 定义文件对话框类
    fileDialog->setWindowTitle(QStringLiteral("选中文件")); // 定义文件对话框标题
    fileDialog->setDirectory(".");  // 设置默认文件路径
    fileDialog->setNameFilter(tr("File(*.*)")); // 设置文件过滤器
    //fileDialog->setFileMode(QFileDialog::ExistingFiles); // 设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setViewMode(QFileDialog::Detail); // 设置视图模式
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    } else {
        return ;
    }

    QString lex_src_path(fileNames[0]);
    ui->leLexSrcPath->setText(lex_src_path);
    QFile lex_src_file(lex_src_path);
    if (!lex_src_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO : Warn & Update StatusBar
        return;
    }
    QString src_qstr(lex_src_file.readAll());
    ui->teLexSrc->clear();
    ui->teLexSrcToken->clear();
    // setText
    ui->teLexSrc->setText(src_qstr);
    // GetTokens & setText
    string src_str = src_qstr.toStdString();
    vector<string> tokens;
    string tokens_decode;
    LexicalAnalyzer::GetStringTokens(tokens, src_str);
    LexicalAnalyzer::DecodeTokens(tokens_decode, tokens);
    ui->teLexSrcToken->setText(S2QS(tokens_decode));
    return ;
}

void MainWindow::on_btnCfgSrcPath_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this); // 定义文件对话框类
    fileDialog->setWindowTitle(QStringLiteral("选中文件")); // 定义文件对话框标题
    fileDialog->setDirectory(".");  // 设置默认文件路径
    fileDialog->setNameFilter(tr("File(*.*)")); // 设置文件过滤器
    //fileDialog->setFileMode(QFileDialog::ExistingFiles); // 设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setViewMode(QFileDialog::Detail); // 设置视图模式
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    } else {
        return ;
    }

    QString cfg_src_path(fileNames[0]);
    ui->leCfgSrcPath->setText(cfg_src_path);
    QFile cfg_src_file(cfg_src_path);
    if (!cfg_src_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO : Warn & Update StatusBar
        return;
    }
    QString src_qstr(cfg_src_file.readAll());
    ui->teCfgSrc->clear();
    ui->teCfgSrcInfo->clear();
    // setText
    ui->teCfgSrc->setText(src_qstr);
    // GetTokens & setText
    string src_str = src_qstr.toStdString();
    unordered_map<string, string> func2tokens;
    unordered_map<string, pair<size_t, size_t>> func_pos;
    unordered_map<string, vector<string>> func2subfunc;
    LexicalAnalyzer::GetStringFuncTokens(func2tokens, func_pos, func2subfunc, src_str);
    if (func2subfunc.count("main")) {
        auto subfunc = func2subfunc["main"];
        size_t start = func_pos["main"].first;
        size_t end = func_pos["main"].second;
        ui->teCfgSrcInfo->append("\n");
        ui->teCfgSrcInfo->append("[MAIN]:\tmain");
        ui->teCfgSrcInfo->append("起始偏移:\t" + NUM2QS(start));
        ui->teCfgSrcInfo->append("末尾偏移:\t" + NUM2QS(end));
        if (subfunc.empty()) {
            ui->teCfgSrcInfo->append("无调用函数");
        } else {
            ui->teCfgSrcInfo->append("调用函数（顺序排列）:");
            for (auto &j : subfunc) {
                ui->teCfgSrcInfo->append("\t    " + S2QS(j));
            }
        }
    }
    for (auto &i : func2subfunc) {
        auto func_name = i.first;
        if (func_name == "main") break;
        auto subfunc = i.second;
        size_t start = func_pos[func_name].first;
        size_t end = func_pos[func_name].second;
        ui->teCfgSrcInfo->append("\n");
        ui->teCfgSrcInfo->append("[FUNC]:\t" + S2QS(func_name));
        ui->teCfgSrcInfo->append("起始偏移:\t" + NUM2QS(start));
        ui->teCfgSrcInfo->append("末尾偏移:\t" + NUM2QS(end));
        if (subfunc.empty()) {
            ui->teCfgSrcInfo->append("无调用函数");
        } else {
            ui->teCfgSrcInfo->append("调用函数（顺序排列）:");
            for (auto &j : subfunc) {
                ui->teCfgSrcInfo->append("\t    " + S2QS(j));
            }
        }
    }
    return ;
}

void MainWindow::on_actionHomDet_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionCfgExt_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionLexAna_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnHomSrc_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this); // 定义文件对话框类
    fileDialog->setWindowTitle(QStringLiteral("选中文件")); // 定义文件对话框标题
    fileDialog->setDirectory(".");  // 设置默认文件路径
    fileDialog->setNameFilter(tr("File(*.*)")); // 设置文件过滤器
    //fileDialog->setFileMode(QFileDialog::ExistingFiles); // 设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setViewMode(QFileDialog::Detail); // 设置视图模式
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    } else {
        return ;
    }

    QString hom_src_path(fileNames[0]);
    ui->leHomSrcPath->setText(hom_src_path);
    QFile hom_src_file(hom_src_path);
    if (!hom_src_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO : Warn & Update StatusBar
        return;
    }
    hom_src_ = hom_src_file.readAll();
    ui->teHomSrc->setText(hom_src_);
    hom_src_file.close();
    HomologyDetection();
    return ;
}

void MainWindow::on_btnHomDst_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this); // 定义文件对话框类
    fileDialog->setWindowTitle(QStringLiteral("选中文件")); // 定义文件对话框标题
    fileDialog->setDirectory(".");  // 设置默认文件路径
    fileDialog->setNameFilter(tr("File(*.*)")); // 设置文件过滤器
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail); // 设置视图模式
    if (fileDialog->exec()) {
        hom_dst_path_list_ = fileDialog->selectedFiles();
    } else {
        return ;
    }
    ui->leHomDstPath->setText(hom_dst_path_list_.join(";"));
    if (hom_dst_path_list_.size() == 1) {
        QFile hom_dst_file(hom_dst_path_list_[0]);
        if (!hom_dst_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // TODO : Warn & Update StatusBar
            return;
        }
        QString hom_dst = hom_dst_file.readAll();
        ui->teHomDst->setText(hom_dst);
        hom_dst_file.close();
    } else {
        ui->teHomDst->setText(hom_dst_path_list_.join("\n"));
    }
    HomologyDetection();
    return ;
}

void MainWindow::on_actionFuncScan_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_btnFunPath_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this); // 定义文件对话框类
    fileDialog->setWindowTitle(QStringLiteral("选中文件")); // 定义文件对话框标题
    fileDialog->setDirectory(".");  // 设置默认文件路径
    fileDialog->setNameFilter(tr("File(*.*)")); // 设置文件过滤器
    //fileDialog->setFileMode(QFileDialog::ExistingFiles); // 设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setViewMode(QFileDialog::Detail); // 设置视图模式
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    } else {
        return ;
    }
    QString func_src_path(fileNames[0]);
    ui->leFuncPath->setText(func_src_path);
    QFile func_src_file(func_src_path);
    if (!func_src_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO : Warn & Update StatusBar
        return;
    }
    QString qstr = func_src_file.readAll();
    ui->teFuncSrc->setText(qstr);
    auto str = qstr.toStdString();
    func_src_file.close();

    FuncInfos func_infos;
    ValueInfos global_values;
    GetFuncInfos(func_infos, global_values, str);

    ui->teFuncRes->clear();
    if (!global_values.empty()) {
        ui->teFuncRes->append("[全局变量]：\n");
        for (auto &v : global_values) {
            ui->teFuncRes->append("  [变量]\t" + S2QS(v.name_) );
            QString type(unsign2str[v.unsigned_] + type2str[v.type_] + " ");
            if (v.is_pointer_) type.append("指针");
            if (v.is_array_) type.append("数组");
            ui->teFuncRes->append("  类型：\t" + type);
            ui->teFuncRes->append("  宽度：\t" + NUM2QS(v.width_));
            if (v.is_array_ || v.is_pointer_) ui->teFuncRes->append("  单元宽度：\t" + NUM2QS(v.size_));
            if (v.is_array_) ui->teFuncRes->append("  数组可用长度：  \t" + NUM2QS(v.len_));
            ui->teFuncRes->append("  作用域起始偏移：\t" + NUM2QS(v.start_));
            ui->teFuncRes->append("  作用域末尾偏移：\t" + NUM2QS(v.end_));
            ui->teFuncRes->append("  代码块深度：\t" + NUM2QS(v.deep_) + "\n");
        }
    }

    for (auto func : func_infos) {
        ui->teFuncRes->append("\n[Func]\t" + S2QS(func.name_));
        ui->teFuncRes->append("  起始偏移：\t" + NUM2QS(func.start_));
        ui->teFuncRes->append("  末尾偏移：\t" + NUM2QS(func.end_));
        if (!func.value_infos_.empty()) {
            ui->teFuncRes->append("  局部变量：\n");
            for (auto &v : func.value_infos_) {
                if (v.deep_ == 0) continue;
                ui->teFuncRes->append("    [变量] \t" + S2QS(v.name_) );
                QString type(unsign2str[v.unsigned_] + type2str[v.type_] + " ");
                if (v.is_pointer_) type.append("指针");
                if (v.is_array_) type.append("数组");
                ui->teFuncRes->append("    类型：\t" + type);
                ui->teFuncRes->append("    宽度：\t" + NUM2QS(v.width_));
                if (v.is_array_ || v.is_pointer_) ui->teFuncRes->append("    单元宽度：\t" + NUM2QS(v.size_));
                if (v.is_array_) ui->teFuncRes->append("    数组可用长度：  \t" + NUM2QS(v.len_));
                ui->teFuncRes->append("    作用域起始偏移：\t" + NUM2QS(v.start_));
                ui->teFuncRes->append("    作用域末尾偏移：\t" + NUM2QS(v.end_));
                ui->teFuncRes->append("    代码块深度：\t" + NUM2QS(v.deep_) + "\n");
            }
        }
        ui->teFuncRes->append("\n");
    }


    return ;
}

void MainWindow::on_actionVulnDet_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_btnVulnPath_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this); // 定义文件对话框类
    fileDialog->setWindowTitle(QStringLiteral("选中文件")); // 定义文件对话框标题
    fileDialog->setDirectory(".");  // 设置默认文件路径
    fileDialog->setNameFilter(tr("File(*.*)")); // 设置文件过滤器
    //fileDialog->setFileMode(QFileDialog::ExistingFiles); // 设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setViewMode(QFileDialog::Detail); // 设置视图模式
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    } else {
        return ;
    }
    QString vuln_src_path(fileNames[0]);
    ui->leVulnPath->setText(vuln_src_path);
    QFile vuln_src_file(vuln_src_path);
    if (!vuln_src_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO : Warn & Update StatusBar
        return;
    }
    QString qstr = vuln_src_file.readAll();
    ui->teVulnSrc->setText(qstr);
    auto str = qstr.toStdString();
    string::iterator str_it = str.begin();
    vuln_src_file.close();

    ValueInfos global_values;
    GetFuncInfos(func_infos_, global_values, str);

    ui->teVulnRes->clear();
    for (auto &func_info : func_infos) {
        //qDebug() << str.substr(func_info.start_, func_info.end_-func_info.start_+1).c_str();

        vector<int> pos;
        vector<string> info;
        vector<int> errlevel;
        vector<int> func_type;
        vector<int> errtype;
        vector<int> line;
        BufVulnScan(pos, func_type, info, errlevel, errtype,
                    str, func_info.start_, func_info.end_, func_info.value_infos_);

        size_t line_i = 0;
        size_t line_cnt = 1;
        for (size_t i = 0; i < pos.size(); ++i) {
            //qDebug() << func_type[i] << vuln_func[func_type[i]].c_str() << vuln_func.size();
            size_t now_i = pos[i];
            line_cnt += count(str_it + line_i, str_it + now_i, '\n');
            line_i = now_i;
            line.push_back(line_cnt);
        }
        line_list_.push_back(line);
        pos_list_.push_back(pos);
        info_list_.push_back(info);
        errlevel_list_.push_back(errlevel);
        func_type_list_.push_back(func_type);
        errlevel_list_.push_back(errlevel);
    }
    RefreshVulnScan();
}

void MainWindow::RefreshVulnScan() {
    auto pit = pos_list_.begin();
    auto iit = info_list_.begin();
    auto eit = errlevel_list_.begin();
    auto fit = func_type_list_.begin();
    auto rit = errtype_list_.begin();
    auto lit = line_list_.begin();
    for (size_t i = 0; i < pos_list_.size(); ++i) {
        auto pos = *pit; ++pit;
        auto info = *iit; ++iit;
        auto errlevel = *eit; ++eit;
        auto func_type = *fit; ++fit;
        auto errtype = *rit; ++rit;
        auto line = *lit; ++lit;
        for (size_t i = 0; i < pos.size(); ++i) {
            //qDebug() << func_type[i] << vuln_func[func_type[i]].c_str() << vuln_func.size();
            size_t now_i = pos[i];
            QString temp = "偏移：" + NUM2QS(now_i) + "，行号：" + NUM2QS(line[i]);
            if (func_type[i]) temp += "，函数：" + functype2qstr[func_type[i]];
            temp += "，危险等级：" + errlevel2qstr[errlevel[i]];
            if (errlevel[i] > LOW) temp += "， 漏洞类型：" + errtype2qstr[errtype[i]];
            if  (!info[i].empty()) temp += "， 信息：" + S2QS(info[i]);
            ui->teVulnRes->append(temp);
        }
    }
}

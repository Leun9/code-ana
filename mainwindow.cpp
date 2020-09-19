#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <string>
#include <algorithm>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include "kernel/calc_similarity.h"
#include "kernel/lexical_analyzer.h"

#define HOM_MINSIZE 6


using std::vector;
using std::string;
using std::sort;

using namespace codeana::kernel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    // Init Lex
    LexicalAnalyzer::Init();

    // Init LineEdit
    ui->leLexSrcPath->setReadOnly(true); // TODO : input path in lineEdit

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HomologyDetection() {
    if (hom_dst_.isEmpty()) return;
    if (hom_src_.isEmpty()) return;
    // GetTokens & setText
    string src_str = hom_src_.toStdString();
    string dst_str = hom_dst_.toStdString();

// 根据token特征计算相似度
    vector<string> src_tokens, dst_tokens;
    LexicalAnalyzer::GetStringTokens(src_tokens, src_str);
    LexicalAnalyzer::GetStringTokens(dst_tokens, dst_str);

    vector<size_t> src_pos;
    vector<size_t> dst_pos;
    vector<size_t> len;
    size_t len_sum;
    size_t len_tot;
    //qDebug() << src_tokens.size() << dst_tokens.size();
    CalcTokensSimlarity(src_tokens, dst_tokens, HOM_MINSIZE, src_pos, dst_pos, len, len_sum, len_tot);
    //qDebug() << len_sum << len_tot;

    // 调用CalcTokensSimlarity得到长度大于HOM_MINSIZE的相似代码块后，后处理结果，使相似代码块不重叠。
    len_sum = 0;
    ui->teHomInfo->clear();
    if (dst_pos.size() != 0) ui->teHomInfo->append("相似块: ");
    vector<pair<size_t, size_t>> temp;
    for (size_t i = 0; i < dst_pos.size(); ++i) temp.push_back(pair<size_t, size_t>(dst_pos[i], i));
    sort(temp.begin(), temp.end());
    size_t last_end = 0;
    for (auto &tempi : temp) {
        //qDebug() << tempi.first << tempi.second;
        size_t i = tempi.second;
        size_t end = dst_pos[i] + len[i];
        if (last_end > dst_pos[i]) {
            if (last_end >= end) continue;
            src_pos[i] += last_end - dst_pos[i];
            dst_pos[i] = last_end; // dst_pos[i] += last_end - dst_pos[i];
            len[i] = end - dst_pos[i];
        }
        last_end = end;
        len_sum += len[i];
        ui->teHomInfo->append("\t目标文件行: " + QString::number(dst_pos[i] + 1)
                              + ", 源文件行：" + QString::number(src_pos[i] + 1)
                              + ", 长度： " + QString::number(len[i]));
    }
    double rate = 0;
    //qDebug() << len_sum << dst_tokens.size();
    if (dst_tokens.size() != 0) {
        rate = 100 * ((double)len_sum / dst_tokens.size());
    }
    //qDebug() << rate;
    ui->teHomInfo->append("\n相似度: " + QString::number(rate, 'f', 1) + "%");

// 根据cfg计算相似度
    // TODO
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
    ui->teLexSrcToken->setText(QString::fromStdString(tokens_decode));
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
        ui->teCfgSrcInfo->append("起始偏移:\t" + QString::number(start));
        ui->teCfgSrcInfo->append("末尾偏移:\t" + QString::number(end));
        if (subfunc.empty()) {
            ui->teCfgSrcInfo->append("无调用函数");
        } else {
            ui->teCfgSrcInfo->append("调用函数（顺序排列）:");
            for (auto &j : subfunc) {
                ui->teCfgSrcInfo->append("\t    " + QString::fromStdString(j));
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
        ui->teCfgSrcInfo->append("[FUNC]:\t" + QString::fromStdString(func_name));
        ui->teCfgSrcInfo->append("起始偏移:\t" + QString::number(start));
        ui->teCfgSrcInfo->append("末尾偏移:\t" + QString::number(end));
        if (subfunc.empty()) {
            ui->teCfgSrcInfo->append("无调用函数");
        } else {
            ui->teCfgSrcInfo->append("调用函数（顺序排列）:");
            for (auto &j : subfunc) {
                ui->teCfgSrcInfo->append("\t    " + QString::fromStdString(j));
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
    HomologyDetection();
    return ;
}


void MainWindow::on_btnHomDst_clicked()
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

    QString hom_dst_path(fileNames[0]);
    ui->leHomDstPath->setText(hom_dst_path);
    QFile hom_dst_file(hom_dst_path);
    if (!hom_dst_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO : Warn & Update StatusBar
        return;
    }
    hom_dst_ = hom_dst_file.readAll();
    ui->teHomDst->setText(hom_dst_);
    HomologyDetection();
    return ;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <string>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include "kernel/lexical_analyzer.h"


using std::vector;
using std::string;
using codeana::kernel::LexicalAnalyzer;

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

    // Init TextEdit
    ui->teLexSrc->setReadOnly(true);
    ui->teLexSrcToken->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    }

    this->lex_src_path_ = fileNames[0];
    ui->leLexSrcPath->setText(this->lex_src_path_);
    QFile lex_src_file(this->lex_src_path_);
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <vector>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QDebug>

using std::vector;
using std::string;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HomologyDetection(){


    FILE* scr_fp = fopen(this->homology_src_path_.toStdString().c_str(), "r");
    // TODO : check fp

    FILE* dst_fp = fopen(this->homology_dst_path_.toStdString().c_str(), "r");
    // TODO : check fp

    vector<string> src;
    vector<string> dst;
    vector<size_t> src_pos;
    vector<size_t> dst_pos;
    vector<size_t> len;
    size_t len_sum;
    size_t len_total;

}


void MainWindow::on_btnSrcPath_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("File(*.*)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    }

    // TODO : size > 1
    if (fileNames.size() == 1) {
       //qDebug() << fileNames[0];
       this->homology_src_path_ = fileNames[0];

       QFile file(this->homology_src_path_);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           // TODO : Update StatusBar
       }
       ui->leSrcPath->setText(this->homology_src_path_);
       ui->leSrc->clear();

       QTextStream in(&file);
       QString line = in.readLine();
       while (!line.isNull()) {
           ui->leSrc->append(line);
           line = in.readLine();
       }

       if (!this->homology_src_path_.isEmpty()) this->HomologyDetection();
    }
}

void MainWindow::on_btnDstPath_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("File(*.*)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    }

     // TODO : size > 1
    if (fileNames.size() == 1) {
        //qDebug() << fileNames[0];
        this->homology_dst_path_ = fileNames[0];

        QFile file(this->homology_dst_path_);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // TODO : Update StatusBar
        }
        ui->leDstPath->setText(this->homology_dst_path_);
        ui->leDst->clear();

        QTextStream in(&file);
        QString line = in.readLine();
        while (!line.isNull()) {
            ui->leDst->append(line);
            line = in.readLine();
        }

        if (!this->homology_src_path_.isEmpty()) this->HomologyDetection();
    }

}

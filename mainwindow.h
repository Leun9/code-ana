#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSrcPath_clicked();
    void on_btnDstPath_clicked();

private:
    void HomologyDetection();

    Ui::MainWindow *ui;

    // Homology Analyzer
    QString homology_src_path_;
    QString homology_dst_path_;
};
#endif // MAINWINDOW_H

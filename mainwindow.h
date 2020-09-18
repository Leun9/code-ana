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
    void on_btnLexSrcPath_clicked();
    void on_btnCfgSrcPath_clicked();
    void on_actionHomDet_triggered();
    void on_actionCfgExt_triggered();
    void on_actionLexAna_triggered();

    void on_btnHomSrc_clicked();

    void on_btnHomDst_clicked();

private:
    void HomologyDetection();

private:
    Ui::MainWindow *ui;
    QString hom_src_;
    QString hom_dst_;
};
#endif // MAINWINDOW_H

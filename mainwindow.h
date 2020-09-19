#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const size_t kHOM_MINSIZE = 6;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void __HomUpdateInfo(QString qstr);
    void __HomUpdateInfo2(QString qstr);
    void __HomUpdateTe(QString qstr);

public slots:
    void HomUpdateInfo(QString qstr);
    void HomUpdateInfo2(QString qstr);
    void HomUpdateTe(QString qstr);

private slots:
    void on_btnLexSrcPath_clicked();
    void on_btnCfgSrcPath_clicked();
    void on_actionHomDet_triggered();
    void on_actionCfgExt_triggered();
    void on_actionLexAna_triggered();
    void on_btnHomSrc_clicked();
    void on_btnHomDst_clicked();

private:
    void HomologyDetectionThread(QString hom_dst_path, int mode);
    void HomologyDetection();

private:
    Ui::MainWindow *ui;
    QString hom_src_;
    QStringList hom_dst_path_list_;

};



#endif // MAINWINDOW_H

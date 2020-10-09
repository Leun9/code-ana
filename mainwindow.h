#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using std::list;
using std::string;
using std::vector;

const size_t kHOM_TOKENMINSIZE = 6;
const size_t kHOM_CFGMINSIZE = 64;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void __HomUpdateInfo(const QString &qstr);
    void __HomUpdateInfo2(const QString &qstr);
    void __HomUpdateTe(const QString &qstr);

public slots:
    void HomUpdateInfo(const QString &qstr);
    void HomUpdateInfo2(const QString &qstr);
    void HomUpdateTe(const QString &qstr);

private slots:
    void on_btnLexSrcPath_clicked();
    void on_btnCfgSrcPath_clicked();
    void on_actionHomDet_triggered();
    void on_actionCfgExt_triggered();
    void on_actionLexAna_triggered();
    void on_btnHomSrc_clicked();
    void on_btnHomDst_clicked();
    void on_actionFuncScan_triggered();
    void on_btnFunPath_clicked();
    void on_actionVulnDet_triggered();
    void on_btnVulnPath_clicked();

private:
    void HomologyDetectionThread(QString hom_dst_path, int mode);
    void HomologyDetection();
    void RefreshVulnScan();

private:
    Ui::MainWindow *ui;
    bool vuln_checked_[10]; // FIXME : 写死
    size_t hom_timer_cnt_;
    QElapsedTimer hom_timer_;
    QString hom_src_;
    QStringList hom_dst_path_list_;


    list<vector<int>> pos_list_;
    list<vector<string>> info_list_;
    list<vector<int>> errlevel_list_;
    list<vector<int>> func_type_list_;
    list<vector<int>> errtype_list_;
    list<vector<int>> line_list_;

};



#endif // MAINWINDOW_H

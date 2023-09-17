#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//********************************
#include <QMap>
#include <QTimer>
#include <inttypes.h>
#include "ethercat.h"
//********************************
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getEthInfo(void);  /* 获取网卡信息 */
private slots:
    void connectToSlavers(void);   /* 配置Ethercat从站 */
    void pdoTaskTimout(void);    /* 定时器槽函数，用于过程数据通信 */
    void update_adpter_list(void); // after click scan adapter, update avialable list in select adapter
private:
    Ui::MainWindow *ui;
    QTimer *pdotimer;
    QMap<QString,QString> adapter_info;  /* 存放网卡信息 */
    QString ifname;                 /* 网卡名字 */
    char IOmap[256];                /* PDO映射的数组 */
    bool ConnectFlag;      /* 成功初始化标志位 */
};
#endif // MAINWINDOW_H

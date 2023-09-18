#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//********************************
#include <QMap>
#include <QTimer>
#include <QActionGroup>
#include <inttypes.h>
#include "ethercat.h"
#include "stayopenmenu.h"
#include "communication.h"
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

    private:
        Ui::MainWindow *ui;
        Communication *communication;
        StayOpenMenu *menuSelect_Adapter; // menu store adapter list

        void update_AdapterList(QMap<QString,QString> adapter_info); // update adapter list according to adapter_info
        void interface_init(void); // user interface initialization


    private slots:
        void on_actionScan_Adapter_triggered(void); // scan adapter

};

#endif // MAINWINDOW_H

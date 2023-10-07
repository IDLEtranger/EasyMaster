#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//********************************
#include <QMap>
#include <QTimer>
#include <QActionGroup>
#include <QTreeWidgetItem>

#include <inttypes.h>
#include "ethercat.h"
#include "stayopenmenu.h"
#include "ethercatmanager.h"
//********************************
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // Ui::MainWindow is UIC generate form mainwindow.ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class EtherCATManager;

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        EtherCATManager *ethercat_manager;
        StayOpenMenu *menuSelect_Adapter; // menu store adapter list

        void update_AdapterList(QMap<QString,QString> adapter_info); // update adapter list according to adapter_info
        void interface_init(EtherCATManager *ethercat_manager); // user interface initialization

        void build_tree_header(); // build tree header
        void init_tree(); // initialize tree


    private slots:
        void on_actionScan_Adapter_triggered(void); // scan adapter

    public slots:
        void updateInfoBrowserSlot(const QString& text); // update info browser slot
};

#endif // MAINWINDOW_H

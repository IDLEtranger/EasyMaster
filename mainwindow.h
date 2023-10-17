#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//********************************
#include <QMap>
#include <QTimer>
#include <QActionGroup>
#include <QTextCursor>
#include <thread>
#include <QScrollArea>
#include <QLabel>

#include "stayopenmenu.h"
#include "customtreeview.h"

#include <inttypes.h>
#include "ethercat.h"
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

    class FocusSlave
    {
        public:
            FocusSlave(MainWindow *mainwindow = nullptr, QString focus_slave = "", int8_t focus_slave_in_ec_slave = -1)
                : _mainwindow(mainwindow)
                , _focus_slave(focus_slave)
                , _focus_slave_in_ec_slave(focus_slave_in_ec_slave) {}

            QString get_focus_slave() const {return _focus_slave;}
            int get_focus_slave_num() const {return _focus_slave_in_ec_slave;}

            void set_focus_slave(QString new_focus_slave)
            {
                if(_focus_slave != new_focus_slave)
                {
                    _focus_slave = new_focus_slave;
                    focus_slave_Changed();
                }
            }

        private:
            MainWindow *_mainwindow; // pointer to mainwindow

            QString _focus_slave;
            int8_t _focus_slave_in_ec_slave;

            void focus_slave_Changed();
    } focus_slave;

private:
    Ui::MainWindow *ui = nullptr;
    EtherCATManager *ethercat_manager = nullptr;
    StayOpenMenu *menuSelect_Adapter = nullptr; // menu store adapter list

    QMenu *contextMenu = nullptr;
    QAction *expandAllAction = nullptr;
    QAction *collapseAllAction = nullptr;

    QActionGroup *adapterGroup = nullptr;
    QAction *No_adapter_action = nullptr;

    QLabel *label_focus_slave = nullptr;

    QTextCursor Info_Browser_cursor; // cursor for info browser

    QItemSelectionModel *slavesinfo_selectionModel = nullptr; // selection model for treeview

    // Enum For Object Directory TreeWidget
    enum TreeColNum {col_slave=0, col_index, col_subindex, col_name, col_value, col_type, col_flags};

    void update_AdapterList(QMap<QString,QString> adapter_info); // update adapter list according to adapter_info
    void interface_init(EtherCATManager *ethercat_manager); // user interface initialization

    void init_tree(); // initialize tree


private slots:
    void on_actionScan_Adapter_triggered(void); // scan adapter
    void showContextMenu_slot(const QPoint &pos); // show context menu slot
    void do_currentChanged_slot(const QModelIndex &current, const QModelIndex &previous); // do current changed slot
    void on_button_updata_clicked();
    void on_lineEdit_target_v_returnPressed();
    void on_action_OP_triggered();

public slots:
    void updateInfoBrowserSlot(const QString& text); // update info browser slot
    void change_state_indicator_slot(ec_state current_state); // change state indicator slot
};

#endif // MAINWINDOW_H

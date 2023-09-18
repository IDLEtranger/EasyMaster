#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    interface_init(); // user interface initialization
}

MainWindow::~MainWindow()
{
    delete ui;
}



/**
 * @brief MainWindow::on_actionScan_Adapter_triggered
 * call update_AdapterList function
 */
void MainWindow::on_actionScan_Adapter_triggered()
{
    update_AdapterList(communication->adapter_info_read());
}



/**
 * @brief MainWindow::render_interface
 * user interface initialization
 */
void MainWindow::interface_init(void)
{
    ui->setupUi(this); // set up ui from mainwindow.ui
    setWindowTitle(tr("EtherCAT Master"));
    // create communication object
    communication = new Communication();
    // create menu for menuAdapter_A
    menuSelect_Adapter = new StayOpenMenu(ui->menuAdapter_A);

    menuSelect_Adapter->setTitle(tr("Select Adapter"));
    ui->menuAdapter_A->addMenu(menuSelect_Adapter);
    update_AdapterList(communication->adapter_info_read()); // update adapter list according to adapter_info
}



/**
 * @brief MainWindow::update_AdapterList
 */
void MainWindow::update_AdapterList(QMap<QString,QString> adapter_info)
{
    menuSelect_Adapter->clear(); // clear menu for update
    QActionGroup *adapterGroup = new QActionGroup(this);
    adapterGroup->setExclusive(true); // Ensure only one action can be checked

    if(adapter_info.size() > 0)
    {
        QMap<QString, QString>::const_iterator iterator_adapter = adapter_info.constBegin();

        while (iterator_adapter != adapter_info.constEnd())
        {
            // create action for each adapter
            QAction *CurrentAction_adapter = new QAction(iterator_adapter.key(), this);
            CurrentAction_adapter->setCheckable(true);
            CurrentAction_adapter->setStatusTip(tr("Network Interface Name: ") + iterator_adapter.value());

            menuSelect_Adapter->addAction(CurrentAction_adapter);  // add to menu
            adapterGroup->addAction(CurrentAction_adapter);  // add to group

            connect(CurrentAction_adapter, &QAction::triggered, [this, CurrentAction_adapter, key = iterator_adapter.key(), value = iterator_adapter.value()]() {
                communication->selected_adapter = value;
                qDebug() << "selected adapter: " << communication->selected_adapter;
            });

            ++iterator_adapter;
        }
    }
    else
    {
        menuSelect_Adapter->addAction("No adapter found");
        return;
    }
}

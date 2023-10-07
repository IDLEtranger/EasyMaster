#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // set up ui from mainwindow.ui
    ethercat_manager = new EtherCATManager(); // create communication object

    connect(ethercat_manager, &EtherCATManager::update_browser,
            this, &MainWindow::updateInfoBrowserSlot);

    interface_init(ethercat_manager); // user interface initialization
    build_tree_header(); // build tree header
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ethercat_manager;
    delete menuSelect_Adapter;
};



/**
 * @brief MainWindow::on_actionScan_Adapter_triggered
 * call update_AdapterList function
 */
void MainWindow::on_actionScan_Adapter_triggered()
{
    update_AdapterList(ethercat_manager->adapter_info_read());
}



/**
 * @brief MainWindow::updateInfoBrowserSlot
 * update info browser slot
 */
void MainWindow::updateInfoBrowserSlot(const QString& text)
{
    ui->Info_Browser->append(text);
}



/**
 * @brief MainWindow::interface_init
 * user interface initialization
 */
void MainWindow::interface_init(EtherCATManager *ethercat_manager)
{
    setWindowTitle(tr("EasyMaster"));
    setWindowIcon(QIcon(":/my_images/icon/WINDOWS10ICON/WINDOWS10ICON/Win 10 Icon (156).png"));

    // create menu for menuAdapter_A
    menuSelect_Adapter = new StayOpenMenu(ui->menuAdapter_A);

    menuSelect_Adapter->setTitle(tr("Select Adapter"));

    ui->menuAdapter_A->addMenu(menuSelect_Adapter);

    update_AdapterList(ethercat_manager->adapter_info_read()); // update adapter list according to adapter_info

    // render indicator light


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
                ethercat_manager->current_selected_adapter = {key, value};
                ethercat_manager->connect2slave();
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



/**
 * @brief MainWindow::build_tree_header
 * build tree header
 */
void MainWindow::build_tree_header()
{
    ui->treeSlaves->clear();
    ui->treeSlaves->setColumnCount(6);
    QTreeWidgetItem *header = new QTreeWidgetItem();
    header->setText(EtherCATManager::colODIndex, "Index");
    header->setText(EtherCATManager::colODName, "Name");
    header->setText(EtherCATManager::colODValue, "Value");
    header->setText(EtherCATManager::colODInOUT, "In/Out");
    header->setText(EtherCATManager::colODType, "Type");
    header->setText(EtherCATManager::colODBitlen, "Bitlength");
    header->setTextAlignment(EtherCATManager::colODIndex, Qt::AlignCenter);
    header->setTextAlignment(EtherCATManager::colODName, Qt::AlignCenter);
    header->setTextAlignment(EtherCATManager::colODValue, Qt::AlignCenter);
    header->setTextAlignment(EtherCATManager::colODInOUT, Qt::AlignCenter);
    header->setTextAlignment(EtherCATManager::colODType, Qt::AlignCenter);
    header->setTextAlignment(EtherCATManager::colODBitlen, Qt::AlignCenter);
    ui->treeSlaves->setHeaderItem(header);
}

/**
 * @brief MainWindow::init_tree
 * initialize tree
 */
void MainWindow::init_tree()
{

}




#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // set up ui from mainwindow.ui

    /******** create communication object ********/
    ethercat_manager = new EtherCATManager(); // create communication object

    /******** connect slot with signal ********/
    connect(ethercat_manager, &EtherCATManager::updateInfoBrowser,
            this, &MainWindow::updateInfoBrowserSlot);

    /******** initialize ********/
    Info_Browser_cursor = ui->Info_Browser->textCursor();
    interface_init(ethercat_manager); // user interface initialization
    build_tree_header(); // build slaveinfo tree header

}

MainWindow::~MainWindow()
{
    delete ui;
    delete ethercat_manager;
};



/******** SLOT Function ********/

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
    Info_Browser_cursor.movePosition(QTextCursor::End);
    Info_Browser_cursor.insertText(text);
}


/**
 * @brief MainWindow::change_state_indicator_slot
 * change state indicator, called by ethercat_manager->change_state_indicator()
 */
void MainWindow::change_state_indicator_slot(ec_state current_state)
{
    static ec_state last_state = EC_STATE_NONE;

    // use map to store state and corresponding QAction
    QMap<ec_state, QAction*> stateToAction = {
        {EC_STATE_NONE, nullptr}, // or another QAction if EC_STATE_NONE has one
        {EC_STATE_BOOT, ui->action_Boot_Strap},
        {EC_STATE_INIT, ui->action_Init},
        {EC_STATE_PRE_OP, ui->action_Pre_OP},
        {EC_STATE_SAFE_OP, ui->action_Safe_OP},
        {EC_STATE_OPERATIONAL, ui->action_OP},
        {EC_STATE_ACK, nullptr},
        {EC_STATE_ERROR, nullptr}
    };

    auto setIconGray = [&](ec_state state) {
        if(stateToAction.contains(state) && stateToAction[state])
            stateToAction[state]->setIcon(QIcon(":/my_images/icon/grey light.png"));
    };

    // set icon according to last state
    setIconGray(last_state);

    // set icon according to current state
    if(stateToAction.contains(current_state) && stateToAction[current_state])
        stateToAction[current_state]->setIcon(QIcon(":/my_images/icon/green light.png"));

    last_state = current_state;
}

/******** Private Function ********/

/**
 * @brief MainWindow::interface_init
 * user interface initialization
 */
void MainWindow::interface_init(EtherCATManager *ethercat_manager)
{
    setWindowTitle(tr("EasyMaster"));
    setWindowIcon(QIcon(":/my_images/icon/WINDOWS10ICON/WINDOWS10ICON/Win 10 Icon (156).png"));

    // set up state indicator bar separator style
    ui->state_indicator_bar->setStyleSheet("QToolBar::separator { background: transparent; border: none; width: 20px; }");

    // show current slave in state indicator bar
    QWidget *spacerWidget = new QWidget();
    QWidget *leftSpacerWidget = new QWidget();
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    leftSpacerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    leftSpacerWidget->setFixedWidth(20);
    ui->state_indicator_bar->addWidget(spacerWidget);
    ui->state_indicator_bar->addWidget(new QLabel("Current Slave:"));
    ui->state_indicator_bar->addWidget(leftSpacerWidget);

    // create menu for menuAdapter_A
    menuSelect_Adapter = new StayOpenMenu(ui->menuAdapter_A);
    menuSelect_Adapter->setTitle(tr("Select Adapter"));
    ui->menuAdapter_A->addMenu(menuSelect_Adapter);

    // add default action to menuSelect_Adapter
    menuSelect_Adapter->addAction("No adapter found");
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

            connect(CurrentAction_adapter, &QAction::triggered, this, [this, CurrentAction_adapter, key = iterator_adapter.key(), value = iterator_adapter.value()]() {
                ethercat_manager->current_selected_adapter = {key, value};
                ui->Info_Browser->clear(); // clear info browser

                std::thread slaveinfo_thread([&]() { ethercat_manager->connect2slaves(); });
                slaveinfo_thread.detach();
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

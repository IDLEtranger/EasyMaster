#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // set up ui from mainwindow.ui

    /******** create communication object ********/
    ethercat_manager = new EtherCATManager(this); // create communication object

    /******** create focus slave object ********/
    focus_slave = FocusSlave(this);

    /******** create selection model ********/
    slavesinfo_selectionModel = new QItemSelectionModel(ethercat_manager->slavesinfo_Model);

    /******** create info browser cursor ********/
    Info_Browser_cursor = ui->Info_Browser->textCursor();

    /******** initialize ********/
    interface_init(ethercat_manager); // user interface initialization

    /******** connect slot with signal ********/
    connect(ethercat_manager, &EtherCATManager::updateInfoBrowser, this, &MainWindow::updateInfoBrowserSlot);
    connect(ethercat_manager, &EtherCATManager::change_state_indicator, this, &MainWindow::change_state_indicator_slot);

    connect(ui->tree_slaves, &CustomTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu_slot);
    connect(ui->tree_slaves, &CustomTreeView::clicked, [this](const QModelIndex &index) {
        if (!index.isValid()) {
            ui->tree_slaves->clearSelection();
        }
    });

    connect(expandAllAction, &QAction::triggered, ui->tree_slaves, &CustomTreeView::expandAll);
    connect(collapseAllAction, &QAction::triggered, ui->tree_slaves, &CustomTreeView::collapseAll);

    connect(slavesinfo_selectionModel, &QItemSelectionModel::currentChanged, this, &MainWindow::do_currentChanged_slot);
}

MainWindow::~MainWindow()
{
    delete ui;
};



/******** FocusSlave Class ********/
/**
 * @brief MainWindow::FocusSlave::focus_slave_Changed
 * focus slave changed
 */
void MainWindow::FocusSlave::focus_slave_Changed()
{
    _mainwindow->label_focus_slave->setText(_focus_slave);
    _focus_slave_in_ec_slave = _mainwindow->ethercat_manager->slaveName2index[_focus_slave];
    _mainwindow->change_state_indicator_slot(ec_state(ec_slave[_focus_slave_in_ec_slave].state));
}


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

/********  !State Related!  ********/
/**
 * @brief MainWindow::change_state_indicator_slot
 * change state indicator, called by ethercat_manager->change_state_indicator()
 */
void MainWindow::change_state_indicator_slot(ec_state current_state)
{
    // store last state
    static ec_state last_state = EC_STATE_NONE;

    // use map to store state and corresponding QAction
    static QMap<ec_state, QAction*> stateToAction =
    {
        {EC_STATE_NONE, nullptr},
        {EC_STATE_BOOT, ui->action_Boot_Strap},
        {EC_STATE_INIT, ui->action_Init},
        {EC_STATE_PRE_OP, ui->action_Pre_OP},
        {EC_STATE_SAFE_OP, ui->action_Safe_OP},
        {EC_STATE_OPERATIONAL, ui->action_OP},
        {EC_STATE_ACK, nullptr},
        {EC_STATE_ERROR, nullptr}
    };

    // set icon to gray
    auto setIconGray = [&](ec_state state) {
        if(stateToAction.contains(state) && stateToAction[state])
            stateToAction[state]->setIcon(QIcon(":/my_images/icon/grey light.png"));
    };

    // set last state icon grey
    setIconGray(last_state);

    // set current state icon green
    if(stateToAction.contains(current_state) && stateToAction[current_state])
        stateToAction[current_state]->setIcon(QIcon(":/my_images/icon/green light.png"));

    last_state = current_state;

    // Init can only convert to Pre_OP, Pre_OP cannot convert to OP
    if(current_state == EC_STATE_INIT)
    {
        for(auto it = stateToAction.begin(); it != stateToAction.end(); ++it)
        {
            if(it.key() != EC_STATE_PRE_OP && it.value()) // Ensure the action isn't Pre_OP and it's not nullptr
            {
                it.value()->setDisabled(true); // Disable the action
            }
            else if(it.key() == EC_STATE_PRE_OP && it.value()) // If it's Pre_OP, ensure it's enabled
            {
                it.value()->setEnabled(true);
            }
        }
    }
    if(current_state == EC_STATE_PRE_OP)
    {
        stateToAction[EC_STATE_OPERATIONAL]->setDisabled(true);
    }
}


/**
 * @brief MainWindow::on_action_OP_triggered
 */
void MainWindow::on_action_OP_triggered()
{
    if(focus_slave.get_focus_slave_num() != -1)
    {
        ec_slave[focus_slave.get_focus_slave_num()].state = EC_STATE_OPERATIONAL;
        ec_writestate(focus_slave.get_focus_slave_num());
    }
}



/**
 * @brief MainWindow::showContextMenu
 * show context menu
 */
void MainWindow::showContextMenu_slot(const QPoint &pos)
{
    contextMenu->exec(QCursor::pos());
}



/**
 * @brief MainWindow::do_currentChanged_slot
 * do current changed slot
 */
void MainWindow::do_currentChanged_slot(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // deal with user interface
    ui->lineEdit_target_v->setEnabled(false);
    ui->lineEdit_target_v->clear();
    ui->button_updata->setEnabled(false);

    QModelIndex current_mainNodeIndex = current.sibling(current.row(), 0);
    auto is_bottom = [&](QModelIndex current_mainNodeIndex){return ethercat_manager->slavesinfo_Model->rowCount(current_mainNodeIndex) == 0;};
    auto is_top = [&](QModelIndex current_mainNodeIndex){return !(current_mainNodeIndex.parent().isValid());};

    if(current.isValid() && is_bottom(current_mainNodeIndex)) // bottom node behavior
    {
        QModelIndex current_parent_index = current_mainNodeIndex.parent().siblingAtColumn(col_index);
        QString labelText = "0x" + current_parent_index.data().toString() + ":" + current_mainNodeIndex.siblingAtColumn(col_subindex).data().toString();
        ui->label_name_v->setText(current_mainNodeIndex.siblingAtColumn(col_name).data().toString());
        ui->label_index_v->setText(labelText);
        ui->label_current_value_v->setText(current_mainNodeIndex.siblingAtColumn(col_value).data().toString());
        if(current_mainNodeIndex.siblingAtColumn(col_flags).data().toString().contains("RW"))
        {
            ui->lineEdit_target_v->setEnabled(true);
            ui->button_updata->setEnabled(true);
        }
        focus_slave.set_focus_slave(current_mainNodeIndex.parent().parent().siblingAtColumn(col_slave).data().toString());
    }
    else if(is_top(current_mainNodeIndex)) // top node behavior
    {
        ui->label_name_v->clear();
        ui->label_index_v->clear();
        ui->label_current_value_v->clear();
        focus_slave.set_focus_slave(current_mainNodeIndex.siblingAtColumn(col_slave).data().toString());
    }
    else
    {
        ui->label_name_v->setText(current_mainNodeIndex.siblingAtColumn(col_name).data().toString());
        QString labelText = "0x" + current_mainNodeIndex.siblingAtColumn(1).data().toString();
        ui->label_index_v->setText(labelText);
        ui->label_current_value_v->clear();
        focus_slave.set_focus_slave(current_mainNodeIndex.parent().siblingAtColumn(col_slave).data().toString());
    }
}


/**
 * @brief MainWindow::on_button_updata_clicked
 */
void MainWindow::on_button_updata_clicked()
{
    if(!ui->lineEdit_target_v->text().isEmpty())
    {
        const QVariant target_value = ui->lineEdit_target_v->text();
        qDebug() << target_value.toString();
        ui->lineEdit_target_v->clear();
    }
}



/**
 * @brief MainWindow::on_lineEdit_target_v_returnPressed
 */
void MainWindow::on_lineEdit_target_v_returnPressed()
{
    on_button_updata_clicked();
}

/******** Private Function ********/

/**
 * @brief MainWindow::interface_init
 * user interface initialization
 */
void MainWindow::interface_init(EtherCATManager *ethercat_manager)
{
    // set window title and icon
    setWindowTitle(tr("EasyMaster"));
    setWindowIcon(QIcon(":/my_images/icon/WINDOWS10ICON/WINDOWS10ICON/Win 10 Icon (156).png"));

    /******** create context menu ********/
    expandAllAction = new QAction("Expand All", this);
    collapseAllAction = new QAction("Collapse All", this);
    contextMenu = new QMenu(this);
    expandAllAction->setIcon(QIcon(":/my_images/icon/expand_all.png"));
    collapseAllAction->setIcon(QIcon(":/my_images/icon/collapse_all.png"));
    contextMenu->addAction(expandAllAction);
    contextMenu->addAction(collapseAllAction);

    /******** treeview ********/
    ui->tree_slaves->setContextMenuPolicy(Qt::CustomContextMenu); // enable custom context menu
    ui->tree_slaves->setModel(ethercat_manager->slavesinfo_Model);
    ui->tree_slaves->setSelectionModel(slavesinfo_selectionModel);
    ui->tree_slaves->setEditTriggers(QAbstractItemView::NoEditTriggers); // disable edit
    ui->tree_slaves->setSelectionMode(QAbstractItemView::SingleSelection); // single selection
    ui->tree_slaves->setSelectionBehavior(QAbstractItemView::SelectRows); // select rows

    // disable lineEdit and update button
    ui->lineEdit_target_v->setEnabled(false);
    ui->button_updata->setEnabled(false);

    // set up state indicator bar separator style
    ui->state_indicator_bar->setStyleSheet("QToolBar::separator { background: transparent; border: none; width: 20px; }");
    ui->state_indicator_bar->setIconSize(QSize(12, 12));

    // show current slave in state indicator bar
    QWidget *spacerWidget = new QWidget();
    QWidget *leftSpacerWidget = new QWidget();
    label_focus_slave = new QLabel(focus_slave.get_focus_slave());
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    leftSpacerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    leftSpacerWidget->setFixedWidth(20);
    ui->state_indicator_bar->addWidget(spacerWidget);
    ui->state_indicator_bar->addWidget(label_focus_slave);
    ui->state_indicator_bar->addWidget(new QLabel("Current Slave: "));
    ui->state_indicator_bar->addWidget(leftSpacerWidget);

    // set up splitter default ratio
    ui->splitter_horizontal->setStretchFactor(0, 18);
    ui->splitter_horizontal->setStretchFactor(1, 7);
    ui->splitter_vertical->setStretchFactor(0,16);
    ui->splitter_vertical->setStretchFactor(1, 6);

    // hide border of groupbox
    ui->groupBox->setStyleSheet("QGroupBox { border: none; }");

    // create menu for menuAdapter_A
    menuSelect_Adapter = new StayOpenMenu(ui->menuAdapter_A);
    menuSelect_Adapter->setTitle(tr("Select Adapter"));
    ui->menuAdapter_A->addMenu(menuSelect_Adapter);

    // add default action to menuSelect_Adapter
    QAction *No_adapter_action = new QAction("No adapter found", this);
    menuSelect_Adapter->addAction(No_adapter_action);

    // set treeview each column's default width
    ui->tree_slaves->setColumnWidth(col_slave, 100);
    ui->tree_slaves->setColumnWidth(col_index, 40);
    ui->tree_slaves->setColumnWidth(col_subindex, 30);
    ui->tree_slaves->setColumnWidth(col_name, 150);
    ui->tree_slaves->setColumnWidth(col_value, 150);
    ui->tree_slaves->setColumnWidth(col_type, 150);
    ui->tree_slaves->setColumnWidth(col_flags, 50);
}



/**
 * @brief MainWindow::update_AdapterList
 */
void MainWindow::update_AdapterList(QMap<QString,QString> adapter_info)
{
    static QMap<QString,QString> last_adapter_info;

    if(adapterGroup != nullptr)
    {
        adapterGroup->setExclusive(false);
        foreach(QAction *action, adapterGroup->actions())
        {
            action->setChecked(false);
        }
        adapterGroup->setExclusive(true);
    }

    if(last_adapter_info != adapter_info)
    {
        menuSelect_Adapter->clear(); // clear menu for update
        adapterGroup = new QActionGroup(this);
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

                // connect current action with slot(lambda function)
                connect(CurrentAction_adapter, &QAction::toggled, this, [this, CurrentAction_adapter, key = iterator_adapter.key(), value = iterator_adapter.value()](bool checked) {
                    if (checked)
                    {
                        // clear info browser
                        ui->Info_Browser->clear();
                        // clear slavesinfo_Model, but keep header
                        ethercat_manager->slavesinfo_Model->removeRows(0, ethercat_manager->slavesinfo_Model->rowCount());
                        // close ethercat communication
                        ec_close();

                        ethercat_manager->current_selected_adapter = {key, value};
                        // Set the cursor to a wait cursor
                        QApplication::setOverrideCursor(Qt::WaitCursor);
                        ethercat_manager->connect2slaves();
                        QApplication::restoreOverrideCursor();
                    }
                });

                ++iterator_adapter;
            }
            last_adapter_info = adapter_info;
        }
        else
        {;
            menuSelect_Adapter->addAction(No_adapter_action);
            last_adapter_info = adapter_info;
        }
    }
}


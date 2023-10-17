#ifndef ETHERCATMANAGER_H
#define ETHERCATMANAGER_H
//********************************
#include <QMap>
#include <QTimer>
#include <QDebug>
#include <QStandardItemModel>

#include <inttypes.h>
#include "ethercat.h"
//********************************
#define SLAVEINFO_DISPALY

#ifdef SLAVEINFO_DISPALY
#define LOG_DISPLAY(...) emit updateInfoBrowser(QString::asprintf(__VA_ARGS__))
#else
#define LOG_DISPLAY(...)
#endif
//********************************
class MainWindow;

class EtherCATManager : public QObject
{
    Q_OBJECT
    friend class MainWindow;

public:
    // store selected adapter information
    struct
    {
        QString name;
        QString desc;
    } current_selected_adapter;

    /******** Function ********/
    EtherCATManager(MainWindow *mainwindow);
    ~EtherCATManager();

    QMap<QString,QString> adapter_info_read(void); // read adapter information
    void connect2slaves(void); // connect to slave
    void get_netinfo(void); // get net information

    void startPollingstate(int period);

private:
    MainWindow *_mainwindow; // pointer to mainwindow

    /******** Variables for EtherCAT communication ********/
    QMap<QString,QString> adapter_info; // store adapter information
    QMap<QString,int8_t> slaveName2index; // store slave name and corresponding index
    char IOmap[4096]; // store IOmap
    ec_ODlistt ODlist; // store object directory list
    ec_OElistt OElist; // store object entry list
    boolean printSDO = TRUE;
    boolean printMAP = TRUE;
    char usdo[128];
    char hstr[1024];

    // Model-View-Variable
    QStandardItemModel *slavesinfo_Model; // model store slaves information
    enum Model_row_col {FixedColumnCount = 7};
    QList<QModelIndex> slaveIndices;

    // ESM state poll
    QTimer *poll_timer;


    /******** Function ********/
    QString objectaccess2string(uint8 objectaccess); // convert object access to string
    enum {Pre_R = 0b00000001, Safe_R = 0b00000010, OP_R = 0b00000100, Pre_W = 0b00001000, Safe_W = 0b00010000, OP_W = 0b00100000, RxPDO_map = 0b01000000, TxPDO_map = 0b10000000};

    QString objectcode2string(uint8 objectcode); // convert object code to string
    char* dtype2string(uint16 dtype); // convert data type to string
    char* SDO2string(uint16 slave, uint16 index, uint8 subidx, uint16 dtype); // convert SDO to string

    int slaveinfo_PDOassign(uint16 slave, uint16 PDOassign, int mapoffset, int bitoffset); // get PDO assign, called by slaveinfo_map_SM()
    int slaveinfo_SII_RTxPDO(uint16 slave, uint8 t, int mapoffset, int bitoffset); // get Rx/TxPDO information, called by slaveinfo_map_SII()

    void slaveinfo_sdo(int cnt); // get slave SDO information, called by slaveinfo()
    int slaveinfo_map_SII(int slave); // called by slaveinfo()
    int slaveinfo_map_SM(int slave); // get slave SM, called by slaveinfo()

    void slavesinfo(char *ifname);

    void init_slavesinfo_ModelData(); // initialize slavesinfo_Model data

private slots:
    void poll_timer_timeout(void); // timer slot function, used for ESM state poll

signals:
    void updateInfoBrowser(const QString& text); // update info browser signal
    void change_state_indicator(ec_state current_state); // change state indicator signal
};

#endif // ETHERCATMANAGER_H

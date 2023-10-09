#ifndef ETHERCATMANAGER_H
#define ETHERCATMANAGER_H
//********************************
#include <QMap>
#include <QTimer>
#include <QDebug>

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

class EtherCATManager : public QObject
{
    Q_OBJECT
    friend class MainWindow;

    public:
        // store selected adapter information
        struct adapter_desc
        {
            QString name;
            QString desc;
        } current_selected_adapter;

        /******** Function ********/
        EtherCATManager();

        QMap<QString,QString> adapter_info_read(void); // read adapter information
        void connect2slaves(void); // connect to slave

    private:
        /******** Variables for EtherCAT communication ********/
        QMap<QString,QString> adapter_info; // store adapter information
        //QTimer *pdotimer;

        char IOmap[4096]; // store IOmap
        ec_ODlistt ODlist; // store object directory list
        ec_OElistt OElist; // store object entry list
        boolean printSDO = TRUE;
        boolean printMAP = TRUE;
        char usdo[128];
        char hstr[1024];

        // Enum For Object Directory TreeWidget
        enum TreeItemType {itGroupItem=1001, itSlaveItem, itODItem};
        enum TreeColNum {colItem=0, colODIndex, colODName, colODValue, colODInOUT, colODType, colODBitlen};

        /******** Function ********/
        void get_netinfo(void); // get net information

        char* dtype2string(uint16 dtype); // convert data type to string
        char* SDO2string(uint16 slave, uint16 index, uint8 subidx, uint16 dtype); // convert SDO to string

        int slaveinfo_PDOassign(uint16 slave, uint16 PDOassign, int mapoffset, int bitoffset); // get PDO assign, called by slaveinfo_map_SM()
        int slaveinfo_SII_RTxPDO(uint16 slave, uint8 t, int mapoffset, int bitoffset); // get Rx/TxPDO information, called by slaveinfo_map_SII()

        void slaveinfo_sdo(int cnt); // get slave SDO information, called by slaveinfo()
        int slaveinfo_map_SII(int slave); // called by slaveinfo()
        int slaveinfo_map_SM(int slave); // get slave SM, called by slaveinfo()

        void slaveinfo(char *ifname);

    private slots:
        //void pdoTaskTimout(void); // timer slot function, used for process data communication

    signals:
        void updateInfoBrowser(const QString& text); // update info browser signal
        void change_state_indicator(ec_state current_state); // change state indicator signal
};

#endif // ETHERCATMANAGER_H

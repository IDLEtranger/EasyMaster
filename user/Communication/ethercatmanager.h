#ifndef ETHERCATMANAGER_H
#define ETHERCATMANAGER_H
//********************************
#include <QMap>
#include <QTimer>
#include <inttypes.h>
#include "ethercat.h"
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
        void connect2slave(void); // connect to slave

    private:
        /******** Variables for EtherCAT communication ********/
        QMap<QString,QString> adapter_info; // store adapter information
        QTimer *pdotimer;

        char IOmap[4096]; // store IOmap
        ec_ODlistt ODlist; // store object directory list
        ec_OElistt OElist; // store object entry list
        boolean printSDO = FALSE;
        boolean printMAP = FALSE;
        char usdo[128];
        char hstr[1024];
        //Enum For Object Directory TreeWidget
        enum TreeItemType{itGroupItem=1001, itSlaveItem, itODItem};
        enum TreeColNum{colItem=0, colODIndex, colODName, colODValue, colODInOUT, colODType, colODBitlen};

        /******** Function ********/
        void get_netinfo(void);  // get net information
        void get_ODlist(void); // get object directory list

        char* dtype2string(uint16 dtype); // convert data type to string
        char* SDO2string(uint16 slave, uint16 index, uint8 subidx, uint16 dtype); // convert SDO to string
        int slaveinfo_PDOassign(uint16 slave, uint16 PDOassign, int mapoffset, int bitoffset); // get PDO assign
        int slaveinfo_map_SM(int slave); // get slave SM
        int slaveinfo_SII_RTxPDO(uint16 slave, uint8 t, int mapoffset, int bitoffset); // get SII PDO information
        int slaveinfo_map_SII(int slave);
        void slaveinfo_sdo(int cnt);

    private slots:
        void pdoTaskTimout(void); // timer slot function, used for process data communication

    signals:
        void update_browser(const QString& text); // update info browser signal
};

#endif // ETHERCATMANAGER_H

#ifndef COMMUNICATION_H
#define COMMUNICATION_H
//********************************
#include <QMap>
#include <QTimer>
#include <QActionGroup>
#include <inttypes.h>
#include "ethercat.h"
#include "stayopenmenu.h"
#include "communication.h"
//********************************

class Communication
{
    public:
        QString selected_adapter; // store selected adapter name

        Communication();
        QMap<QString,QString> adapter_info_read(void); // read adapter information

    private slots:
        void pdoTaskTimout(void); // timer slot function, used for process data communication
        //void connect2slave(void); // connect to slave

    private:
        QMap<QString,QString> adapter_info; // store adapter information
        char IOmap[256]; // store IOmap
        bool ConnectFlag; // connect flag
        QTimer *pdotimer;

        void get_netinfo(void);  // get net information
};

#endif // COMMUNICATION_H

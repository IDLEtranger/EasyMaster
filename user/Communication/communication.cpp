#include "communication.h"

Communication::Communication()
{
    // get net info
    get_netinfo();
}

/**
 * @brief Communication::get_netinfo
 * get net information
 */
void Communication::get_netinfo(void)
{
    ec_adaptert * adapter = NULL;
    adapter = ec_find_adapters();
    while(adapter != NULL)
    {
        adapter_info.insert(adapter->desc, adapter->name);
        adapter = adapter->next;
    }
}

/**
 * @brief Communication::pdoTaskTimout
 * timer slot function, used for process data communication
 */
void Communication::pdoTaskTimout(void)
{
    int wkc = 0;
    ec_send_processdata();
    wkc = ec_receive_processdata(EC_TIMEOUTRET);
}



/**
 * @brief Communication::adapter_info_read
 * read adapter information
 */
QMap<QString,QString> Communication::adapter_info_read(void)
{
    return adapter_info;
}



/**
 * @brief myMainWindow::connectToSlavers
 * 连接按钮槽函数，用于配置从站
 */
//void Communication::connect2slave(void)
//{
//    int  chk;
//    char *eth0;
//    // get net info
//    QByteArray ba = ethinfo[ifname].toLatin1();
//    eth0 = ba.data();
//    /* 绑定网卡  */
//    if(ConnectFlag == false)
//    {
//        ui->textBrowser->append("start soem\n");
//        if (ec_init(eth0))
//        {
//            ui->textBrowser->append("ec_init on ifname "+ ifname+ " succeeded.\n");
//            ui->textBrowser->append(QString::asprintf("ifname dese is %s",eth0));
//            /* 扫描从站 */
//            if ( ec_config_init(FALSE) > 0 )
//            {
//                ui->textBrowser->append(QString::asprintf("%d slaves found and configured.\n",ec_slavecount));
//                ec_configdc();
//                /* 配置DC周期 周期理想的状态是和定时器的周期一样，但是由于定时器精度不够，所以周期需要增加，测试不建议使用DC同步模式 */
//                ec_dcsync0(1, TRUE, 25000000,0);
//                ec_config_map(&IOmap);
//                ui->textBrowser->append("Slaves mapped, state to SAFE_OP.\n");
//                ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);
//                ui->textBrowser->append(QString::asprintf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]));
//                ui->textBrowser->append("Request operational state \n");

//                ec_slave[0].state = EC_STATE_OPERATIONAL;
//                /* 发送一帧 PDO数据 准备进入OP状态 */
//                ec_send_processdata();
//                ec_receive_processdata(EC_TIMEOUTRET);
//                /* 启动PDO通信任务 */
//                pdotimer->start(20);
//                /* OP状态请求 */
//                ec_writestate(0);
//                chk = 210;
//                /* 等待所有从站进入OP状态 */
//                do
//                {
//                    ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
//                    ec_send_processdata();
//                    ec_receive_processdata(EC_TIMEOUTRET);
//                }
//                while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
//                ui->textBrowser->append(QString::asprintf("chk =%d,%d",chk,ec_slave[1].ALstatuscode));

//                if (ec_slave[0].state == EC_STATE_OPERATIONAL )
//                {
//                    ui->textBrowser->append("Operational state reached .\n");
//                    ConnectFlag = true;
//                }else{
//                    /* 关闭连接 */
//                    ui->textBrowser->append("Operational state fail!!! connect closed\n");
//                    ConnectFlag = false;
//                    pdotimer->stop();
//                    ec_close();
//                }
//            }
//        }
//    }else{
//        /* 关闭连接 */
//        ui->textBrowser->append("close soem\n");
//        ConnectFlag = false;
//        pdotimer->stop();
//        ec_close();
//    }
//    if(ConnectFlag ==  true)
//        ui->con_pushButton->setText(QString::fromLocal8Bit("断开"));
//    else
//        ui->con_pushButton->setText(QString::fromLocal8Bit("连接"));
//}

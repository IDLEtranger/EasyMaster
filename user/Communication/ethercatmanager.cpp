#include "ethercatmanager.h"

EtherCATManager::EtherCATManager()
{
    // get ethercat info
    get_netinfo();
}

/**
 * @brief EtherCATManager::get_netinfo
 * get net information
 */
void EtherCATManager::get_netinfo(void)
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
 * @brief EtherCATManager::get_ODlist
 * get object directory list
 */
void EtherCATManager::get_ODlist(void)
{
    for(int i = 1; i <= ec_slavecount ; i++)
    {
        ec_readODlist(i, &ODlist);
        // display ever OD description in debug console
        //qDebug() << "ODlist of slave " << i << " : ";
        //qDebug() << ODlist->Index << ODlist->Name << ODlist->DataType << ODlist->Entries << ODlist->ObjectCode << ODlist->MaxSub;
    }
}



/**
 * @brief EtherCATManager::pdoTaskTimout
 * timer slot function, used for process data communication
 */
void EtherCATManager::pdoTaskTimout(void)
{
    int wkc = 0;
    ec_send_processdata();
    wkc = ec_receive_processdata(EC_TIMEOUTRET);
}



/**
 * @brief EtherCATManager::adapter_info_read
 * read adapter information
 */
QMap<QString,QString> EtherCATManager::adapter_info_read(void)
{
    return adapter_info;
}



/**
 * @brief myMainWindow::connectToSlavers
 * 连接按钮槽函数，用于配置从站
 */
void EtherCATManager::connect2slave(void)
{
    QByteArray byteArray = current_selected_adapter.desc.toUtf8();
    char* eth = byteArray.data();

    // connect to slave
    emit update_browser("start soem\n");

    if(ec_init(eth)) // if OK
    {
        emit update_browser("ec_init on ifname "+ current_selected_adapter.name + " succeeded.\n");
        emit update_browser(QString::asprintf("ifname desc is %s", eth));

        // scan slaves
        if(ec_config_init(FALSE) > 0) // slave count > 0
        {
            emit update_browser(QString::asprintf("%d slaves found and configured.\n", ec_slavecount));
//            // modulate DC time
//            ec_dcsync0(1, TRUE, 25000000,0);

//            ec_config_map(&IOmap);

//            ec_configdc();

//            emit update_browser("Slaves mapped, state to SAFE_OP.\n");

//            // wait for all slaves to reach SAFE_OP state
//            ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

//            emit update_browser(QString::asprintf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]));
//            emit update_browser("Request operational state \n");

//            ec_slave[0].state = EC_STATE_OPERATIONAL;

//            // Send a frame of PDO and prepare to enter the OP state
//            ec_send_processdata();
//            ec_receive_processdata(EC_TIMEOUTRET);

//            // start PDO timer
//            pdotimer->start(20);

//            // request OP state for all slaves
//            ec_writestate(0);

//            // wait for all slaves to reach OP state
//            int num_try = 210;
//            do
//            {
//                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
//            }
//            while (num_try-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

//            if (ec_slave[0].state == EC_STATE_OPERATIONAL )
//            {
//                emit update_browser("Operational state reached for all slaves.\n");
//            }
//            else
//            {
//                emit update_browser("Not all slaves reached operational state.\n");
//                for(int i = 1; i<=ec_slavecount ; i++)
//                {
//                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
//                    {
//                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
//                               i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
//                    }
//                }
//                pdotimer->stop();
//                ec_close();
//            }
        }
    }
    else
    {
        // close connection
        emit update_browser("Init failed, close soem.\n");
        pdotimer->stop();
        ec_close();
    }
}



/**
 * @brief EtherCATManager::dtype2string
 * @param dtype
 * @return char* hstr
 */
char* EtherCATManager::dtype2string(uint16 dtype)
{
    switch(dtype)
    {
    case ECT_BOOLEAN:
        sprintf(hstr, "BOOLEAN");
        break;
    case ECT_INTEGER8:
        sprintf(hstr, "INTEGER8");
        break;
    case ECT_INTEGER16:
        sprintf(hstr, "INTEGER16");
        break;
    case ECT_INTEGER32:
        sprintf(hstr, "INTEGER32");
        break;
    case ECT_INTEGER24:
        sprintf(hstr, "INTEGER24");
        break;
    case ECT_INTEGER64:
        sprintf(hstr, "INTEGER64");
        break;
    case ECT_UNSIGNED8:
        sprintf(hstr, "UNSIGNED8");
        break;
    case ECT_UNSIGNED16:
        sprintf(hstr, "UNSIGNED16");
        break;
    case ECT_UNSIGNED32:
        sprintf(hstr, "UNSIGNED32");
        break;
    case ECT_UNSIGNED24:
        sprintf(hstr, "UNSIGNED24");
        break;
    case ECT_UNSIGNED64:
        sprintf(hstr, "UNSIGNED64");
        break;
    case ECT_REAL32:
        sprintf(hstr, "REAL32");
        break;
    case ECT_REAL64:
        sprintf(hstr, "REAL64");
        break;
    case ECT_BIT1:
        sprintf(hstr, "BIT1");
        break;
    case ECT_BIT2:
        sprintf(hstr, "BIT2");
        break;
    case ECT_BIT3:
        sprintf(hstr, "BIT3");
        break;
    case ECT_BIT4:
        sprintf(hstr, "BIT4");
        break;
    case ECT_BIT5:
        sprintf(hstr, "BIT5");
        break;
    case ECT_BIT6:
        sprintf(hstr, "BIT6");
        break;
    case ECT_BIT7:
        sprintf(hstr, "BIT7");
        break;
    case ECT_BIT8:
        sprintf(hstr, "BIT8");
        break;
    case ECT_VISIBLE_STRING:
        sprintf(hstr, "VISIBLE_STRING");
        break;
    case ECT_OCTET_STRING:
        sprintf(hstr, "OCTET_STRING");
        break;
    default:
        sprintf(hstr, "Type 0x%4.4X", dtype);
    }
    return hstr;
}



/**
 * @brief EtherCATManager::SDO2string
 * read sdo value in string
 */
char* EtherCATManager::SDO2string(uint16 slave, uint16 index, uint8 subidx, uint16 dtype)
{
    int length = sizeof(usdo) - 1;
    int i;
    float *sr;
    double *dr;
    char es[32];

    uint8 *u8;
    int8 *i8;
    uint16 *u16;
    int16 *i16;
    uint32 *u32;
    int32 *i32;
    uint64 *u64;
    int64 *i64;

    memset(&usdo, 0, 128);

    ec_SDOread(slave, index, subidx, FALSE, &length, &usdo, EC_TIMEOUTRXM);
    if (EcatError)
    {
        return ec_elist2string();
    }
    else
    {
        switch(dtype)
        {
        case ECT_BOOLEAN:
            u8 = (uint8*) &usdo[0];
            if (*u8) sprintf(hstr, "TRUE");
            else sprintf(hstr, "FALSE");
            break;
        case ECT_INTEGER8:
            i8 = (int8*) &usdo[0];
            sprintf(hstr, "0x%2.2x %d", *i8, *i8);
            break;
        case ECT_INTEGER16:
            i16 = (int16*) &usdo[0];
            sprintf(hstr, "0x%4.4x %d", *i16, *i16);
            break;
        case ECT_INTEGER32:
        case ECT_INTEGER24:
            i32 = (int32*) &usdo[0];
            sprintf(hstr, "0x%8.8x %d", *i32, *i32);
            break;
        case ECT_INTEGER64:
            i64 = (int64*) &usdo[0];
            sprintf(hstr, "0x%16.16" PRIx64 " %" PRId64, *i64, *i64);
            break;
        case ECT_UNSIGNED8:
            u8 = (uint8*) &usdo[0];
            sprintf(hstr, "0x%2.2x %u", *u8, *u8);
            break;
        case ECT_UNSIGNED16:
            u16 = (uint16*) &usdo[0];
            sprintf(hstr, "0x%4.4x %u", *u16, *u16);
            break;
        case ECT_UNSIGNED32:
        case ECT_UNSIGNED24:
            u32 = (uint32*) &usdo[0];
            sprintf(hstr, "0x%8.8x %u", *u32, *u32);
            break;
        case ECT_UNSIGNED64:
            u64 = (uint64*) &usdo[0];
            sprintf(hstr, "0x%16.16" PRIx64 " %" PRIu64, *u64, *u64);
            break;
        case ECT_REAL32:
            sr = (float*) &usdo[0];
            sprintf(hstr, "%f", *sr);
            break;
        case ECT_REAL64:
            dr = (double*) &usdo[0];
            sprintf(hstr, "%f", *dr);
            break;
        case ECT_BIT1:
        case ECT_BIT2:
        case ECT_BIT3:
        case ECT_BIT4:
        case ECT_BIT5:
        case ECT_BIT6:
        case ECT_BIT7:
        case ECT_BIT8:
            u8 = (uint8*) &usdo[0];
            sprintf(hstr, "0x%x", *u8);
            break;
        case ECT_VISIBLE_STRING:
            strcpy(hstr, usdo);
            break;
        case ECT_OCTET_STRING:
            hstr[0] = 0x00;
            for (i = 0 ; i < length ; i++)
            {
                sprintf(es, "0x%2.2x ", usdo[i]);
                strcat( hstr, es);
            }
            break;
        default:
            sprintf(hstr, "Unknown type");
        }
        return hstr;
    }
}



/**
 * @brief EtherCATManager::slaveinfo_PDOassign
 * Read PDO assign (RxPDO and TxPDO)
 */
int EtherCATManager::slaveinfo_PDOassign(uint16 slave, uint16 PDOassign, int mapoffset, int bitoffset)
{
    uint16 idxloop, nidx, subidxloop, rdat, idx, subidx;
    uint8 subcnt;
    int bsize = 0;
    int wkc, rdl; // read data length
    int32 rdat2; // read data
    uint8 bitlen, obj_subidx;
    uint16 obj_idx;
    int abs_offset, abs_bit;

    rdl = sizeof(rdat); // return data length is 2 bytes
    rdat = 0;

    /* read PDO assign subindex 0 ( = number of PDO's) */
    wkc = ec_SDOread(slave, PDOassign, 0x00, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
    // read data is number of PDO's
    rdat = etohs(rdat); // convert to local byte order

    if ((wkc > 0) && (rdat > 0))
    {
        nidx = rdat; // number of index is read data(number of PDO's)
        bsize = 0;
        /* read all PDO's */
        for (idxloop = 1; idxloop <= nidx; idxloop++)
        {
            rdl = sizeof(rdat); // return data length is 2 bytes
            rdat = 0; // clear read data
            /* read PDO assign, PDOassign is TxPDO or RxPDO */
            wkc = ec_SDOread(slave, PDOassign, (uint8)idxloop, FALSE, &rdl, &rdat, EC_TIMEOUTRXM);
            /* result is index of PDO */
            idx = etohl(rdat);
            if (idx > 0)
            {
                rdl = sizeof(subcnt); // return data length is 1 byte
                subcnt = 0;
                /* read number of subindexes of PDO */
                wkc = ec_SDOread(slave,idx, 0x00, FALSE, &rdl, &subcnt, EC_TIMEOUTRXM);
                subidx = subcnt;
                /* for each subindex */
                for (subidxloop = 1; subidxloop <= subidx; subidxloop++)
                {
                    rdl = sizeof(rdat2); // return data length is 4 bytes
                    rdat2 = 0;

                    /* read SDO that is mapped in PDO */
                    wkc = ec_SDOread(slave, idx, (uint8)subidxloop, FALSE, &rdl, &rdat2, EC_TIMEOUTRXM);
                    rdat2 = etohl(rdat2);

                    /* extract bitlength of SDO */
                    bitlen = LO_BYTE(rdat2); // Macro to get low byte(8bit) of a word
                    bsize = bitlen + bsize; // bsize is total bitlength of PDO

                    obj_idx = (uint16)(rdat2 >> 16); // get 17 to 32 bit of rdat2 is index
                    obj_subidx = (uint8)((rdat2 >> 8) & 0x000000ff); // get 9 to 16 bit of rdat2 is subindex
                    /* bitlen = LO_BYTE(rdat2); // low 8bit is bitlen */

                    abs_offset = mapoffset + (bitoffset / 8);
                    abs_bit = bitoffset % 8;

                    ODlist.Slave = slave;
                    ODlist.Index[0] = obj_idx;
                    OElist.Entries = 0; // OElist.Entries is number of entries in list
                    wkc = 0;

                    /* read object entry from dictionary if not a filler (0x0000:0x00) */
                    if(obj_idx || obj_subidx)
                        wkc = ec_readOEsingle(0, obj_subidx, &ODlist, &OElist); // return resulting object entry structure.
                    printf("  [0x%4.4X.%1d] 0x%4.4X:0x%2.2X 0x%2.2X", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);

                    if((wkc > 0) && OElist.Entries)
                    {
                        printf(" %-12s %s\n", dtype2string(OElist.DataType[obj_subidx]), OElist.Name[obj_subidx]);
                    }

                    else
                        printf("\n");
                    bitoffset += bitlen;
                };
            };
        };
    };
    /* return total found bitlength (PDO) */
    return bsize;
}



/**
 * @brief EtherCATManager::slaveinfo_map_sdo
 *
 */
int EtherCATManager::slaveinfo_map_SM(int slave)
{
    int wkc, rdl;
    int retVal = 0;
    uint8 nSM; // number of SM
    uint8 iSM; // iterator
    uint8 tSM;
    int Tsize, outputs_bo, inputs_bo;
    uint8 SMt_bug_add;
    SMt_bug_add = 0;
    outputs_bo = 0;
    inputs_bo = 0;

    printf("PDO mapping according to CoE :\n");
    rdl = sizeof(nSM);
    nSM = 0;

    /* read SyncManager Communication Type object count */
    /*
        ECT_SDO_SMCOMMTYPE expands to 0x1C00, subindex 00 is number of SM channel
        subindex 1-32 is the cooresponding SM type
        0 : Mailbox input
        1 : Mailbox output
        2 : Process data input
        3 : Process date output
    */
    wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, 0x00, FALSE, &rdl, &nSM, EC_TIMEOUTRXM);

    if ((wkc > 0) && (nSM >= 3))
    {
        /* make nSM equal to number of defined SM, because SM named from SM0 */
        nSM--;

        /* limit to maximum number of SM defined, if true the slave can't be configured */
        if (nSM > EC_MAXSM)
            nSM = EC_MAXSM;

        /* iterate for every SM type defined */
        for (iSM = 2 ; iSM < nSM + 1 ; iSM++)
        {
            rdl = sizeof(tSM);
            tSM = 0; // type of SM
            /* read SyncManager Communication Type */
            wkc = ec_SDOread(slave, ECT_SDO_SMCOMMTYPE, iSM + 1, FALSE, &rdl, &tSM, EC_TIMEOUTRXM);

            if (wkc > 0)
            {
                if((iSM == 2) && (tSM == 2)) // SM2 has type 2 == mailbox out, this is a bug in the slave!
                {
                    SMt_bug_add = 1; // try to correct, this works if the types are 0 1 2 3 and should be 1 2 3 4
                    printf("Activated SM type workaround, possible incorrect mapping.\n");
                }
                if(tSM)
                    tSM += SMt_bug_add; // only add if SMt > 0

                if (tSM == 3) // outputs
                {
                    /* read the assign RXPDO */
                    printf("  SM%1d outputs\n     addr b   index: sub bitl data_type    name\n", iSM);
                    Tsize = slaveinfo_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].outputs - (uint8 *)&IOmap[0]), outputs_bo );
                    outputs_bo += Tsize;
                }
                if (tSM == 4) // inputs
                {
                    /* read the assign TXPDO */
                    printf("  SM%1d inputs\n     addr b   index: sub bitl data_type    name\n", iSM);
                    Tsize = slaveinfo_PDOassign(slave, ECT_SDO_PDOASSIGN + iSM, (int)(ec_slave[slave].inputs - (uint8 *)&IOmap[0]), inputs_bo );
                    inputs_bo += Tsize;
                }
            }
        }
    }

    /* found some I/O bits ? */
    if ((outputs_bo > 0) || (inputs_bo > 0))
        retVal = 1;
    return retVal;
}



/**
 * @brief EtherCATManager::slaveinfo_siiPDO
 */
int EtherCATManager::slaveinfo_SII_RTxPDO(uint16 slave, uint8 isRxPDO, int mapoffset, int bitoffset)
{
    // 50 is TxPDO and 51=50+t is RxPDO
    // TxPDO = 0 RxPDO = 1
    if (isRxPDO > 1)
        isRxPDO = 1;

    int i;

    uint16 start_pos, word_read, len_count, entries_num, entries_iterator, Size;
    uint8 eectl;
    uint16 obj_idx;
    uint8 obj_subidx;
    uint8 obj_name;
    uint8 obj_datatype;
    uint8 bitlen;
    int totalsize;

    ec_eepromPDOt eepPDO; // record to store rxPDO and txPDO table from eeprom
    ec_eepromPDOt *PDO;
    int abs_offset, abs_bit;
    char str_name[EC_MAXNAME + 1];

    eectl = ec_slave[slave].eep_pdi; // 0 = eeprom to master, 1 = eeprom to PDI
    // initialize with 0
    Size = 0;
    totalsize = 0;

    PDO = &eepPDO;
    PDO->nPDO = 0;
    PDO->Length = 0;
    PDO->Index[1] = 0;
    for (i = 0 ; i < EC_MAXSM ; i++)
        PDO->SMbitsize[i] = 0;

    /*
     * ECT_SII_PDO expands to 50 (defined in ETG2010_S_D_V1i0i0i2_EtherCATSIISpecification)
     * In Categories Types, TxPDO has value 50, RxPDO has value 51
     * ec_siifind Return:
     * byte address of section at section length entry, if not available then 0
     */
    PDO->Startpos = ec_siifind(slave, ECT_SII_PDO + isRxPDO);
    if (PDO->Startpos > 0)
    {
        start_pos = PDO->Startpos;
        // read Category Word Size(PDO->Length) (word = 16bit = 8 + 8bit)
        word_read = ec_siigetbyte(slave, start_pos++);
        word_read += (ec_siigetbyte(slave, start_pos++) << 8);
        PDO->Length = word_read;

        len_count = 1;
        /* traverse through all PDOs */
        do
        {
            PDO->nPDO++;
            /* read PDO Index
             * For RxPDO: 0x1600 to 17FF,
             * For TxPDO: 0x1A00 to 1BFF
             */
            PDO->Index[PDO->nPDO] = ec_siigetbyte(slave, start_pos++);
            PDO->Index[PDO->nPDO] += (ec_siigetbyte(slave, start_pos++) << 8);

            PDO->BitSize[PDO->nPDO] = 0;
            len_count++;
            /* number of entries in PDO */
            entries_num = ec_siigetbyte(slave, start_pos++);
            PDO->SyncM[PDO->nPDO] = ec_siigetbyte(slave, start_pos++);
            // skip Synchronization(Reference to DC Sync)
            start_pos++;
            // Name of the Object(Index to STRINGS)
            obj_name = ec_siigetbyte(slave, start_pos++);
            // skip Flags
            start_pos += 2;

            len_count += 2;

            if (PDO->SyncM[PDO->nPDO] < EC_MAXSM) /* active and in range SM? */
            {
                str_name[0] = 0;
                if(obj_name)
                    ec_siistring(str_name, slave, obj_name);
                if (isRxPDO)
                    printf("  SM%1d RXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
                else
                    printf("  SM%1d TXPDO 0x%4.4X %s\n", PDO->SyncM[PDO->nPDO], PDO->Index[PDO->nPDO], str_name);
                printf("     addr b   index: sub bitl data_type    name\n");

                /* read all entries defined in PDO */
                for (entries_iterator = 1; entries_iterator <= entries_num; entries_iterator++)
                {
                    len_count += 4;
                    obj_idx = ec_siigetbyte(slave, start_pos++);
                    obj_idx += (ec_siigetbyte(slave, start_pos++) << 8);
                    obj_subidx = ec_siigetbyte(slave, start_pos++);
                    obj_name = ec_siigetbyte(slave, start_pos++);
                    obj_datatype = ec_siigetbyte(slave, start_pos++);
                    bitlen = ec_siigetbyte(slave, start_pos++);
                    abs_offset = mapoffset + (bitoffset / 8);
                    abs_bit = bitoffset % 8;

                    PDO->BitSize[PDO->nPDO] += bitlen;
                    start_pos += 2;

                    str_name[0] = 0;
                    if(obj_name)
                        ec_siistring(str_name, slave, obj_name);

                    printf("  [0x%4.4X.%1d] 0x%4.4X:0x%2.2X 0x%2.2X", abs_offset, abs_bit, obj_idx, obj_subidx, bitlen);
                    printf(" %-12s %s\n", dtype2string(obj_datatype), str_name);
                    bitoffset += bitlen;
                    totalsize += bitlen;
                }
                // SM bitsize is total bitlength of PDO
                PDO->SMbitsize[ PDO->SyncM[PDO->nPDO] ] += PDO->BitSize[PDO->nPDO];
                Size += PDO->BitSize[PDO->nPDO];
                len_count++;
            }
            else /* PDO deactivated because SM is 0xff or > EC_MAXSM */
            {
                len_count += 4 * entries_num;
                start_pos += 8 * entries_num;
                len_count++;
            }
            if (PDO->nPDO >= (EC_MAXEEPDO - 1)) len_count = PDO->Length; /* limit number of PDO entries in buffer */
        }
        while (len_count < PDO->Length);
    }
    if (eectl)
        ec_eeprom2pdi(slave); /* if eeprom control was previously pdi then restore */
    return totalsize;
}



/**
 * @brief EtherCATManager::slaveinfo_map_SII
 * return 1 if found some I/O bits
 */
int EtherCATManager::slaveinfo_map_SII(int slave)
{
    int retVal = 0;
    int Tsize, outputs_bitoff, inputs_bitoff;

    printf("PDO mapping according to SII :\n");

    outputs_bitoff = 0;
    inputs_bitoff = 0;
    /* read the assign RXPDOs */
    Tsize = slaveinfo_SII_RTxPDO(slave, 1, (int)(ec_slave[slave].outputs - (uint8*)&IOmap), outputs_bitoff );
    outputs_bitoff += Tsize;
    /* read the assign TXPDOs */
    Tsize = slaveinfo_SII_RTxPDO(slave, 0, (int)(ec_slave[slave].inputs - (uint8*)&IOmap), inputs_bitoff );
    inputs_bitoff += Tsize;
    /* found some I/O bits ? */
    if ((outputs_bitoff > 0) || (inputs_bitoff > 0))
        retVal = 1;
    return retVal;
}



/**
 * @brief EtherCATManager::slaveinfo_sdo
 *
 */
void EtherCATManager::slaveinfo_sdo(int cnt)
{
    int i, j;

    ODlist.Entries = 0;
    memset(&ODlist, 0, sizeof(ODlist));

    if(ec_readODlist(cnt, &ODlist))
    {
        printf(" CoE Object Description found, %d entries.\n",ODlist.Entries);
        for(i = 0 ; i < ODlist.Entries ; i++)
        {
            ec_readODdescription(i, &ODlist);
            while(EcatError)
                printf("%s", ec_elist2string()); // error list
            printf(" Index: %4.4x Datatype: %4.4x Objectcode: %2.2x Name: %s\n",
                   ODlist.Index[i], ODlist.DataType[i], ODlist.ObjectCode[i], ODlist.Name[i]);

            memset(&OElist, 0, sizeof(OElist));
            ec_readOE(i, &ODlist, &OElist);
            while(EcatError) printf("%s", ec_elist2string());
            for( j = 0 ; j < ODlist.MaxSub[i]+1 ; j++)
            {
                if ((OElist.DataType[j] > 0) && (OElist.BitLength[j] > 0))
                {
                    printf("  Sub: %2.2x Datatype: %4.4x Bitlength: %4.4x Obj.access: %4.4x Name: %s\n",
                           j, OElist.DataType[j], OElist.BitLength[j], OElist.ObjAccess[j], OElist.Name[j]);
                    if ((OElist.ObjAccess[j] & 0x0007))
                    {
                        printf("          Value :%s\n", SDO2string(cnt, ODlist.Index[i], j, OElist.DataType[j]));
                    }
                }
            }
        }
    }
    else
    {
        while(EcatError) printf("%s", ec_elist2string());
    }
}



/**
 * @brief EtherCATManager::slaveinfo
 *
 */
void EtherCATManager::slaveinfo(char *ifname)
{
    int cnt, i, j, nSM;
    uint16 ssigen;
    int expectedWKC;

    printf("Starting slaveinfo\n");

    /* initialise SOEM, bind socket to ifname */
    if (ec_init(ifname))
    {
        printf("ec_init on %s succeeded.\n",ifname);
        /* find and auto-config slaves */
        if ( ec_config(FALSE, &IOmap) > 0 )
        {
            ec_configdc();
            while(EcatError) printf("%s", ec_elist2string());
            printf("%d slaves found and configured.\n",ec_slavecount);
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            printf("Calculated workcounter %d\n", expectedWKC);
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 3);
            if (ec_slave[0].state != EC_STATE_SAFE_OP )
            {
                printf("Not all slaves reached safe operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_SAFE_OP)
                    {
                        printf("Slave %d State=%2x StatusCode=%4x : %s\n",
                               i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }


            ec_readstate();
            for( cnt = 1 ; cnt <= ec_slavecount ; cnt++)
            {
                printf("\nSlave:%d\n Name:%s\n Output size: %dbits\n Input size: %dbits\n State: %d\n Delay: %d[ns]\n Has DC: %d\n",
                       cnt, ec_slave[cnt].name, ec_slave[cnt].Obits, ec_slave[cnt].Ibits,
                       ec_slave[cnt].state, ec_slave[cnt].pdelay, ec_slave[cnt].hasdc);
                if (ec_slave[cnt].hasdc) printf(" DCParentport:%d\n", ec_slave[cnt].parentport);
                printf(" Activeports:%d.%d.%d.%d\n", (ec_slave[cnt].activeports & 0x01) > 0 ,
                       (ec_slave[cnt].activeports & 0x02) > 0 ,
                       (ec_slave[cnt].activeports & 0x04) > 0 ,
                       (ec_slave[cnt].activeports & 0x08) > 0 );
                printf(" Configured address: %4.4x\n", ec_slave[cnt].configadr);
                printf(" Man: %8.8x ID: %8.8x Rev: %8.8x\n", (int)ec_slave[cnt].eep_man, (int)ec_slave[cnt].eep_id, (int)ec_slave[cnt].eep_rev);
                for(nSM = 0 ; nSM < EC_MAXSM ; nSM++)
                {
                    if(ec_slave[cnt].SM[nSM].StartAddr > 0)
                        printf(" SM%1d A:%4.4x L:%4d F:%8.8x Type:%d\n",nSM, ec_slave[cnt].SM[nSM].StartAddr, ec_slave[cnt].SM[nSM].SMlength,
                               (int)ec_slave[cnt].SM[nSM].SMflags, ec_slave[cnt].SMtype[nSM]);
                }
                for(j = 0 ; j < ec_slave[cnt].FMMUunused ; j++)
                {
                    printf(" FMMU%1d Ls:%8.8x Ll:%4d Lsb:%d Leb:%d Ps:%4.4x Psb:%d Ty:%2.2x Act:%2.2x\n", j,
                           (int)ec_slave[cnt].FMMU[j].LogStart, ec_slave[cnt].FMMU[j].LogLength, ec_slave[cnt].FMMU[j].LogStartbit,
                           ec_slave[cnt].FMMU[j].LogEndbit, ec_slave[cnt].FMMU[j].PhysStart, ec_slave[cnt].FMMU[j].PhysStartBit,
                           ec_slave[cnt].FMMU[j].FMMUtype, ec_slave[cnt].FMMU[j].FMMUactive);
                }
                printf(" FMMUfunc 0:%d 1:%d 2:%d 3:%d\n",
                       ec_slave[cnt].FMMU0func, ec_slave[cnt].FMMU1func, ec_slave[cnt].FMMU2func, ec_slave[cnt].FMMU3func);
                printf(" MBX length wr: %d rd: %d MBX protocols : %2.2x\n", ec_slave[cnt].mbx_l, ec_slave[cnt].mbx_rl, ec_slave[cnt].mbx_proto);
                ssigen = ec_siifind(cnt, ECT_SII_GENERAL);
                /* SII general section */
                if (ssigen)
                {
                    ec_slave[cnt].CoEdetails = ec_siigetbyte(cnt, ssigen + 0x07);
                    ec_slave[cnt].FoEdetails = ec_siigetbyte(cnt, ssigen + 0x08);
                    ec_slave[cnt].EoEdetails = ec_siigetbyte(cnt, ssigen + 0x09);
                    ec_slave[cnt].SoEdetails = ec_siigetbyte(cnt, ssigen + 0x0a);
                    if((ec_siigetbyte(cnt, ssigen + 0x0d) & 0x02) > 0)
                    {
                        ec_slave[cnt].blockLRW = 1;
                        ec_slave[0].blockLRW++;
                    }
                    ec_slave[cnt].Ebuscurrent = ec_siigetbyte(cnt, ssigen + 0x0e);
                    ec_slave[cnt].Ebuscurrent += ec_siigetbyte(cnt, ssigen + 0x0f) << 8;
                    ec_slave[0].Ebuscurrent += ec_slave[cnt].Ebuscurrent;
                }
                printf(" CoE details: %2.2x FoE details: %2.2x EoE details: %2.2x SoE details: %2.2x\n",
                       ec_slave[cnt].CoEdetails, ec_slave[cnt].FoEdetails, ec_slave[cnt].EoEdetails, ec_slave[cnt].SoEdetails);
                printf(" Ebus current: %d[mA]\n only LRD/LWR:%d\n",
                       ec_slave[cnt].Ebuscurrent, ec_slave[cnt].blockLRW);
                if ((ec_slave[cnt].mbx_proto & ECT_MBXPROT_COE) && printSDO)
                    si_sdo(cnt);
                if(printMAP)
                {
                    if (ec_slave[cnt].mbx_proto & ECT_MBXPROT_COE)
                        si_map_sdo(cnt);
                    else
                        si_map_sii(cnt);
                }
            }
        }
        else
        {
            printf("No slaves found!\n");
        }
        printf("End slaveinfo, close socket\n");
        /* stop SOEM, close socket */
        ec_close();
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n",ifname);
    }
}

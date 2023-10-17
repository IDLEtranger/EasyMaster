#ifndef SLAVEINFO_H
#define SLAVEINFO_H
/***************/
#include <QVariant>
#include <QMap>
#include <QString>
/***************/

class SlaveInfo
{
    struct SMinfo
    {
        uint16_t SMtype;
        uint16_t SMsize;
        uint16_t SMstart;
        uint16_t SMend;
    };
public:
    SlaveInfo();
private:
    uint16_t _slaveNumber;
    QString _name;

    uint16_t _outputSize_bit;
    uint16_t _inputSize_bit;

    uint16_t _delay_ns;
    bool _hasDC;

    uint16_t _ParentPort;

    QString _activePorts;
    uint16_t _configuredAddress;

    QString _Vender;
    QString _productCode;
    QString _revisionNumber;


    QList<QString> _smDetails;
    QList<QString> _fmmuDetails;
    QList<QString> _mbxDetails;
    QList<QString> _coeDetails;
    int _ebusCurrent;
    bool _onlyLRD_LWR;
    QList<QString> _pdoMappingOutputs;
    QList<QString> _pdoMappingInputs;

};

#endif // SLAVEINFO_H

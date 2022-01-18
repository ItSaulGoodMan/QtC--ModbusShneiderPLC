#ifndef TEST_H
#define TEST_H
#include "QModbusTcpClient.h"
#include "QModbusDataUnit.h"

#include <QObject>

class test : public QObject
{
    Q_OBJECT
    QModbusTcpClient *My_client;
public:
    explicit test(QObject *parent = nullptr);

    bool Connect_to_modbus(QString IP_address, int Port);
signals:

};

#endif // TEST_H

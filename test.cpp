#include "test.h"
#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>
#include <qdebug.h>

test::test(QObject *parent)
    : QObject{parent}
{
    Connect_to_modbus("192.168.137.11", 502);
    My_client = new QModbusTcpClient() ;
}

bool test::Connect_to_modbus(QString IP_address , int Port)
{
    if ( ! My_client ) {
        return false ;
    }

    if  ( My_client->state()!= QModbusDevice::ConnectedState )  {        //determines whether the current connection state to the OFF state

        //Modbus TCP connection configuration parameters IP + port number Port modbus protocol for 502
        My_client -> setConnectionParameter ( QModbusDevice::NetworkAddressParameter , IP_address ) ;
        My_client -> setConnectionParameter( QModbusDevice::NetworkPortParameter , Port ) ;

        if  ( ! My_client -> connectDevice ( ) )  {
            qDebug ( ) <<  "Failed to connect to modbus device" ;
            return false ;
        }
        else  {
            qDebug ( ) <<  "Successfully connected to modbs device" ;
            return true ;
        }
    }

    else  {
        My_client -> disconnectDevice ( ) ;
        return false ;
    }

}


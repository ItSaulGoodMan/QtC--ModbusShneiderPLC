#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QMainWindow>
#include "istorijagresaka.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QModbusTcpClient *My_client;

signals:
void statechange_on();
void statechange_off();
//void my_readC_finished();

private slots:
void onStateChanged();
void myfunction();
void myfunction1();
void myfunction2();


void on_pushButton_pressed();

void on_pushButton_released();

void on_pushButton_2_pressed();

void on_pushButton_2_released();

void on_pushButton_3_pressed();

void on_pushButton_3_released();

void on_pushButton_4_pressed();

void on_pushButton_4_released();

void on_pushButton_5_clicked();


void on_pushButton_6_clicked();


void on_pushButton_7_clicked();

void on_pushButton_8_clicked();

public:
    bool Connect_to_modbus(QString IP_address, int Port);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ReadReady_HoldingCustom();
public slots:
    bool read_modbus_tcp_Coils(int start_add, quint16 numbers, int Server_ID);
    void ReadReady_Coils();
    void Write_modbus_tcp_Coils(bool ok1, uint i2, QString str1, int star_add, int Number);
    void Write_modbus_tcp_Holding(quint16 value, uint pos, QString str1, int star_add, int Number);
    void ReadReady_CoilsCustom();
    bool read_modbus_tcp_CoilsCustom(uint pos, int start_add, quint16 numbers, int Server_ID);
    void ReadReady_CoilsCustom1();
    bool read_modbus_tcp_CoilsCustom1(uint pos, int start_add, quint16 numbers, int Server_ID);
    void Write_modbus_tcp_HoldingCustom(bool value, uint word, int pos, QString str1, int star_add, int Number);
    bool read_modbus_tcp_HoldingCustom(uint pos, int start_add, quint16 numbers, int Server_ID);
    bool read_modbus_tcp_Holding(int start_add, quint16 numbers, int Server_ID);
    void ReadReady_Holding();
     bool read_modbus_tcp_DiscreteCustom(uint pos, int start_add , quint16 numbers , int Server_ID );
     void ReadReady_DiscreteCustom();
     void Write_modbus_tcp_Discrete(bool ok1, uint i2, QString str1 , int star_add , int Number);
     void resizeEvent(QResizeEvent *evt);
private:
    Ui::MainWindow *ui;
    //istorija *istorija;
};
#endif // MAINWINDOW_H

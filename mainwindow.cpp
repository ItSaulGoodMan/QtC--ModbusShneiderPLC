#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QModbusTcpClient>
//#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>
#include <QDebug>
#include<QtDebug>
#include<QTime>
#include <QBitArray>
#include <QDataStream>
#include <QTimer>
#include <QtSql>
#include <QDateTime>
#include"istorijagresaka.h"
#include "istorijagresaka.cpp"
#include <QSound>
#include <QDir>
#include<QCoreApplication>
#include<istorijagresaka.h>
#include <QWindow>
using namespace std;
int konekcija;
//***************WRITING UNITS**************
QModbusDataUnit writeUnit(QModbusDataUnit::Coils , 0, 70 ) ;
QModbusDataUnit writeUnit1(QModbusDataUnit::HoldingRegisters , 0, 99 ) ;
QModbusDataUnit writeUnit2(QModbusDataUnit::DiscreteInputs , 0, 10 ) ;

//******************************************

//***************GLOBAL VARIABLES***********
uint readCustomPos;
uint16_t readHolding;
uint16_t readHoldingWord;
uint16_t ReadValue;
uint16_t CoilStream[70];
uint16_t CoilStreamPrevious[70];

QString greska[70];

QString putanja1 = QCoreApplication::applicationDirPath();
QString baza1 = putanja1 + ".baza.db";






//******************************************

//********FUNCTIONS*************************
//******************************************

//********FOR READING***********************
//******************************************


//********COILS*****************************
bool read_modbus_tcp_Coils(int start_add, quint16 numbers, int Server_ID); // read_modbus_tcp_Coils(0, 30 , 1); --READING ALL 30 COIL VALUES

bool read_modbus_tcp_CoilsCustom1(uint pos, int start_add, quint16 numbers, int Server_ID); // read_modbus_tcp_CoilsCustom1(22, 0 ,30 , 0 ); --
//READING COIL FROM POSITION 22
//******************************************

//********HOLDING REGISTERS******************
bool read_modbus_tcp_Holding(int start_add, quint16 numbers, int Server_ID);// read_modbus_tcp_Holding(0, 30 , 1); --READING ALL 30 HOLDING REGISTERS
bool read_modbus_tcp_HoldingCustom(uint pos, int start_add, quint16 numbers, int Server_ID);// read_modbus_tcp_HoldingCustom(4, 0 ,30 , 0 );
//READING HOLDING REGISTER NUMBER 4
//******************************************

//********FOR WRITING***********************
//******************************************

//********COILS*****************************
void Write_modbus_tcp_Coils(bool ok1, uint i2, QString str1, int star_add, int Number);// Write_modbus_tcp_Coils(1, 2, "1", 0,  10); --
//--WRITING VALUE 1 TO COIL NUMBER 2
//******************************************

//********HOLDING REGISTERS******************
void Write_modbus_tcp_Holding(quint16 value, uint pos, QString str1, int star_add, int Number);// Write_modbus_tcp_Holding(63, 4, "1", 0,  10);
//-- WRITE VALUE 63 INTO HOLDING REGISTER NUMBER 4
void Write_modbus_tcp_HoldingCustom(bool value, uint word, int pos, QString str1, int star_add, int Number);//Write_modbus_tcp_HoldingCustom(1,5,1, "1" , 0 ,30);
//-WRITE BIT VALUE 1, INTO HOLDING REGISTER NUMBER 5, ON POSITION 1




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui -> label_27 -> setText("Uredjaj nije konektovan");

    this->setWindowTitle("PLC MANAGER");


    QPixmap bkgnd(":/resursi/resursi/pozadina2.jpg");
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);



    greska[0] = "Smetnje zatezanja trake";
    greska[1] = "Preteze uze desno";
    greska[2] = "Preteze uze lijevo";
    greska[3] = "Bocno skretanje rep";
    greska[4] = "Bocno skretanje pog. st.";
    greska[5] = "Preopt. Bimetal m1";
    greska[6] = "Preopt. Bimetal m2";
    greska[7] = "Preg. Os m1";
    greska[8] = "Pregar. os m2";
    greska[9] = "Grij.Rez.Red m1";
    greska[10] = "Grij. Nam m1";
    greska[11] = "Grij. lez. m2";
    greska[12] = "Grij. Nam m2";
    greska[13] = "sf6 gas m1";
    greska[14] = "SF6 gas m2";
    greska[15] = "Zapunj. kosa";
    greska[16] = "Not pogon";
    greska[17] = "Not kljuc";
    greska[18] = "Proklizavanje";
    greska[19] = "Krajnji vrata M1";
    greska[20] = "Krajnji vrata M2";
    greska[21] = "Greska kocnice sig";













    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(baza1);

    if(!mydb.open())

        ui -> label_2 -> setText("Database is not connected");

else
        ui -> label_2 -> setText("Database is  connected");

    QString query = "CREATE TABLE testtable("
            "timestamp datetime, greska text,"
            "brGreske integer);";

    QSqlQuery qry;
    if(!qry.exec(query)){
        qDebug()<<"error creating table";
    }
    QString query2 = "CREATE TABLE unesi("
            "coil integer,"
            "polje integer);";
    QSqlQuery qry2;
    if(!qry.exec(query2)){
        qDebug()<<"error creating table";
    }




    My_client = new QModbusTcpClient() ;
    Connect_to_modbus("192.168.137.11", 502);
    connect(My_client, &QModbusClient::stateChanged,this, &MainWindow::onStateChanged);
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);




read_modbus_tcp_Coils(0, 30 , 1);
read_modbus_tcp_Holding(0, 30 , 1);

QTimer *timer = new QTimer(this);
connect(timer, SIGNAL(timeout()), this, SLOT(myfunction()));
 timer -> start(300);
 QTimer *timer1 = new QTimer(this);
 connect(timer1, SIGNAL(timeout()), this, SLOT(myfunction1()));
  timer1 -> start(3000);

QTimer *timer2 = new QTimer(this);
connect(timer2, SIGNAL(timeout()), this, SLOT(myfunction2()));
timer2 -> start(2000);
    //ReadReady_Coils();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent *evt)
{
    QDir directory("");
    QString path = directory.filePath("pozadina.jpg");
    qDebug()<<path;
    QPixmap bkgnd(":/resursi/resursi/pozadina2.jpg");//Load pic
bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);//set scale of pic to match the window
QPalette p = palette(); //copy current, not create new
p.setBrush(QPalette::Background, bkgnd);//set the pic to the background
setPalette(p);//show the background pic

QMainWindow::resizeEvent(evt); //call base implementation
}






bool MainWindow::Connect_to_modbus(QString IP_address , int Port)
{
    if ( ! My_client ) {
        return false ;
    }

    if  ( My_client->state()!= QModbusDevice::ConnectedState )  {        //determines whether the current connection state to the OFF state

        //Modbus TCP connection configuration parameters IP + port number Port modbus protocol for 502
        My_client -> setConnectionParameter ( QModbusDevice::NetworkAddressParameter , IP_address ) ;
        My_client -> setConnectionParameter( QModbusDevice::NetworkPortParameter , Port ) ;

        if  ( ! My_client -> connectDevice () )  {
           // qDebug ( ) <<  "Failed to connect to modbus device";
            return false ;
        }
        else  {
            //qDebug ( ) <<  "Successfully connected to modbs device" ;
            return true ;
        }
    }

    else  {
        My_client -> disconnectDevice ( ) ;
        return false ;
    }

}

void MainWindow::onStateChanged ( )                //function of a channel connection state change
{
   if ( My_client -> state ( )  == QModbusDevice::ConnectedState )
   {
      emit statechange_on() ;
       //qDebug()<<"Konektovan";
       //konekcija ++;
       //QString s = QString::number(konekcija);
       //ui -> label_27 -> setText("Uredjaj konektovan");
   }

   else  {
      emit statechange_off() ;
       //qDebug()<<"Nije Konektovan";
      // QString s = QString::number(konekcija);
       //ui -> label_27 -> setText("Uredjaj nije konektovan");
       //konekcija --;

   }
}

void MainWindow::myfunction1()
{
    int j = 0;
    for(int i = 0; i<23; i++){

        if(CoilStream[i]==0)
        {
            j++;
          //  break;
        }
}
    if(j==0){
    ui -> frame -> setStyleSheet("background-image: url(:/resursi/good.jpg);");

    }
    else {
        ui -> frame -> setStyleSheet("background-image: url(:/resursi/images.png);");
        QSound::play(":/resursi/resursi/beep.wav");

}

}

void MainWindow::myfunction2()
{
    bool provjera;
    QSqlQuery sqlQuery;
    sqlQuery.prepare("SELECT \"coil\", \"polje\"FROM unesi;");
    sqlQuery.exec();
    if (sqlQuery.isActive()){
        if (sqlQuery.isSelect()){
            if (sqlQuery.last()){
                int vrijednost=sqlQuery.value(0).toInt();
                int polje=sqlQuery.value(1).toInt();

                  if(vrijednost==0){
                      provjera = 0;
                  }
                  else{
                      provjera = 1;
                  }
                 Write_modbus_tcp_Coils(provjera, polje, "1", 0,  10);
                qDebug()<<vrijednost<<polje;

            }
        }
    }
}







void MainWindow::myfunction()
{

    QSqlQuery qry1;
    QDateTime timestamp = QDateTime::currentDateTime();


  read_modbus_tcp_Coils(0 ,30 , 0 );

  if(My_client -> state ( )  == QModbusDevice::ConnectedState){
      ui -> pushButton_6 -> setText("Diskonektuj");

   ui -> label_27 -> setText("Uredjaj konektovan");

  for(int i = 0; i<23; i++){
      if(CoilStream[i]==0 && CoilStream[i]!=CoilStreamPrevious[i])
      {

          qry1.prepare("INSERT INTO testtable("
                     " timestamp, greska,"
                      "brGreske)"
                      "VALUES(?,?,?);");
          qry1.addBindValue(timestamp);
          qry1.addBindValue(greska[i]);
          qry1.addBindValue(i);


          if(!qry1.exec()){

          qDebug()<<"Error adding values";
          }

      }

  }




  QString converted = QString::number(CoilStream[22]);
  QLabel* label_3 = new QLabel;
  if(CoilStream[0] == 0){
  ui -> label_3->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_3->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_4 = new QLabel;
  if(CoilStream[1] == 0){
  ui -> label_4->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_4->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_5 = new QLabel;
  if(CoilStream[2] == 0){
  ui -> label_5->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_5->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_6 = new QLabel;
  if(CoilStream[3] == 0){
  ui -> label_6->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_6->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_7 = new QLabel;
  if(CoilStream[4] == 0){
  ui -> label_7->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_7->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_8 = new QLabel;
  if(CoilStream[5] == 0){
  ui -> label_8->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_8->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_9 = new QLabel;
  if(CoilStream[6] == 0){
  ui -> label_9->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_9->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_10 = new QLabel;
  if(CoilStream[7] == 0){
  ui -> label_10->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_10->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_11 = new QLabel;
  if(CoilStream[8] == 0){
  ui -> label_11->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_11->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_12 = new QLabel;
  if(CoilStream[9] == 0){
  ui -> label_12->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_12->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_13 = new QLabel;
  if(CoilStream[10] == 0){
  ui -> label_13->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_13->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_14 = new QLabel;
  if(CoilStream[11] == 0){
  ui -> label_14->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_14->setStyleSheet("QLabel { background-color : green; color : black; }");
  }


  QLabel* label_15 = new QLabel;
  if(CoilStream[12] == 0){
  ui -> label_15->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_15->setStyleSheet("QLabel { background-color : green; color : black; }");
  }


  QLabel* label_16 = new QLabel;
  if(CoilStream[13] == 0){
  ui -> label_16->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_16->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_17 = new QLabel;
  if(CoilStream[14] == 0){
  ui -> label_17->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_17->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_18 = new QLabel;
  if(CoilStream[15] == 0){
  ui -> label_18->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_18->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_19 = new QLabel;
  if(CoilStream[16] == 0){
  ui -> label_19->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_19->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_20 = new QLabel;
  if(CoilStream[17] == 0){
  ui -> label_20->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_20->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_21 = new QLabel;
  if(CoilStream[18] == 0){
  ui -> label_21->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_21->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_22 = new QLabel;
  if(CoilStream[19] == 0){
  ui -> label_22->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_22->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_23 = new QLabel;
  if(CoilStream[20] == 0){
  ui -> label_23->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_23->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_24 = new QLabel;
  if(CoilStream[21] == 0){
  ui -> label_24->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_24->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_25 = new QLabel;
  if(CoilStream[22] == 0){
  ui -> label_25->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_25->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  QLabel* label_26 = new QLabel;
  if(CoilStream[23] == 0){
  ui -> label_26->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else {
      ui -> label_26->setStyleSheet("QLabel { background-color : green; color : black; }");
  }

  read_modbus_tcp_HoldingCustom(3, 0 ,30 , 0 );
}
  else {
      ui -> label_27 -> setText("Uredjaj nije konektovan");
      ui -> pushButton_6 -> setText("Konektuj");
  }

  for(int i = 0; i<23; i++)
  {
      CoilStreamPrevious[i] = CoilStream[i];
  }
}

bool MainWindow::read_modbus_tcp_Coils(int start_add , quint16 numbers , int Server_ID )
{
     if( !My_client -> state() == QModbusDevice::ConnectedState ) {
        qDebug()<<"Nisi ni konektovan a citao bi e moj ti";
         return false ;

    }

    QModbusDataUnit ReadUnit(QModbusDataUnit::Coils , start_add , numbers ) ;
    qDebug ( ) <<  "Configuration ReadUnit complete" ;
     if(auto  *reply = My_client -> sendReadRequest ( ReadUnit,Server_ID )){ //    "ready signal and slot connection" ;
           //QObject::connect(reply , &QModbusReply::finished, Server_ID);
           connect(reply, &QModbusReply::finished, this, &MainWindow::ReadReady_Coils);

     {
        if( !reply -> isFinished() )
        {   qDebug ( ) << this , &MainWindow::ReadReady_Coils;
           qDebug ( ) << "function reads the entry slot" ;
           return true ;
         }
         else
        {
            qDebug() <<  "delete reply in advance" ;
            delete reply ;
            return false ;
        }

     }


}



}

void MainWindow::ReadReady_Coils()
{
    qDebug() <<  "started slot function" ;
    QModbusReply * reply = qobject_cast < QModbusReply * > (sender());

         if  (!reply) {
             qDebug()<<QModbusDevice::NoError<<  "Quit early" ;
             return  ;
         }
       if  ( reply -> error() == QModbusDevice::NoError) {
           qDebug ( ) <<  "receive data" ;
           const QModbusDataUnit Unit = reply -> result();


           for ( uint16_t i = 0 ; i < Unit.valueCount() ;   i++ )
           {
               /*
                QByteArray AllData =unit.values();//Read all at once
               */

               uint16_t res = Unit.value(i) ;
               CoilStream[i] = res;
               uint16_t length = Unit.valueCount();
             //  qDebug()<<res;
               writeUnit.setValue (i, res) ;

              // uint16_t Coils_Bufer[length];//Read one by one
               //Coils_Bufer[i] = res ;
               //After reading, store the data Coils_Bufer[i] Customized array to store data

           }

       }
       else
       {
       }

       reply -> deleteLater() ;  //delete the reply
}

bool MainWindow::read_modbus_tcp_Holding(int start_add , quint16 numbers , int Server_ID )
{
     if( !My_client -> state() == QModbusDevice::ConnectedState ) {
        qDebug()<<"Nisi ni konektovan a citao bi e moj ti";
         return false ;

    }

    QModbusDataUnit ReadUnit(QModbusDataUnit::HoldingRegisters , start_add , numbers ) ;
    qDebug ( ) <<  "Configuration ReadUnit complete" ;
     if(auto  *reply = My_client -> sendReadRequest ( ReadUnit,Server_ID )){ //    "ready signal and slot connection" ;
           //QObject::connect(reply , &QModbusReply::finished, Server_ID);
           connect(reply, &QModbusReply::finished, this, &MainWindow::ReadReady_Holding);

     {
        if( !reply -> isFinished() )
        {   qDebug ( ) << this , &MainWindow::ReadReady_Coils;
           qDebug ( ) << "function reads the entry slot" ;
           return true ;
         }
         else
        {
            qDebug() <<  "delete reply in advance" ;
            delete reply ;
            return false ;
        }

     }

}
}

void MainWindow::ReadReady_Holding()
{
    qDebug() <<  "started slot function" ;
    QModbusReply * reply = qobject_cast < QModbusReply * > (sender());

         if  (!reply) {
             qDebug()<<QModbusDevice::NoError<<  "Quit early" ;
             return  ;
         }
       if  ( reply -> error() == QModbusDevice::NoError) {
           qDebug ( ) <<  "receive data" ;
           const QModbusDataUnit Unit = reply -> result();


           for ( uint16_t i = 0 ; i < Unit.valueCount() ;   i++ )
           {
               /*
                QByteArray AllData =unit.values();//Read all at once
               */

               uint16_t res = Unit.value(i) ;
               uint16_t length = Unit.valueCount();
               qDebug()<<res;
               writeUnit1.setValue (i, res) ;
              // uint16_t Coils_Bufer[length];//Read one by one
               //Coils_Bufer[i] = res ;
               //After reading, store the data Coils_Bufer[i] Customized array to store data

           }

       }
       else
       {
       }

       reply -> deleteLater() ;  //delete the reply
}









bool MainWindow::read_modbus_tcp_CoilsCustom(uint pos, int start_add , quint16 numbers , int Server_ID )
{
     if( !My_client -> state() == QModbusDevice::ConnectedState ) {
        qDebug()<<"Nisi ni konektovan a citao bi e moj ti";
         return false ;

    }

    QModbusDataUnit ReadUnit1(QModbusDataUnit::Coils , start_add , numbers ) ;
    qDebug() <<  "Configuration ReadUnit complete" ;
     if(auto  *reply = My_client -> sendReadRequest (ReadUnit1,Server_ID)){ //    "ready signal and slot connection" ;
           //QObject::connect(reply , &QModbusReply::finished, Server_ID);
           connect(reply, &QModbusReply::finished, this, &MainWindow::ReadReady_CoilsCustom);

     {
        if( !reply -> isFinished() )
        {   qDebug ( ) << this , &MainWindow::ReadReady_CoilsCustom;
           qDebug ( ) << "function reads the entry slot" ;
           return true ;
         }
         else
        {
            qDebug() <<  "delete reply in advance" ;
            delete reply ;
            return false ;
        }

     }

}
}











void MainWindow::ReadReady_CoilsCustom()
{
    qDebug() <<  "started slot function" ;
    QModbusReply * reply = qobject_cast < QModbusReply * > (sender());

         if  (!reply) {
             qDebug()<<QModbusDevice::NoError<<  "Quit early" ;
             return  ;
         }
       if  ( reply -> error() == QModbusDevice::NoError) {
           qDebug () <<  "receive data" ;
           const QModbusDataUnit Unit = reply -> result();



               /*
                QByteArray AllData =unit.values();//Read all at once
               */

               uint16_t res = Unit.value(1) ;
               qDebug()<<res;
               ReadValue = res;
              // uint16_t Coils_Bufer[length];//Read one by one
               //Coils_Bufer[i] = res ;
               //After reading, store the data Coils_Bufer[i] Customized array to store data

           }


       else
       {
       }

       reply -> deleteLater() ;  //delete the reply
}

bool MainWindow::read_modbus_tcp_CoilsCustom1(uint pos, int start_add , quint16 numbers , int Server_ID )
{
    readCustomPos = pos;
     if( !My_client -> state() == QModbusDevice::ConnectedState ) {
        qDebug()<<"Nisi ni konektovan a citao bi e moj ti";
         return false ;

    }

    QModbusDataUnit ReadUnit2(QModbusDataUnit::Coils , start_add , numbers ) ;
    qDebug() <<  "Configuration ReadUnit complete" ;
     if(auto  *reply = My_client -> sendReadRequest (ReadUnit2,Server_ID)){ //    "ready signal and slot connection" ;
           //QObject::connect(reply , &QModbusReply::finished, Server_ID);
           connect(reply, &QModbusReply::finished, this, &MainWindow::ReadReady_CoilsCustom1);

     {
        if( !reply -> isFinished() )
        {   qDebug ( ) << this , &MainWindow::ReadReady_CoilsCustom1;
           qDebug ( ) << "function reads the entry slot" ;
           return true ;
         }
         else
        {
            qDebug() <<  "delete reply in advance" ;
            delete reply ;
            return false ;
        }

     }

}
}








void MainWindow::ReadReady_CoilsCustom1()
{
    uint pos = 24;
    qDebug() <<  "started slot function" ;
    QModbusReply * reply = qobject_cast < QModbusReply * > (sender());

         if  (!reply) {
             qDebug()<<QModbusDevice::NoError<<  "Quit early" ;
             return  ;
         }
       if  ( reply -> error() == QModbusDevice::NoError) {
           qDebug () <<  "receive data" ;
           const QModbusDataUnit Unit = reply -> result();



               /*
                QByteArray AllData =unit.values();//Read all at once
               */

               uint16_t res = Unit.value(readCustomPos) ;
               qDebug()<<res;
               ReadValue = res;
              QString converted = QString::number(res);
             // ui ->label_3 -> setText(converted);
              // uint16_t Coils_Bufer[length];//Read one by one
               //Coils_Bufer[i] = res ;
               //After reading, store the data Coils_Bufer[i] Customized array to store data

           }


       else
       {
       }

       reply -> deleteLater() ;  //delete the reply
}

bool MainWindow::read_modbus_tcp_DiscreteCustom(uint pos, int start_add , quint16 numbers , int Server_ID )
{
    readCustomPos = pos;
     if( !My_client -> state() == QModbusDevice::ConnectedState ) {
        qDebug()<<"Nisi ni konektovan a citao bi e moj ti";
         return false ;

    }

    QModbusDataUnit ReadUnit2(QModbusDataUnit::DiscreteInputs , start_add , numbers ) ;
    qDebug() <<  "Configuration ReadUnit complete" ;
     if(auto  *reply = My_client -> sendReadRequest (ReadUnit2,Server_ID)){ //    "ready signal and slot connection" ;
           //QObject::connect(reply , &QModbusReply::finished, Server_ID);
           connect(reply, &QModbusReply::finished, this, &MainWindow::ReadReady_DiscreteCustom);

     {
        if( !reply -> isFinished() )
        {   qDebug ( ) << this , &MainWindow::ReadReady_DiscreteCustom;
           qDebug ( ) << "function reads the entry slot" ;
           return true ;
         }
         else
        {
            qDebug() <<  "delete reply in advance" ;
            delete reply ;
            return false ;
        }

     }

}
}








void MainWindow::ReadReady_DiscreteCustom()
{
    uint pos = 24;
    qDebug() <<  "started slot function" ;
    QModbusReply * reply = qobject_cast < QModbusReply * > (sender());

         if  (!reply) {
             qDebug()<<QModbusDevice::NoError<<  "Quit early" ;
             return  ;
         }
       if  ( reply -> error() == QModbusDevice::NoError) {
           qDebug () <<  "receive data" ;
           const QModbusDataUnit Unit = reply -> result();



               /*
                QByteArray AllData =unit.values();//Read all at once
               */

               uint16_t res = Unit.value(readCustomPos) ;
               qDebug()<<res;
               ReadValue = res;
              QString converted = QString::number(res);
             // ui ->label_3 -> setText(converted);
              // uint16_t Coils_Bufer[length];//Read one by one
               //Coils_Bufer[i] = res ;
               //After reading, store the data Coils_Bufer[i] Customized array to store data

           }


       else
       {
       }

       reply -> deleteLater() ;  //delete the reply
}







void MainWindow::Write_modbus_tcp_Coils(bool ok1, uint i2, QString str1 , int star_add , int Number)
{

    quint16 number1 = static_cast < quint16 > (Number) ;  //data type conversion//C ++ in

    uint i1 = i2;
    bool ok = ok1 ;
       writeUnit.setValue (i1, ok) ; //Set sending data


    if  ( auto  * reply = My_client -> sendWriteRequest ( writeUnit ,  1 ) )  { //ui->spinBox_SerAddress->value() is the server address sendWriteRequest is writing data to the server
            if  ( ! reply -> isFinished ( ) )  {    //The Reply When Returns to true Reply has WAS Finished or aborted.
               connect(reply, &QModbusReply::finished, this, [this, reply]() {
                    if  (reply->error()== QModbusDevice::ProtocolError )  {
                        qDebug()<< (tr("Write response error: %1 (Mobus exception: 0x%2)")
                                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                    5000);
                    }   else  if  (reply -> error() != QModbusDevice::NoError)  {
                        qDebug() <<  ( tr ( "the Write Response error:%. 1 (code: 0x% 2)" ) .
                            arg (reply -> errorString ()).arg(reply -> error ( ) ,  - 1 ,  16 ) ,  5000 ) ;
                    }
                    reply -> deleteLater();
               });
            }
              else  {
                //broadcast replies return immediately
                reply -> deleteLater ( ) ;
            }
        }  else  {
            qDebug ( )  <<  ( tr ( "Write error: " )  + My_client -> errorString ( ) ,  5000 ) ;
        }

}

void MainWindow::Write_modbus_tcp_Discrete(bool ok1, uint i2, QString str1 , int star_add , int Number)
{

    quint16 number1 = static_cast < quint16 > (Number) ;  //data type conversion//C ++ in

    uint i1 = i2;
    bool ok = ok1 ;
       writeUnit2.setValue (i1, ok) ; //Set sending data


    if  ( auto  * reply = My_client -> sendWriteRequest ( writeUnit2 ,  1 ) )  { //ui->spinBox_SerAddress->value() is the server address sendWriteRequest is writing data to the server
            if  ( ! reply -> isFinished ( ) )  {    //The Reply When Returns to true Reply has WAS Finished or aborted.
               connect(reply, &QModbusReply::finished, this, [this, reply]() {
                    if  (reply->error()== QModbusDevice::ProtocolError )  {
                        qDebug()<< (tr("Write response error: %1 (Mobus exception: 0x%2)")
                                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                    5000);
                    }   else  if  (reply -> error() != QModbusDevice::NoError)  {
                        qDebug() <<  ( tr ( "the Write Response error:%. 1 (code: 0x% 2)" ) .
                            arg (reply -> errorString ()).arg(reply -> error ( ) ,  - 1 ,  16 ) ,  5000 ) ;
                    }
                    reply -> deleteLater();
               });
            }
              else  {
                //broadcast replies return immediately
                reply -> deleteLater ( ) ;
            }
        }  else  {
            qDebug ( )  <<  ( tr ( "Write error: " )  + My_client -> errorString ( ) ,  5000 ) ;
        }

}

















void MainWindow::Write_modbus_tcp_Holding(quint16 value, uint pos, QString str1 , int star_add , int Number)
{


    quint16 number1 = static_cast < quint16 > (Number) ;  //data type conversion//C ++ in

    uint i1 = pos;
  //  bool ok = ok1 ;
    quint16 heksa = value;

    writeUnit1.setValue (pos, heksa) ; //Set sending data
     quint16 a = 6;
    QByteArray q_b;
    q_b.setNum(a);
    qDebug()<<q_b;

    bool dosta = a&1;
    for(int i = 0; i <16; i++){
        bool d = a&i;
        qDebug()<<d;
    }
    int n = a, p = 2, b = 0;
    int mask = 1 << p;
    int modifajd = (n & ~mask) | (b << p);

    qDebug()<<modifajd;

    if  ( auto  * reply = My_client -> sendWriteRequest ( writeUnit1 ,  1 ) )  { //ui->spinBox_SerAddress->value() is the server address sendWriteRequest is writing data to the server
            if  ( ! reply -> isFinished ( ) )  {    //The Reply When Returns to true Reply has WAS Finished or aborted.
               connect(reply, &QModbusReply::finished, this, [this, reply]() {
                    if  (reply->error()== QModbusDevice::ProtocolError )  {
                        qDebug()<< (tr("Write response error: %1 (Mobus exception: 0x%2)")
                                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                    5000);
                    }   else  if  (reply -> error() != QModbusDevice::NoError)  {
                        qDebug() <<  ( tr ( "the Write Response error:%. 1 (code: 0x% 2)" ) .
                            arg (reply -> errorString ()).arg(reply -> error ( ) ,  - 1 ,  16 ) ,  5000 ) ;
                    }
                    reply -> deleteLater();
               });
            }
              else  {
                //broadcast replies return immediately
                reply -> deleteLater ( ) ;
            }
        }  else  {
            qDebug ( )  <<  ( tr ( "Write error: " )  + My_client -> errorString ( ) ,  5000 ) ;
        }

}



void MainWindow::Write_modbus_tcp_HoldingCustom(bool value, uint word,int pos, QString str1 , int star_add , int Number)
{
    read_modbus_tcp_HoldingCustom(word, 0 ,30 , 0 );


//    QModbusDataUnit writeUnit1(QModbusDataUnit::HoldingRegisters , 0, 33 ) ;

    quint16 number1 = static_cast < quint16 > (Number) ;  //data type conversion//C ++ in

    uint i1 = pos;
  //  bool ok = ok1 ;
    quint16 heksa = readHolding;
    int n = readHolding, p = pos, b = value;
    int mask = 1 << p;
    int modifajd = (n & ~mask) | (b << p);

    writeUnit1.setValue (word, modifajd) ; //Set sending data
     quint16 a = 6;
    QByteArray q_b;
    q_b.setNum(a);
    qDebug()<<q_b;

    bool dosta = a&1;
    for(int i = 0; i <16; i++){
        bool d = a&i;
        qDebug()<<d;
    }

    if  ( auto  * reply = My_client -> sendWriteRequest ( writeUnit1 ,  1 ) )  { //ui->spinBox_SerAddress->value() is the server address sendWriteRequest is writing data to the server
            if  ( ! reply -> isFinished ( ) )  {    //The Reply When Returns to true Reply has WAS Finished or aborted.
               connect(reply, &QModbusReply::finished, this, [this, reply]() {
                    if  (reply->error()== QModbusDevice::ProtocolError )  {
                        qDebug()<< (tr("Write response error: %1 (Mobus exception: 0x%2)")
                                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                    5000);
                    }   else  if  (reply -> error() != QModbusDevice::NoError)  {
                        qDebug() <<  ( tr ( "the Write Response error:%. 1 (code: 0x% 2)" ) .
                            arg (reply -> errorString ()).arg(reply -> error ( ) ,  - 1 ,  16 ) ,  5000 ) ;
                    }
                    reply -> deleteLater();
               });
            }
              else  {
                //broadcast replies return immediately
                reply -> deleteLater ( ) ;
            }
        }  else  {
            qDebug ( )  <<  ( tr ( "Write error: " )  + My_client -> errorString ( ) ,  5000 ) ;
        }

}


bool MainWindow::read_modbus_tcp_HoldingCustom(uint pos, int start_add , quint16 numbers , int Server_ID )
{
    readCustomPos = pos;
     if( !My_client -> state() == QModbusDevice::ConnectedState ) {
        qDebug()<<"Nisi ni konektovan a citao bi e moj ti";
         return false ;

    }

    QModbusDataUnit ReadUnit2(QModbusDataUnit::HoldingRegisters , start_add , numbers ) ;
    qDebug() <<  "Configuration ReadUnit complete" ;
     if(auto  *reply = My_client -> sendReadRequest (ReadUnit2,Server_ID)){ //    "ready signal and slot connection" ;
           //QObject::connect(reply , &QModbusReply::finished, Server_ID);
           connect(reply, &QModbusReply::finished, this, &MainWindow::ReadReady_HoldingCustom);

     {
        if( !reply -> isFinished() )
        {   qDebug ( ) << this , &MainWindow::ReadReady_HoldingCustom;
           qDebug ( ) << "function reads the entry slot" ;
           return true ;
         }
         else
        {
            qDebug() <<  "delete reply in advance" ;
            delete reply ;
            return false ;
        }

     }

}
}








void MainWindow::ReadReady_HoldingCustom()
{
    uint pos = 24;
    qDebug() <<  "started slot function" ;
    QModbusReply * reply = qobject_cast < QModbusReply * > (sender());

         if  (!reply) {
             qDebug()<<QModbusDevice::NoError<<  "Quit early" ;
             return  ;
         }
       if  ( reply -> error() == QModbusDevice::NoError) {
           qDebug () <<  "receive data" ;
           const QModbusDataUnit Unit = reply -> result();



               /*
                QByteArray AllData =unit.values();//Read all at once
               */

               uint16_t res = Unit.value(readCustomPos) ;
               uint16_t res1 = Unit.value(3) ;

               ReadValue = res;
               qDebug()<<res;
              QString converted = QString::number(res);
             // ui ->label_3 -> setText(converted);
              ReadValue = res;
              readHolding = res;
              readHoldingWord = res;
              QString converted1 = QString::number(res1);
              ui ->label -> setText("Proklizavanje: " + converted1);
              // uint16_t Coils_Bufer[length];//Read one by one
               //Coils_Bufer[i] = res ;
               //After reading, store the data Coils_Bufer[i] Customized array to store data

           }


       else
       {
       }

       reply -> deleteLater() ;  //delete the reply
}










void MainWindow::on_pushButton_pressed()
{
     Write_modbus_tcp_Coils(1, 22, "1", 0,  10);
     ui -> pushButton->setStyleSheet("QPushButton { background-color : green; color : black; }");

}


void MainWindow::on_pushButton_released()
{
    Write_modbus_tcp_Coils(0, 22, "1", 0,  10);
    ui -> pushButton->setStyleSheet("QPushButton { background-color : red; color : black; }");

}


void MainWindow::on_pushButton_2_pressed()
{
    Write_modbus_tcp_Coils(0, 26, "1", 0,  10);
    ui -> pushButton_2->setStyleSheet("QPushButton { background-color : green; color : black; }");


}


void MainWindow::on_pushButton_2_released()
{
    Write_modbus_tcp_Coils(0, 26, "1", 0,  10);
    ui -> pushButton_2->setStyleSheet("QPushButton { background-color : red; color : black; }");

}


void MainWindow::on_pushButton_3_pressed()
{
    Write_modbus_tcp_Coils(1, 24, "1", 0,  10);
    ui -> pushButton_3->setStyleSheet("QPushButton { background-color : green; color : black; }");

}


void MainWindow::on_pushButton_3_released()
{
    Write_modbus_tcp_Coils(0, 24, "1", 0,  10);
    ui -> pushButton_3->setStyleSheet("QPushButton { background-color : red; color : black; }");

}



void MainWindow::on_pushButton_4_pressed()
{
    Write_modbus_tcp_Coils(0, 25, "1", 0,  10);
    ui -> pushButton_4->setStyleSheet("QPushButton { background-color : green; color : black; }");


}


void MainWindow::on_pushButton_4_released()
{
    Write_modbus_tcp_Coils(1, 25, "1", 0,  10);
    ui -> pushButton_4->setStyleSheet("QPushButton { background-color : red; color : black; }");

}


void MainWindow::on_pushButton_5_clicked()
{

IstorijaGresaka istorija;
istorija.setWindowFlags(Qt::Window);
istorija.setModal(true);
istorija.exec();
        // istorija1 = new istorija(this);




}

void MainWindow::on_pushButton_6_clicked()
{
    QString IPAdresa = ui -> lineEdit -> text();
    QString brojporta = ui -> lineEdit_2 -> text();
    int port = brojporta.toInt();
    Connect_to_modbus(IPAdresa, port);

}





void MainWindow::on_pushButton_7_clicked()
{
    Write_modbus_tcp_Coils(1, 100, "1", 0,  10);

}


void MainWindow::on_pushButton_8_clicked()
{
    Write_modbus_tcp_Holding(33, 4, "1", 0,  10);
}


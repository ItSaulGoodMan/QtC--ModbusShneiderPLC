#include "istorijagresaka.h"
#include "ui_istorijagresaka.h"
#include <QtSql>
#include <QDebug>
#include <QString>
#include <QTime>
using namespace std;



QString putanja = QCoreApplication::applicationDirPath();
QString baza = putanja + ".baza.db";


IstorijaGresaka::IstorijaGresaka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IstorijaGresaka)
{

    ui->setupUi(this);
     // add this.

    ui -> tableView ->setAlternatingRowColors(true);
    ui -> tableView ->setStyleSheet("alternate-background-color:rgba(221,235,247,255);background-color:rgba(255,255,255,255);");

    ui -> tableView_2 ->setAlternatingRowColors(true);
    ui -> tableView_2 ->setStyleSheet("alternate-background-color:rgba(221,235,247,255);background-color:rgba(255,255,255,255);");

    ui -> tableView_3 ->setAlternatingRowColors(true);
    ui -> tableView_3 ->setStyleSheet("alternate-background-color:rgba(221,235,247,255);background-color:rgba(255,255,255,255);");


    QPixmap bkgnd(":/resursi/resursi/pozadina2.jpg");
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);

    istorijagresaka();
}

IstorijaGresaka::~IstorijaGresaka()
{
    delete ui;
}

void IstorijaGresaka::resizeEvent(QResizeEvent *evt)
{
QPixmap bkgnd(":/resursi/resursi/pozadina2.jpg");//Load pic
bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);//set scale of pic to match the window
QPalette p = palette(); //copy current, not create new
p.setBrush(QPalette::Background, bkgnd);//set the pic to the background
setPalette(p);//show the background pic

QDialog::resizeEvent(evt); //call base implementation
}




void IstorijaGresaka::istorijagresaka()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(baza);

    if(!mydb.open())
 qDebug()<<"Otvorena baza";
else
 qDebug()<<"Nije otvorena baza";    QSqlQuery *qry = new QSqlQuery(mydb);

    qry -> prepare("SELECT * FROM testtable");
    qry -> exec();
    model -> setQuery(*qry);
    ui -> tableView -> setModel(model);



}

void IstorijaGresaka::on_commandLinkButton_clicked()
{
    QString greskica = ui->lineEdit->text();
    int brojgreske = greskica.toInt();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(baza);

    if(!mydb.open())
 qDebug()<<"Otvorena baza";
else
 qDebug()<<"Nije otvorena baza";
    QSqlQuery *qry = new QSqlQuery(mydb);

    QString kveri = "SELECT * FROM testtable WHERE brGreske = " + greskica;

    qry -> prepare(kveri);
    qry -> exec();
    model -> setQuery(*qry);
    ui -> tableView_2 -> setModel(model);

}


void IstorijaGresaka::on_comboBox_activated(int index)
{
    //int brojgreske = greskica.toInt();
    QString greskica = QString::number(index);
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(baza);

    if(!mydb.open())
 qDebug()<<"Otvorena baza";
else
 qDebug()<<"Nije otvorena baza";
    QSqlQuery *qry = new QSqlQuery(mydb);

    QString kveri = "SELECT * FROM testtable WHERE brGreske = " + greskica;

    qry -> prepare(kveri);
    qry -> exec();
    model -> setQuery(*qry);
    ui -> tableView_2 -> setModel(model);



}


void IstorijaGresaka::on_pushButton_clicked()
{
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(baza);
    //mydb.setDatabaseName(baza);

    if(!mydb.open())
      qDebug()<<"Otvorena baza";
    else
     qDebug()<<"Nije otvorena baza";
    QSqlQuery *qry = new QSqlQuery(mydb);

    QString kveri = "DELETE FROM testtable;";

    qry -> prepare(kveri);
    qry -> exec();
    istorijagresaka();
    on_comboBox_activated(0);

}




void IstorijaGresaka::on_pushButton_2_clicked()
{
    istorijagresaka();

}


void IstorijaGresaka::on_pushButton_3_clicked()
{
    int index = 5;
    QString greskica = QString::number(index);
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(baza);

    if(!mydb.open())
 qDebug()<<"Otvorena baza";
else
 qDebug()<<"Nije otvorena baza";
    QSqlQuery *qry = new QSqlQuery(mydb);
    QDateTime pocetak = ui -> dateTimeEdit -> dateTime();
    QDateTime kraj = ui -> dateTimeEdit_2 -> dateTime();


    QString start_time = pocetak.toString();
    QString end_time = kraj.toString();
qDebug()<<start_time;

    QString kveri1 = "SELECT * FROM testtable WHERE timestamp BETWEEN ? AND ?;";

    QString kveri = "SELECT * FROM testtable WHERE timestamp BETWEEN " + start_time + "AND" + end_time + ";";

    qry -> prepare(kveri1);
    qry->addBindValue(pocetak);
    qry->addBindValue(kraj);

    qry -> exec();
    model -> setQuery(*qry);
    ui -> tableView_3 -> setModel(model);
}


#include "formutama.h"
#include "ui_formutama.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

formUtama::formUtama(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::formUtama)
{
    ui->setupUi(this);
    qDebug() << QSqlDatabase::drivers();

    Koneksi = QSqlDatabase::addDatabase("QSQLITE");
//    Koneksi.setHostName("localhost");
//    Koneksi.setPort(3307);
//    Koneksi.setUserName("root");
//    Koneksi.setPassword("");
    Koneksi.setDatabaseName("E:/pertanian.db");

    if (Koneksi.open())
    {
        qDebug() << "OK";
    }else
    {
        qDebug() << "No";
        qDebug() << "Error :" << Koneksi.lastError();
    }


}

formUtama::~formUtama()
{
    delete ui;
}

void formUtama::on_pushButton_clicked()
{
    uiADM = new admin(this);
    uiADM->show();
}


void formUtama::on_pushButton_2_clicked()
{
    uiPBL = new pembeli(this);
    uiPBL->show();
}


void formUtama::on_pushButton_3_clicked()
{
    uiPRD = new produk(this);
    uiPRD->show();
}


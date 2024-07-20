#include "admin.h"
#include "ui_admin.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QtSql>

admin::admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin)
    , model(new QSqlTableModel(this))
{
    ui->setupUi(this);

    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    Koneksi = QSqlDatabase::addDatabase("QSQLITE");
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

admin::~admin()
{
    Koneksi.close();
    delete ui;
}

void admin::on_admAmbilData_clicked()
{
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    QSqlQuery query(Koneksi);

    if (!query.exec("SELECT id_admin, username, password, alamat, no_hp FROM admin"))
    {
        qDebug() << "Error retrieving data from database:" << query.lastError();
        return;
    }

    // Bersihkan tabel sebelum mengisinya
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    // Setel header tabel
    QStringList headers = {"ID Admin", "Username", "Password", "Alamat", "No HP"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(query.value("id_admin").toInt())));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("username").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("password").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("alamat").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("no_hp").toString()));
        row++;
    }

    Koneksi.close();
}


void admin::on_pushButton_clicked()
{

}


void admin::on_admTambah_clicked()
{
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error :" << Koneksi.lastError();
            return;
        }
    }

    QSqlQuery QueryInsertData(Koneksi);
    QueryInsertData.prepare("INSERT INTO admin(id_admin, username, password, alamat, no_hp) VALUES(:id_admin, :username, :password, :alamat, :no_hp)");
    QueryInsertData.bindValue(":id_admin", ui->lineEdit->text().toInt());
    QueryInsertData.bindValue(":username", ui->lineEdit_2->text());
    QueryInsertData.bindValue(":password", ui->lineEdit_3->text());
    QueryInsertData.bindValue(":alamat", ui->lineEdit_4->text());
    QueryInsertData.bindValue(":no_hp", ui->lineEdit_5->text());

    if (!QueryInsertData.exec()) {
        qDebug() << "Error inserting data into database:" << QueryInsertData.lastError();
    } else {
        qDebug() << "Data successfully inserted!";
        QMessageBox::information(this, "Success", "Data berhasil ditambahkan!");

        // Hapus semua isian input
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_5->clear();
    }

    Koneksi.close();
}


void admin::on_admHapus_clicked()
{
    // Dapatkan baris yang dipilih di QTableWidget
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Tidak ada baris yang dipilih untuk dihapus.");
        return;
    }

    // Ambil baris pertama yang dipilih
    int selectedRow = ui->tableWidget->row(selectedItems[0]);

    // Ambil nilai id_admin dari kolom pertama di baris yang dipilih
    QTableWidgetItem *idItem = ui->tableWidget->item(selectedRow, 0);
    if (!idItem) {
        QMessageBox::warning(this, "Peringatan", "Tidak dapat menemukan ID Admin di baris yang dipilih.");
        return;
    }

    int id_admin = idItem->text().toInt();

    // Buka koneksi database
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    // Hapus data dari database
    QSqlQuery query(Koneksi);
    query.prepare("DELETE FROM admin WHERE id_admin = :id_admin");
    query.bindValue(":id_admin", id_admin);

    if (!query.exec()) {
        qDebug() << "Error deleting data from database:" << query.lastError();
        QMessageBox::warning(this, "Error", "Gagal menghapus data dari database.");
    } else {
        qDebug() << "Data successfully deleted!";
        QMessageBox::information(this, "Success", "Data berhasil dihapus!");

        // Hapus baris dari QTableWidget
        ui->tableWidget->removeRow(selectedRow);
    }

    Koneksi.close();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
}


void admin::on_admEdit_clicked()
{
    // Dapatkan nilai id_admin dari QLineEdit
    int id_admin = ui->lineEdit->text().toInt();

    // Buka koneksi database jika belum terbuka
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    // Perbarui data di database
    QSqlQuery query(Koneksi);
    query.prepare("UPDATE admin SET username = :username, password = :password, alamat = :alamat, no_hp = :no_hp WHERE id_admin = :id_admin");
    query.bindValue(":id_admin", id_admin);
    query.bindValue(":username", ui->lineEdit_2->text());
    query.bindValue(":password", ui->lineEdit_3->text());
    query.bindValue(":alamat", ui->lineEdit_4->text());
    query.bindValue(":no_hp", ui->lineEdit_5->text());

    if (!query.exec()) {
        qDebug() << "Error updating data in database:" << query.lastError();
        QMessageBox::warning(this, "Error", "Gagal memperbarui data di database.");
    } else {
        qDebug() << "Data successfully updated!";
        QMessageBox::information(this, "Success", "Data berhasil diperbarui!");

        // Perbarui data di QTableWidget
        QList<QTableWidgetItem *> selectedItems = ui->tableWidget->selectedItems();
        if (!selectedItems.isEmpty()) {
            int selectedRow = ui->tableWidget->row(selectedItems[0]);
            ui->tableWidget->setItem(selectedRow, 0, new QTableWidgetItem(QString::number(id_admin)));
            ui->tableWidget->setItem(selectedRow, 1, new QTableWidgetItem(ui->lineEdit_2->text()));
            ui->tableWidget->setItem(selectedRow, 2, new QTableWidgetItem(ui->lineEdit_3->text()));
            ui->tableWidget->setItem(selectedRow, 3, new QTableWidgetItem(ui->lineEdit_4->text()));
            ui->tableWidget->setItem(selectedRow, 4, new QTableWidgetItem(ui->lineEdit_5->text()));
        }
    }

    Koneksi.close();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();

}


void admin::on_tableWidget_itemSelectionChanged()
{
    // Dapatkan baris yang dipilih di QTableWidget
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    // Ambil baris pertama yang dipilih
    int selectedRow = ui->tableWidget->row(selectedItems[0]);

    // Ambil nilai dari kolom di baris yang dipilih dan set ke QLineEdit
    ui->lineEdit->setText(ui->tableWidget->item(selectedRow, 0)->text());
    ui->lineEdit_2->setText(ui->tableWidget->item(selectedRow, 1)->text());
    ui->lineEdit_3->setText(ui->tableWidget->item(selectedRow, 2)->text());
    ui->lineEdit_4->setText(ui->tableWidget->item(selectedRow, 3)->text());
    ui->lineEdit_5->setText(ui->tableWidget->item(selectedRow, 4)->text());
}


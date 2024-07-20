#include "produk.h"
#include "ui_produk.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

produk::produk(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::produk)
{
    ui->setupUi(this);

    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    Koneksi = QSqlDatabase::addDatabase("QSQLITE");
    Koneksi.setDatabaseName("E:/pertanian.db");

    if (Koneksi.open())
    {
        qDebug() << "Database connection: OK";
    } else {
        qDebug() << "Database connection: Failed";
        qDebug() << "Error :" << Koneksi.lastError();
    }

    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &produk::on_tableWidget_itemSelectionChanged);
}

produk::~produk()
{
    Koneksi.close();
    delete ui;
}

void produk::on_pushButton_clicked()
{
    refreshTable();
}

void produk::refreshTable()
{
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    QSqlQuery query(Koneksi);

    if (!query.exec("SELECT id_produk, nama_produk, harga, deskpripsi, stok FROM produk"))
    {
        qDebug() << "Error retrieving data from database:" << query.lastError();
        return;
    }

    // Bersihkan tabel sebelum mengisinya
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    // Setel header tabel
    QStringList headers = {"ID Produk", "Nama Produk", "Harga", "Deskripsi", "Stok"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(query.value("id_produk").toInt())));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("nama_produk").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("harga").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("deskpripsi").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("stok").toString()));
        row++;
    }

    Koneksi.close();
}

void produk::on_pushButton_2_clicked()
{
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    QSqlQuery query(Koneksi);
    query.prepare("INSERT INTO produk(id_produk, nama_produk, harga, deskpripsi, stok) VALUES(:id_produk, :nama_produk, :harga, :deskpripsi, :stok)");
    query.bindValue(":id_produk", ui->lineEdit->text().toInt());
    query.bindValue(":nama_produk", ui->lineEdit_2->text());
    query.bindValue(":harga", ui->lineEdit_3->text());
    query.bindValue(":deskpripsi", ui->lineEdit_4->text());
    query.bindValue(":stok", ui->lineEdit_5->text());

    if (!query.exec()) {
        qDebug() << "Error inserting data into database:" << query.lastError();
        QMessageBox::warning(this, "Error", "Gagal menambah data ke database.");
    } else {
        qDebug() << "Data successfully inserted!";
        QMessageBox::information(this, "Success", "Data berhasil ditambahkan!");

        refreshTable(); // Perbarui tampilan tabel setelah data ditambahkan

        // Hapus semua isian input
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
    }

    Koneksi.close();
}

void produk::on_pushButton_3_clicked()
{
    // Dapatkan baris yang dipilih di QTableWidget
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Tidak ada baris yang dipilih untuk dihapus.");
        return;
    }

    // Ambil baris pertama yang dipilih
    int selectedRow = ui->tableWidget->row(selectedItems[0]);

    // Ambil nilai id_produk dari kolom pertama di baris yang dipilih
    QTableWidgetItem *idItem = ui->tableWidget->item(selectedRow, 0);
    if (!idItem) {
        QMessageBox::warning(this, "Peringatan", "Tidak dapat menemukan ID Produk di baris yang dipilih.");
        return;
    }

    int id_produk = idItem->text().toInt();

    // Buka koneksi database jika belum terbuka
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    // Hapus data dari database
    QSqlQuery query(Koneksi);
    query.prepare("DELETE FROM produk WHERE id_produk = :id_produk");
    query.bindValue(":id_produk", id_produk);

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
}

void produk::on_pushButton_4_clicked()
{
    // Dapatkan nilai id_produk dari QLineEdit
    int id_produk = ui->lineEdit->text().toInt();

    // Buka koneksi database jika belum terbuka
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    // Perbarui data di database
    QSqlQuery query(Koneksi);
    query.prepare("UPDATE produk SET nama_produk = :nama_produk, harga = :harga, deskpripsi = :deskpripsi, stok = :stok WHERE id_produk = :id_produk");
    query.bindValue(":id_produk", id_produk);
    query.bindValue(":nama_produk", ui->lineEdit_2->text());
    query.bindValue(":harga", ui->lineEdit_3->text());
    query.bindValue(":deskpripsi", ui->lineEdit_4->text());
    query.bindValue(":stok", ui->lineEdit_5->text());

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
            ui->tableWidget->setItem(selectedRow, 0, new QTableWidgetItem(QString::number(id_produk)));
            ui->tableWidget->setItem(selectedRow, 1, new QTableWidgetItem(ui->lineEdit_2->text()));
            ui->tableWidget->setItem(selectedRow, 2, new QTableWidgetItem(ui->lineEdit_3->text()));
            ui->tableWidget->setItem(selectedRow, 3, new QTableWidgetItem(ui->lineEdit_4->text()));
            ui->tableWidget->setItem(selectedRow, 4, new QTableWidgetItem(ui->lineEdit_5->text()));
        }
    }

    Koneksi.close();
}

void produk::on_tableWidget_itemSelectionChanged()
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


#include "pembeli.h"
#include "ui_pembeli.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

pembeli::pembeli(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pembeli)
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

    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &pembeli::on_tableWidget_itemSelectionChanged);
}

pembeli::~pembeli()
{
    Koneksi.close();
    delete ui;
}

void pembeli::on_pushButton_clicked()
{
    refreshTable();
}

void pembeli::refreshTable()
{
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    QSqlQuery query(Koneksi);

    if (!query.exec("SELECT id_pembeli, nama_pembeli, alamat, email, no_hp FROM pembeli"))
    {
        qDebug() << "Error retrieving data from database:" << query.lastError();
        return;
    }

    // Bersihkan tabel sebelum mengisinya
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    // Setel header tabel
    QStringList headers = {"ID Pembeli", "Nama Pembeli", "Alamat", "Email", "No HP"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(query.value("id_pembeli").toInt())));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("nama_pembeli").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("alamat").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("email").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("no_hp").toString()));
        row++;
    }

    Koneksi.close();
}

void pembeli::on_pushButton_2_clicked()
{
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    QSqlQuery query(Koneksi);
    query.prepare("INSERT INTO pembeli(id_pembeli,nama_pembeli, alamat, email, no_hp) VALUES(:id_pembeli,:nama_pembeli, :alamat, :email, :no_hp)");
    query.bindValue(":id_pembeli", ui->lineEdit->text().toInt());
    query.bindValue(":nama_pembeli", ui->lineEdit_2->text());
    query.bindValue(":alamat", ui->lineEdit_3->text());
    query.bindValue(":email", ui->lineEdit_4->text());
    query.bindValue(":no_hp", ui->lineEdit_4->text());

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
        ui->lineEdit_5->clear();
    }

    Koneksi.close();
}

void pembeli::on_pushButton_3_clicked()
{
    // Dapatkan baris yang dipilih di QTableWidget
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Tidak ada baris yang dipilih untuk dihapus.");
        return;
    }

    // Ambil baris pertama yang dipilih
    int selectedRow = ui->tableWidget->row(selectedItems[0]);

    // Ambil nilai id_pembeli dari kolom pertama di baris yang dipilih
    QTableWidgetItem *idItem = ui->tableWidget->item(selectedRow, 0);
    if (!idItem) {
        QMessageBox::warning(this, "Peringatan", "Tidak dapat menemukan ID Pembeli di baris yang dipilih.");
        return;
    }

    int id_pembeli = idItem->text().toInt();

    // Buka koneksi database jika belum terbuka
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    // Hapus data dari database
    QSqlQuery query(Koneksi);
    query.prepare("DELETE FROM pembeli WHERE id_pembeli = :id_pembeli");
    query.bindValue(":id_pembeli", id_pembeli);

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

void pembeli::on_pushButton_4_clicked()
{
    // Dapatkan nilai id_pembeli dari QLineEdit
    int id_pembeli = ui->lineEdit->text().toInt();

    // Buka koneksi database jika belum terbuka
    if (!Koneksi.isOpen()) {
        if (!Koneksi.open()) {
            qDebug() << "Error opening database:" << Koneksi.lastError();
            return;
        }
    }

    // Perbarui data di database
    QSqlQuery query(Koneksi);
    query.prepare("UPDATE pembeli SET nama_pembeli = :nama_pembeli, alamat = :alamat, email = :email, no_hp = :no_hp WHERE id_pembeli = :id_pembeli");
    query.bindValue(":id_pembeli", id_pembeli);
    query.bindValue(":nama_pembeli", ui->lineEdit_2->text());
    query.bindValue(":alamat", ui->lineEdit_3->text());
    query.bindValue(":email", ui->lineEdit_4->text());
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
            ui->tableWidget->setItem(selectedRow, 0, new QTableWidgetItem(QString::number(id_pembeli)));
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

void pembeli::on_tableWidget_itemSelectionChanged()
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

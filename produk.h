#ifndef PRODUK_H
#define PRODUK_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QDialog>
#include <QSqlDatabase>
#include <QTableWidgetItem>


namespace Ui {
class produk;
}

class produk : public QDialog
{
    Q_OBJECT

public:
    explicit produk(QWidget *parent = nullptr);
    ~produk();

private slots:
    void on_pushButton_clicked(); // Menampilkan data produk
    void on_pushButton_2_clicked(); // Menambah data produk
    void on_pushButton_3_clicked(); // Menghapus data produk
    void on_pushButton_4_clicked(); // Mengedit data produk

    void on_tableWidget_itemSelectionChanged(); // Menampilkan data ke lineEdit saat dipilih

private:
    Ui::produk *ui;
    QSqlDatabase Koneksi;
    void refreshTable();
};

#endif // PRODUK_H

#ifndef PEMBELI_H
#define PEMBELI_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlDatabase>
#include <QTableWidgetItem>


namespace Ui {
class pembeli;
}

class pembeli : public QDialog
{
    Q_OBJECT

public:
    explicit pembeli(QWidget *parent = nullptr);
    ~pembeli();

private slots:
    void on_pushButton_clicked(); // Menampilkan data pembeli
    void on_pushButton_2_clicked(); // Menambah data pembeli
    void on_pushButton_3_clicked(); // Menghapus data pembeli
    void on_pushButton_4_clicked(); // Mengedit data pembeli

    void on_tableWidget_itemSelectionChanged(); // Menampilkan data ke lineEdit saat dipilih


private:
    Ui::pembeli *ui;
    QSqlDatabase Koneksi;
    void refreshTable();
};

#endif // PEMBELI_H

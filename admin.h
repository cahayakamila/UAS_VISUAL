#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QSqlTableModel>

namespace Ui {
class admin;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void on_admAmbilData_clicked();

    void on_pushButton_clicked();

    void on_admTambah_clicked();

    void on_admHapus_clicked();

    void on_admEdit_clicked();

    void on_tableWidget_itemSelectionChanged();

private:
    Ui::admin *ui;
    QSqlDatabase Koneksi;
    QSqlTableModel *model;
};

#endif // ADMIN_H

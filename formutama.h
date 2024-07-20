#ifndef FORMUTAMA_H
#define FORMUTAMA_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QSql>
#include <admin.h>
#include <pembeli.h>
#include <produk.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class formUtama;
}
QT_END_NAMESPACE

class formUtama : public QMainWindow
{
    Q_OBJECT

public:
//    QSqlDatabase Koneksi;
    formUtama(QWidget *parent = nullptr);
    ~formUtama();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::formUtama *ui;
    admin *uiADM;
    pembeli *uiPBL;
    produk *uiPRD;
  QSqlDatabase Koneksi;
};
#endif // FORMUTAMA_H

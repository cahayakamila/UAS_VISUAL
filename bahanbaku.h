#ifndef BAHANBAKU_H
#define BAHANBAKU_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class bahanBaku;
}
QT_END_NAMESPACE

class bahanBaku : public QMainWindow
{
    Q_OBJECT

public:
    bahanBaku(QWidget *parent = nullptr);
    ~bahanBaku();

private:
    Ui::bahanBaku *ui;
};
#endif // BAHANBAKU_H

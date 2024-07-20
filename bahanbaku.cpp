#include "bahanbaku.h"
#include "ui_bahanbaku.h"

bahanBaku::bahanBaku(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::bahanBaku)
{
    ui->setupUi(this);
}

bahanBaku::~bahanBaku()
{
    delete ui;
}

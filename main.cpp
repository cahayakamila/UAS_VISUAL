#include "bahanbaku.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bahanBaku w;
    w.show();
    return a.exec();
}

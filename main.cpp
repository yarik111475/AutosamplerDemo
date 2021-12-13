#include <QtCore>
#include <QCoreApplication>
#include "ht1500lconnector.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HT1500LConnector connector("COM1");
    connector.readCfgAndSwReleaseParameters();
    return a.exec();
}

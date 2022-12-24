#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int
main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QTranslator SysTranslator1;
    QTranslator SysTranslator2;
    SysTranslator1.load(QString(":/lang/qt_zh_CN.qm"));
    SysTranslator2.load(QString(":/lang/tr_language_Chinese.qm"));
    a.installTranslator(&SysTranslator1);
    a.installTranslator(&SysTranslator2);
    MainWindow w;
    // w.show();
    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}

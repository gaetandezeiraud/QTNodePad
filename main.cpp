#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Dezeiraud Software");
    a.setOrganizationDomain("https://gaetan.dezeiraud.com");
    a.setApplicationName("QT NotePad");
    a.setApplicationDisplayName("QT NotePad");
    a.setApplicationVersion("0.0.1");

    MainWindow w;
    w.show();
    return a.exec();
}

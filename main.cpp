#include "mainwindow.h"
#include "axml.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AXml xml;
    //xml.writeXML();
    //xml.readXML();
    //xml.addXML();
    //xml.deleteXML();
    xml.amendXML();
    //MainWindow w;
    //w.show();
    return a.exec();
}

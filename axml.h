#ifndef AXML_H
#define AXML_H

#include <QTextStream>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QDomDocument>
#include <QString>
#include <QDebug>
#include <QCoreApplication>

class AXml
{
public:
    AXml();
    void writeXML();
    void readXML();
    void addXML();
    void deleteXML();
    void amendXML();
};

#endif // AXML_H

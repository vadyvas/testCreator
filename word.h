#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QMap>
#include <QImage>
#include <QtXml>
#include <JlCompress.h>


class word
{
public:
    word();

    QMap <QString , QString> img;

    QString readDocx(QString);

private:
    QString xml_str = "";

    QString getXml(QString);
    void traverseNode(const QDomNode&);

    void copyImg(QString path);
};

#endif // WORD_H

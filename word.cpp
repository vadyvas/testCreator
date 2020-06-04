#include "word.h"

word::word()
{

}

QString word::readDocx(QString pathToDocx)
{
    QString path = QDir::currentPath();
    JlCompress::extractDir(pathToDocx, path + "/tmp");

    // get img
    getXml(path + "/tmp/word/_rels/document.xml.rels");
    // get text
    QString xml_text = getXml(path + "/tmp/word/document.xml");

    copyImg(path);

    return xml_text;
}

QString word::getXml(QString path){

    QDomDocument domDoc;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement domElement= domDoc.documentElement();
            traverseNode(domElement);
        }
        file.close();
    }
    return xml_str;
}

void word::traverseNode(const QDomNode& node)
{
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
                if(domElement.tagName() == "w:t") {
                    this->xml_str += domElement.text();
                    //qDebug() << domElement.text();
                }
                if (domElement.tagName() == "Relationship")
                    if (domElement.attribute("Target", "").contains("media"))
                    {
                        img.insert(domElement.attribute("Id", ""), domElement.attribute("Target", ""));
                    }
                if (domElement.tagName() == "a:blip")
                {
                    this->xml_str += "<img style\\= \"vertical-align: middle; margin: 10px;\" src\\=\"@@PLUGINFILE@@/";
                    this->xml_str += img[domElement.attribute("r:embed", "")];
                    this->xml_str += "\" alt\\=\"image\" />";
                }
            }
        }
        traverseNode(domNode);
        domNode = domNode.nextSibling();
    }
}

void word::copyImg(QString path)
{
    QDir *dir = new QDir();
    dir->setPath(path + "/tmp/word/media");

    QStringList files = dir->entryList(QDir::Files);

    foreach(QString f , files)
    {
        qDebug () << f;
        QFile::copy(path + "/tmp/word/media/" + f, path + "/temp/media/" + f);
    }

}

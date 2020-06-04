#include "config.h"

#include <QDebug>

config::config()
{
    pathToApp = QDir::currentPath();

    // get info from file
    getConfig();
}

config::config(QString path)
{
    pathToApp = path;

    // get info from file
    getConfig();
}

// отримує інформацію із файлу
void config::getConfig(){
    QFile file(pathToApp + "/config.cfg");
    if (file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        for(int i = 0; !in.atEnd(); i++)  //читає файл до кінця
        {
            switch (i)
            {
            case 0: this->openPath = in.readLine(); break;
            case 1: this->savePath = in.readLine(); break;
            case 2: this->openExportPath = in.readLine(); break;
            case 3: this->openImgPath = in.readLine(); break;
            case 4: this->questionSymbol = (in.readLine()).toStdString()[0]; break;
            case 5: this->answerSymbol = (in.readLine()).toStdString()[0]; break;
            default: this->recentFiles.push_back(in.readLine()); break;
            }
        }
        file.close();
    } else {
     qDebug() << "conf not open. Set default settings";

         this->openPath = ".";
         this->savePath = ".";
         this->openExportPath = ".";
         this->openImgPath =  ".";
         this->questionSymbol = '$';
         this->answerSymbol = '#';
    }
}

// оновлює інформацію в конфіг файлі
void config::updateConfig()
{
    QFile file(pathToApp + "/config.cfg");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        QString str =  openPath + "\n" + savePath + "\n" + openExportPath + "\n" + openImgPath + "\n" + questionSymbol + "\n" + answerSymbol + "\n";

        for (int i = 0; i < 7 && i < recentFiles.size(); i++)
        {
            str+= recentFiles.at(i) + "\n";
        }

        stream << str;
        file.close();
    }
}

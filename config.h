#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>

class config
{
public:
    config();
    config(QString);

    QString savePath,
            openPath,
            openExportPath,
            openImgPath;

    char questionSymbol,
        answerSymbol;

    QStringList recentFiles;

    void updateConfig();

private:
    void getConfig();

    QString pathToApp;
};

#endif // CONFIG_H

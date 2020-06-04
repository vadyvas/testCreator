#ifndef QUESTIONS_H
#define QUESTIONS_H


#include <QMap>
#include <QString>

#include "quest.h"


class Questions
{
public:
    Questions();

    void setExportOption(QChar, QChar);

    void addQuestion (QString, question);
    void setQuestion (QString, question); // змінює запитання
    question getQuestion (QString);

    QString giftQuestion();

    void setQuestionFromXml(QString);
    void setQuestionFromGift(QString);


    QMap <QString, question> map;

private:
    QChar option_question = '$',
        option_answer = '#';
};

#endif // QUESTIONS_H

#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QStringList>

class question
{
public:
    question();
    question(int type){
        this->type = type;
    }

    QString ques;
    QStringList answers;
    int ra = 0, wa = 0,
        type;

    QString questionFormatGift();
    void clear();

};

#endif // QUESTION_H

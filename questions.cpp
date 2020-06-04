#include "questions.h"
#include <QDebug>

Questions::Questions()
{
}

// встановлює налаштування для експорту
void Questions::setExportOption(QChar q, QChar a)
{
    this->option_question = q;
    this->option_answer = a;
}

void Questions::addQuestion (QString name, question q){
    map[name] = q;
}

void Questions::setQuestion (QString name, question q){
    map[name].clear();
    map[name] = q;
}

question Questions::getQuestion(QString name)
{
    return map[name];
}

// перетворення стрічки отриманої із docx в питання
void Questions::setQuestionFromXml(QString xml_str)
{
    QString formated_str = "";
    question q;
    QString temp,name;

    xml_str = " " + xml_str;

    bool f = false; // after # or no
    for(int i = 1; i < xml_str.length(); i++)
    {
        // && xml_str[i - 1] != '\\' && !(xml_str [i + 1] == '@' || xml_str [i - 1] == '@')
        if(xml_str[i] == this->option_question )
        {
            if (q.ques.length() != 0)
            {
                if (q.ra > 1)
                    q.type = 1;
                else q.type = 0;

                // проста перевірка чи питання існує
                if (getQuestion(name).ques != "")
                    for (int j = 1; true; j ++)
                        if (getQuestion(QString::number(j)).ques == "")
                        { name = QString::number(j); break; }

                map[name] = q;
                name = "";
                //                formated_str += q.formatQuestion();
                q.clear();
            }
            i++;
            bool number = true;
            for (; i < xml_str.length(); i++)
            {
                if (number && xml_str[i].isDigit()){
                    name += xml_str[i];
                }
                else
                {
                    number = false;
                    if (xml_str[i] != this->option_answer)
                        q.ques += xml_str[i];
                    else break;
                }
            }
        }
        if (xml_str[i] == this->option_answer)
        {
            i++;
            temp = "";
            f = true;
            for (;i< xml_str.length(); i++)
            {
                if (xml_str[i] == '+' && f)
                {
                    q.ra ++;
                    f = false;
                }else
                    if (xml_str[i] == '-' && f)
                    {
                        q.wa ++;
                        f = false;
                        // xml_str[i] == this->option_answer || xml_str[i] == this->option_question
                    }
                if (xml_str[i] == this->option_answer || (xml_str[i] == this->option_question && (xml_str[i + 1] != '@' && xml_str[i - 1] != '@') && xml_str[i - 1] != '\\' ))
                {
                    i--;
                    q.answers.push_back(temp);
                    break;
                }
                temp += xml_str[i];
            }
        }
    }
    q.answers.push_back(temp);
    if (q.ra > 1)
        q.type = 1;
    else q.type = 0;

    if (getQuestion(name).ques != "")
        for (int j = 1; true; j ++)
            if (getQuestion(QString::number(j)).ques == "")
            { name = QString::number(j); break; }

    map[name] = q;
    //    return formated_str;
}

// перетворення стрічки отриманої із gift.txt в питання
void Questions::setQuestionFromGift(QString str)
{
    QString  temp = "";
    str = " " + str;
    str += " ";

    question q;
    q.type = 0;

    for (int i = 1; i < str.length() - 1; i++)
    {
        if (str[i] == ':' && str[i + 1] == ':')
        {
            q.clear();
            q.type = 0;

            for (i++;i < str.length() - 1; i++)
                if (str[i] == ':' && str[i + 1] == ':')
                {
                    i++;
                    break;
                }
            for (i++; str.length() - 1; i++)
            {
                if (str[i] != '{' )
                {
                    temp += str[i];
                }
                else {
                    q.ques = temp;
                    temp = "";
                    break;
                }
            }
        }
        if (str [i] == '{')
        {
            for ( i++; i < str.length() - 1; i++)
            {
                if (str[i] == '=' && str[i - 1] != '\\')
                {
                    q.ra ++;
                    if (temp.length() > 1)
                    {
                        q.answers.push_back(temp);
                        temp = "+";
                    } else temp = "+";
                }else
                    if (str[i] == '~' && str[i + 1] == '%')
                    {
                        q.ra ++;
                        if (temp.length() > 1)
                        {
                            q.type = 1;
                            q.answers.push_back(temp);
                            temp = "+";

                        }
                        else temp = "+";
                        for (i += 2; i < str.length() - 1 && str[i] != '%' ; i++);
                        //                                        qDebug () << str[i];
                    } else if (str[i] == '~' && str[i - 1] != '\\')
                    {
                        q.wa ++;
                        if (temp.length() > 1)
                        {
                            q.answers.push_back(temp);
                            temp = "-";

                        } else temp = "-";
                    } else if (str[i] == '}' && str[i - 1] != '\\')
                    {
                        //qDebug() << q.ques << q.ra << q.wa;
                        q.answers.push_back(temp);

                        QString name = "1";

                        // проста перевірка чи питання існує
                        if (getQuestion("1").ques != "")
                            for (int i = 1; true; i ++)
                                if (getQuestion(QString::number(i)).ques == "")
                                { name = QString::number(i); break; }

                        addQuestion(name, q);
                        temp = "";
                        break;
                    } else
                        temp += str[i];
            }
        }
    }
}

QString Questions::giftQuestion(){
    QString formated_str = "";

    question q;

    foreach (QString key, map.keys())
    {
        q = map.value(key);
        formated_str += "::Q" + key + "::" + q.questionFormatGift();
    }

    return formated_str;
}

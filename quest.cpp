#include "quest.h"

question::question()
{}

void question::clear()
{
        this->ra = 0;
        this->wa = 0;
        this->ques = "";
//            this->question_number = "";
        this->answers.clear();
}

QString question::questionFormatGift()
{
    QString formated_str = " " + this->ques + "\n{";


    if (this->ra + this->wa > 0) {
        if (this->ra < 1)
        {
            this->answers.push_back("+" + this->answers[0].mid(1,this->answers[0].length()));
            ra++;
        }
        if (this->wa < 1 && type == 0)
        {
            this->answers.push_back("-" + this->answers[0].mid(1,this->answers[0].length()));
        }
        foreach (QString str, this->answers) {
            formated_str += "\n";
            if (type == 1)
            {
                if (str[0] == '+')
                {
                    formated_str += "~%";
                    formated_str += QString::number(100.0 / this->ra);
                    formated_str += "%" + str.right(str.length() - 1);
                }
                else if (str[0] == '-')
                {
                    formated_str += "~";
                    formated_str += str.right(str.length() - 1);
                }
            } else {
                if (str[0] == '+')
                    formated_str += "=" + str.right(str.length() - 1);
                else if (str[0] == '-')
                    formated_str += "~" + str.right(str.length() - 1);
            }
        }
        formated_str += "\n}\n\n";
        return formated_str;
    }
    else return "";
}

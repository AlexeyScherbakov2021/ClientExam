#ifndef EXSMENQUEST_H
#define EXSMENQUEST_H

#include <QList>
#include <QString>


class ExamAnswer
{
public:
    int id;
    int id_eq;
    QString e_value;
    int e_true;
};


class ExamQuest
{
public:
    int id;
    QString question;
    QList<ExamAnswer> listAnswer;
};



#endif // EXSMENQUEST_H

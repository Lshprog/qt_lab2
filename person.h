#ifndef PERSON_H
#define PERSON_H
#include <QList>

class Person
{
public:
    Person(const);

private:
    QList<Person*> children;
    QString names;
    QString proffesion;
    Person
};

#endif // PERSON_H

#ifndef EXTRAMATERIALOPTION_H
#define EXTRAMATERIALOPTION_H

#include <QString>

struct ExtraMaterialOption
{
    QString name;
    QString value;
    QString measure;

    bool isFilled()
    {
        return !(name.isEmpty() || (value == "0"));
    }
};

#endif // EXTRAMATERIALOPTION_H

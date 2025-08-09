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
        return !(name.isEmpty() && value.isEmpty() && measure.isEmpty());
    }
};

#endif // EXTRAMATERIALOPTION_H

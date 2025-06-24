#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>

struct Material
{
public:
    QString name;
    QString measure;
    QString costPrice;
    QString minAmount;
    QString weight;
    QString wasteRate;

    bool isFilled() const
    {
        if (name.isEmpty() ||
            measure.isEmpty() ||
            costPrice.isEmpty() ||
            minAmount.isEmpty() ||
            weight.isEmpty() ||
            wasteRate.isEmpty()) {
            return false;
        } else {
            return true;
        }
    }
};

#endif // MATERIAL_H

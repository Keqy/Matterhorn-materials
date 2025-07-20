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
            costPrice.isEmpty() || costPrice.startsWith("0") ||
            minAmount.isEmpty() || minAmount.startsWith("0") ||
            weight.isEmpty() || weight == "0.00" ||
            wasteRate.isEmpty() || wasteRate == "0.00") {
            return false;
        } else {
            return true;
        }
    }
};

#endif // MATERIAL_H

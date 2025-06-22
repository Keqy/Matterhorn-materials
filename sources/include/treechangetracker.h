#ifndef TREECHANGETRACKER_H
#define TREECHANGETRACKER_H

#include <QString>
#include <QModelIndex>

enum class ChangeType
{
    Delete,
    Update,
    Insert
};

enum class ItemType
{
    Root,
    Category,
    Type
};

struct TreeChange {
    ChangeType changeType;
    QString name;
    QString parentName;
    QString newName; // For Update.
    ItemType itemType;

    TreeChange(ChangeType changeT, QString itemText, QString parentText, ItemType itemT)
        : changeType(changeT), name(itemText), parentName(parentText), itemType(itemT) {}
};

class TreeChangeTracker : public QObject
{
    Q_OBJECT

public:
    explicit TreeChangeTracker(QObject *parent);
    ~TreeChangeTracker() override {};

    void addInsert(const QString &itemName, const QString &parentName, const ItemType &itemType);
    void addDelete(const QString &itemName, const QString &parentName, const ItemType &itemType);
    void addUpdate(const QString &itemName, const QString &parentName, const ItemType &itemType);

    void clear();
    bool hasChanges() const;
    const QList<TreeChange>& getChanges() const { return changes; }

private:
    QList<TreeChange> changes;
};

#endif // TREECHANGETRACKER_H

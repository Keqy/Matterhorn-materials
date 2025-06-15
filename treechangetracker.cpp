#include "treechangetracker.h"

TreeChangeTracker::TreeChangeTracker(QObject *parent) : QObject(parent) {}

void TreeChangeTracker::addInsert(const QString &itemName,
                                  const QString &parentName,
                                  const ItemType &itemType)
{
    TreeChange change(ChangeType::Insert, itemName, parentName, itemType);
    changes.append(change);
}

void TreeChangeTracker::addDelete(const QString &itemName,
                                  const QString &parentName,
                                  const ItemType &itemType)
{
    TreeChange change(ChangeType::Delete, itemName, parentName, itemType);
    changes.append(change);
}

void TreeChangeTracker::addUpdate(const QString &itemName,
                                  const QString &parentName,
                                  const ItemType &itemType)
{
    TreeChange change(ChangeType::Update, itemName, parentName, itemType);
    changes.append(change);
}

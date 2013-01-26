#include "requestitemmodel.h"
#include "requestitem.h"

#include <QDebug>
#include <QStringList>

RequestItemModel::RequestItemModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

RequestItemModel::~RequestItemModel()
{
    while (rowCount() > 0) {
        removeItem(0);
    }
}

int RequestItemModel::rowCount(const QModelIndex &parent) const
{
    return m_items.count();
}

QVariant RequestItemModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case RoleUrl:
        return m_items.at(index.row())->url();
    case RoleData:
        return m_items.at(index.row())->requestData();
    }

    return QVariant();
}

void RequestItemModel::insertItem(RequestItem *item)
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items.append(item);
    endInsertRows();
}

RequestItem *RequestItemModel::item(int row)
{
    return m_items.at(row);
}

bool RequestItemModel::contains(RequestItem *item)
{
    foreach (RequestItem *listItem, m_items) {
        if (item == listItem) {
            return true;
        }
    }
    return false;
}

void RequestItemModel::removeItem(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    delete m_items.takeAt(row);
    endRemoveRows();
}

void RequestItemModel::loadFromSettings(QSettings *settings)
{
    settings->beginGroup("requests");
    for (int i = 0; i < settings->childGroups().count(); ++i) {
        settings->beginGroup(QString::number(i));
        RequestItem *item = new RequestItem();
        item->setUrl(settings->value("url").toUrl());
        item->setRequestData(settings->value("data").toByteArray());
        insertItem(item);
        settings->endGroup();
    }
    settings->endGroup();
}

void RequestItemModel::saveToSettings(QSettings *settings)
{
    settings->beginGroup("requests");
    for (int i = 0; i < m_items.count(); ++i) {
        settings->beginGroup(QString::number(i));
        settings->setValue("url", m_items.at(i)->url());
        settings->setValue("data", m_items.at(i)->requestData());
        settings->endGroup();
    }
    settings->endGroup();
}


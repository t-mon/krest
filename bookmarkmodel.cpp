#include "bookmarkmodel.h"
#include "requestitemmodel.h"

#include <QUrl>
#include <QDebug>
#include <QSettings>
#include <QStringList>

BookmarkModel::BookmarkModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QModelIndex BookmarkModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        if (row < m_categoryList.count()) {
            qDebug() << "index" << row << column << m_categoryList.count();
            return createIndex(row, column, m_categoryList.at(row).second);
        }
        return QModelIndex();
    }
    RequestItemModel *model = m_categoryList.at(parent.row()).second;
    return createIndex(row, column, model->item(row));
}

QModelIndex BookmarkModel::parent(const QModelIndex &child) const
{
    for (int i = 0; i < m_categoryList.count(); ++i) {
        if (m_categoryList.at(i).second == child.internalPointer()) {
            return QModelIndex();
        }
    }

    RequestItem *item = static_cast<RequestItem*>(child.internalPointer());
    for (int i = 0; i < m_categoryList.count(); ++i) {
        if (m_categoryList.at(i).second->contains(item)) {
            return createIndex(i, 0, m_categoryList.at(i).second);
        }
    }
    return QModelIndex();
}

int BookmarkModel::rowCount(const QModelIndex &parent) const
{
    // root item
    if (!parent.isValid()) {
        qDebug() << "rowcount is" << m_categoryList.count();
        return m_categoryList.count();
    }

    // its a bookmark category
    if (!parent.parent().isValid()) {
        RequestItemModel *model = static_cast<RequestItemModel*>(parent.internalPointer());
        return model->rowCount();
    }
    return 0;
}

int BookmarkModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant BookmarkModel::data(const QModelIndex &index, int role) const
{
    if (!index.parent().isValid()) {
        if (role == Qt::DisplayRole) {
            qDebug() << "data" << m_categoryList.at(index.row()).first;
            return m_categoryList.at(index.row()).first;
        }
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        RequestItemModel *model = static_cast<RequestItemModel*>(index.parent().internalPointer());
        return model->data(model->index(index.row(), 0), RequestItemModel::RoleUrl).toString();
    }
    return QVariant();
}

QVariant BookmarkModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && section == 0 && orientation == Qt::Horizontal) {
        return "Bookmarks";
    }
    return QVariant();
}

void BookmarkModel::addBookmark(const QString &name, RequestItem *item)
{
    for (int i = 0; i < m_categoryList.count(); ++i) {
        if (m_categoryList.at(i).first == name) {
            beginInsertRows(index(i, 0, QModelIndex()), m_categoryList.at(i).second->rowCount(), m_categoryList.at(i).second->rowCount());
            qDebug() << "inserting into child model";
            m_categoryList.at(i).second->insertItem(item);
            endInsertRows();
            return;
        }
    }

    beginInsertRows(QModelIndex(), m_categoryList.count(), m_categoryList.count());
    RequestItemModel * model = new RequestItemModel(this);
    model->insertItem(item);
    m_categoryList.append(qMakePair<QString, RequestItemModel*>(name, model));
    endInsertRows();
}

RequestItem *BookmarkModel::item(const QModelIndex &index)
{
    if (!index.parent().isValid()) {
        return 0;
    }
    return m_categoryList.at(index.parent().row()).second->item(index.row());
}

void BookmarkModel::removeItem(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    if (!index.parent().isValid()) {
        qDebug() << "removing" << index.row() << m_categoryList.count();
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        QPair<QString, RequestItemModel*> pair = m_categoryList.takeAt(index.row());
        pair.second->deleteLater();
        endRemoveRows();

        return;
    }

    beginRemoveRows(index.parent(), index.row(), index.row());
    RequestItemModel *model = static_cast<RequestItemModel*>(index.parent().internalPointer());
    qDebug() << "removing" << index.row() << model->rowCount();
    model->removeItem(index.row());
    endRemoveRows();

}

void BookmarkModel::saveToSettings(QSettings *settings)
{
    settings->beginGroup("bookmarks");

    settings->clear();

    for(int i = 0; i < m_categoryList.count(); ++i) {
        settings->beginGroup(QString::number(i));

        settings->setValue("name", m_categoryList.at(i).first);
        m_categoryList.at(i).second->saveToSettings(settings);

        settings->endGroup();
    }

    settings->endGroup();
}

void BookmarkModel::loadFromSettings(QSettings *settings)
{
    settings->beginGroup("bookmarks");

    for (int i = 0; i < settings->childGroups().count(); ++i) {
        settings->beginGroup(QString::number(i));

        QString name = settings->value("name").toString();

        RequestItemModel *model = new RequestItemModel(this);
        model->loadFromSettings(settings);

        m_categoryList.append(qMakePair<QString, RequestItemModel*>(name, model));

        settings->endGroup();
    }
    settings->endGroup();
}

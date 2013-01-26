#ifndef BOOMARKMODEL_H
#define BOOMARKMODEL_H

#include <QAbstractItemModel>

class RequestItem;
class RequestItemModel;

class QSettings;

class BookmarkModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit BookmarkModel(QObject *parent = 0);
    
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addBookmark(const QString &name, RequestItem *item);
    RequestItem *item(const QModelIndex &index);
    void removeItem(const QModelIndex &index);

    void saveToSettings(QSettings *settings);
    void loadFromSettings(QSettings *settings);
private:
    QList< QPair<QString, RequestItemModel*> > m_categoryList;
    
};

#endif // BOOMARKMODEL_H

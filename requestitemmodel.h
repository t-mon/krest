#ifndef REQUESTITEMMODEL_H
#define REQUESTITEMMODEL_H

#include <QAbstractItemModel>
#include <QSettings>

class RequestItem;

class RequestItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        RoleName,
        RoleUrl,
        RoleData
    };

    explicit RequestItemModel(QObject *parent = 0);
    ~RequestItemModel();
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    void insertItem(RequestItem *item);
    RequestItem *item(int row);
    bool contains(RequestItem *item);
    void removeItem(int row);
    
    void loadFromSettings(QSettings *settings);
    void saveToSettings(QSettings *settings);

private:
    QList<RequestItem*> m_items;
    
};

#endif // REQUESTITEMMODEL_H

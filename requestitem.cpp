#include "requestitem.h"

RequestItem::RequestItem()
{
}

QString RequestItem::name() const
{
    return m_name;
}

void RequestItem::setName(const QString &name)
{
    m_name = name;
}

QUrl RequestItem::url() const
{
    return m_url;
}

void RequestItem::setUrl(const QUrl &url)
{
    m_url = url;
}

QByteArray RequestItem::requestData() const
{
    return m_requestData;
}

void RequestItem::setRequestData(const QByteArray &data)
{
    m_requestData = data;
}

QNetworkAccessManager::Operation RequestItem::operation() const
{
    return m_operation;
}

void RequestItem::setOperation(QNetworkAccessManager::Operation operation)
{
    m_operation = operation;
}

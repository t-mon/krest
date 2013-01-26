#include "requestitem.h"

RequestItem::RequestItem()
{
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

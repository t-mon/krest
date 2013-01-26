#ifndef REQUESTITEM_H
#define REQUESTITEM_H

#include <QUrl>

class RequestItem
{
public:
    RequestItem();

    QUrl url() const;
    void setUrl(const QUrl &url);

    QByteArray requestData() const;
    void setRequestData(const QByteArray &data);

private:
    QUrl m_url;
    QByteArray m_requestData;
};

#endif // REQUESTITEM_H

#ifndef REQUESTITEM_H
#define REQUESTITEM_H

#include <QUrl>
#include <QNetworkAccessManager>

class RequestItem
{
public:
    RequestItem();

    QUrl url() const;
    void setUrl(const QUrl &url);

    QByteArray requestData() const;
    void setRequestData(const QByteArray &data);

    QNetworkAccessManager::Operation operation() const;
    void setOperation(QNetworkAccessManager::Operation operation);

private:
    QUrl m_url;
    QByteArray m_requestData;
    QNetworkAccessManager::Operation m_operation;
};

#endif // REQUESTITEM_H

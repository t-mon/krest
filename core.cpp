#include "core.h"

#include "mainwindow.h"
#include "requestitemmodel.h"
#include "requestitem.h"
#include "bookmarkmodel.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QDebug>

Core *Core::s_instance = 0;

Core *Core::instance()
{
    if (s_instance == 0) {
        s_instance = new Core();
        s_instance->init();
    }
    return s_instance;
}

Core::Core(QObject *parent) :
    QObject(parent)
{
}

Core::~Core()
{
    m_bookmarkModel->saveToSettings(m_settings);
}

void Core::init()
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)), SLOT(networkReplyFinished(QNetworkReply*)));

    m_settings = new QSettings("krest", QString(), this);

    m_bookmarkModel = new BookmarkModel(this);
    m_bookmarkModel->loadFromSettings(m_settings);

    qDebug() << "creating mainwindow";
    m_window = new MainWindow();
    m_window->show();

}

BookmarkModel *Core::bookmarks() const
{
    return m_bookmarkModel;
}

void Core::sendRequest(RequestItem *item)
{
    QNetworkRequest request;
    request.setUrl(item->url());

    switch (item->operation()) {
    case QNetworkAccessManager::GetOperation:
        m_nam->get(request);
        break;
    case QNetworkAccessManager::DeleteOperation:
        m_nam->deleteResource(request);
        break;
    case QNetworkAccessManager::PostOperation:
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
//        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        m_nam->post(request, item->requestData());
        break;
    case QNetworkAccessManager::PutOperation:
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
//        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        m_nam->put(request, item->requestData());
        break;
    }
}

void Core::networkReplyFinished(QNetworkReply *reply)
{
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString statusText = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    emit replyReceived(reply->readAll(), QString("%1 %2").arg(QString::number(status)).arg(statusText));
}


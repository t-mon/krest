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

    if (item->operation() == QNetworkAccessManager::GetOperation) {
        m_nam->get(request);
    } else if(item->operation() == QNetworkAccessManager::PostOperation) {

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        m_nam->post(request, item->requestData());
    }
}

void Core::networkReplyFinished(QNetworkReply *reply)
{
    emit replyReceived(reply->readAll());
}


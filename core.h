#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QSettings>
#include <QStringList>

class MainWindow;
class BookmarkModel;
class RequestItem;

class Core : public QObject
{
    Q_OBJECT
public:
    static Core *instance();
    ~Core();

    void init();

    QStringList urlHistory() const;

    BookmarkModel *bookmarks() const;

    void sendRequest(RequestItem *item);
    
signals:
    void replyReceived(const QByteArray &data, const QString &status);

private slots:
    void networkReplyFinished(QNetworkReply *reply);

private:
    MainWindow *m_window;
    
    QNetworkAccessManager *m_nam;

    QSettings *m_settings;

    BookmarkModel *m_bookmarkModel;

private:
    explicit Core(QObject *parent = 0);
    static Core *s_instance;
};
#endif // CORE_H

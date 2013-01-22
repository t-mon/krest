#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

    void networkReplyFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *m_nam;

    QStringList m_dataHistory;
    QStringList m_urlHistory;

    QSettings *m_settings;
};

#endif // MAINWINDOW_H

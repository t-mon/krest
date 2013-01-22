#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>

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
};

#endif // MAINWINDOW_H

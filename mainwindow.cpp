#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)), SLOT(networkReplyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QNetworkRequest request;
    request.setUrl(ui->leAddressl->text());

    if(ui->rbGet->isChecked()) {
        m_nam->get(request);
    } else if(ui->rbPost->isChecked()) {

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        m_nam->post(request, ui->leData->text().toUtf8());
    }
}

void MainWindow::networkReplyFinished(QNetworkReply *reply)
{

    ui->plainTextEdit->setPlainText(reply->readAll());
}


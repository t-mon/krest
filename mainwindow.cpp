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

    m_settings = new QSettings("krest", QString(), this);
    m_urlHistory = m_settings->value("urls").toStringList();
    m_dataHistory = m_settings->value("data").toStringList();

    ui->cbUrl->addItems(m_urlHistory);
    ui->cbData->addItems(m_dataHistory);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString url = ui->cbUrl->currentText();
    QString data = ui->cbData->currentText();

    QNetworkRequest request;
    request.setUrl(url);

    if(ui->rbGet->isChecked()) {
        m_nam->get(request);
    } else if(ui->rbPost->isChecked()) {

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        m_nam->post(request, data.toLatin1());
    }

    if (!m_urlHistory.contains(url)) {
        m_urlHistory.prepend(url);
        ui->cbUrl->insertItem(0, url);

        m_settings->setValue("urls", m_urlHistory);
    }
    if (!m_dataHistory.contains(data)) {
        m_dataHistory.prepend(data);
        ui->cbData->insertItem(0, data);

        m_settings->setValue("data", m_dataHistory);
    }
}

void MainWindow::networkReplyFinished(QNetworkReply *reply)
{

    ui->plainTextEdit->setPlainText(reply->readAll());
}


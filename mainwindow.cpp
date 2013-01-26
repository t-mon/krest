#include "mainwindow.h"
#include "requestitemmodel.h"
#include "requestitem.h"
#include "bookmarkmodel.h"

#include "ui_mainwindow.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("KRest");

    ui->splitter->setStretchFactor(0, 20);
    ui->splitter->setStretchFactor(1, 80);
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)), SLOT(networkReplyFinished(QNetworkReply*)));

    m_settings = new QSettings("krest", QString(), this);
    m_urlHistory = m_settings->value("urls").toStringList();
    m_dataHistory = m_settings->value("data").toStringList();

    ui->cbUrl->addItems(m_urlHistory);
    ui->cbData->addItems(m_dataHistory);

    m_bookmarkModel = new BookmarkModel(this);
    m_bookmarkModel->loadFromSettings(m_settings);

    ui->tvBookmarks->setModel(m_bookmarkModel);

    connect(ui->tvBookmarks, SIGNAL(clicked(QModelIndex)), SLOT(bookmarkClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    m_bookmarkModel->saveToSettings(m_settings);
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

void MainWindow::bookmarkClicked(const QModelIndex &index)
{
    RequestItem *item = m_bookmarkModel->item(index);
    if (item) {

        int i = ui->cbUrl->findText(item->url().toString());
        if (i >= 0) {
            ui->cbUrl->setCurrentIndex(i);
        } else {
            ui->cbUrl->insertItem(0, item->url().toString());
            ui->cbUrl->setCurrentIndex(0);
        }

        i = ui->cbData->findText(item->requestData());
        if (i >= 0) {
            ui->cbData->setCurrentIndex(i);
        } else {
            ui->cbData->insertItem(0, item->requestData());
            ui->cbData->setCurrentIndex(0);
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add Bookmark", "Enter category name", QLineEdit::Normal, QString(), &ok );
    if (ok) {
        RequestItem *item = new RequestItem();
        item->setUrl(ui->cbUrl->currentText());
        item->setRequestData(ui->cbData->currentText().toLocal8Bit());
        m_bookmarkModel->addBookmark(name, item);
    }
}

void MainWindow::on_tvBookmarks_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "*************";
    QPoint globalPos = mapToGlobal(pos);

    QMenu menu;
    QAction *removeAction = menu.addAction("Remove");

    QAction* selectedItem = menu.exec(globalPos);
    if (selectedItem == removeAction)
    {
        QModelIndex index = ui->tvBookmarks->indexAt(pos);
        if (index.isValid()) {
            if (QMessageBox::question(this, "Remove?", "Delete this item and all of its content?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                m_bookmarkModel->removeItem(index);
            }
        }
    }
}

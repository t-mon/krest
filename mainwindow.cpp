#include "mainwindow.h"

#include "core.h"
//#include "requestitemmodel.h"
#include "requestitem.h"
#include "bookmarkmodel.h"

#include "ui_mainwindow.h"
#include "addbookmarkdialog.h"

//#include <QtNetwork/QNetworkRequest>
//#include <QtNetwork/QNetworkReply>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QUrl>

#if QT_VERSION >= 0x050000
  #include <QJsonDocument>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("KRest");

    ui->splitter->setStretchFactor(0, 20);
    ui->splitter->setStretchFactor(1, 80);

    ui->tvBookmarks->setModel(Core::instance()->bookmarks());

    connect(Core::instance(), SIGNAL(replyReceived(QByteArray,QString)), SLOT(replyReceived(QByteArray,QString)));
    connect(ui->tvBookmarks, SIGNAL(clicked(QModelIndex)), SLOT(bookmarkClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::replyReceived(const QByteArray &data, const QString &status)
{
    QByteArray formattedData = data;
#if QT_VERSION >= 0x050000
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);
    if (error.error == QJsonParseError::NoError) {
        formattedData = jsonDoc.toJson();
    }
#endif
    ui->statusLable->setText(status);
    ui->plainTextEdit->setPlainText(formattedData);
}

void MainWindow::on_pushButton_clicked()
{
    QString url = ui->cbUrl->currentText();
    QString data = ui->cbData->currentText();

    RequestItem *item = new RequestItem();
    item->setUrl(QUrl(url));

    if (ui->rbGet->isChecked()) {
        item->setOperation(QNetworkAccessManager::GetOperation);
    } else if (ui->rbDelete->isChecked()) {
        item->setOperation(QNetworkAccessManager::DeleteOperation);
    } else if (ui->rbPost->isChecked()) {
        item->setOperation(QNetworkAccessManager::PostOperation);
        item->setRequestData(data.toLatin1());
    } else if (ui->rbPut->isChecked()) {
        item->setOperation(QNetworkAccessManager::PutOperation);
        item->setRequestData(data.toLatin1());
    }

    Core::instance()->sendRequest(item);

    int index = ui->cbUrl->findText(url);
    if (index >= 0) {
        ui->cbUrl->removeItem(index);
    }
    ui->cbUrl->insertItem(0, url);
    ui->cbUrl->setCurrentIndex(0);

    index = ui->cbData->findText(data);
    if (index >= 0) {
        ui->cbData->removeItem(index);
    }
    ui->cbData->insertItem(0, data);
    ui->cbData->setCurrentIndex(0);
}

void MainWindow::bookmarkClicked(const QModelIndex &index)
{
    RequestItem *item = Core::instance()->bookmarks()->item(index);
    if (item) {

        int i = ui->cbUrl->findText(item->url().toString());
        if (i >= 0) {
            ui->cbUrl->setCurrentIndex(i);
        } else {
            ui->cbUrl->insertItem(0, item->url().toString());
            ui->cbUrl->setCurrentIndex(0);
        }

        switch (item->operation()) {
        case QNetworkAccessManager::GetOperation:
            ui->rbGet->setChecked(true);
            break;
        case QNetworkAccessManager::DeleteOperation:
            ui->rbDelete->setChecked(true);
            break;
        case QNetworkAccessManager::PostOperation:
            ui->rbPost->setChecked(true);
            break;
        case QNetworkAccessManager::PutOperation:
            ui->rbPut->setChecked(true);
            break;
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
    AddBookmarkDialog dialog;
    if(dialog.exec()) {
        RequestItem *item = new RequestItem();
        item->setName(dialog.name());
        item->setUrl(ui->cbUrl->currentText());
        item->setRequestData(ui->cbData->currentText().toLocal8Bit());
        if (ui->rbGet->isChecked()) {
            item->setOperation(QNetworkAccessManager::GetOperation);
        } else if (ui->rbDelete->isChecked()) {
            item->setOperation(QNetworkAccessManager::DeleteOperation);
        } else if (ui->rbPost->isChecked()) {
            item->setOperation(QNetworkAccessManager::PostOperation);
        } else if (ui->rbPut->isChecked()) {
            item->setOperation(QNetworkAccessManager::PutOperation);
        }
        Core::instance()->bookmarks()->addBookmark(dialog.category(), item);
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
                Core::instance()->bookmarks()->removeItem(index);
            }
        }
    }
}

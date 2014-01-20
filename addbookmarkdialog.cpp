#include "addbookmarkdialog.h"
#include "ui_addbookmarkdialog.h"

#include "core.h"
#include "bookmarkmodel.h"

AddBookmarkDialog::AddBookmarkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookmarkDialog)
{
    ui->setupUi(this);

    ui->listView->setModel(Core::instance()->bookmarks());

    connect(ui->listView, SIGNAL(clicked(QModelIndex)), SLOT(selectCategory(QModelIndex)));
}

AddBookmarkDialog::~AddBookmarkDialog()
{
    delete ui;
}

QString AddBookmarkDialog::category() const
{
    return ui->leCategory->text();
}

QString AddBookmarkDialog::name() const
{
    return ui->leName->text();
}

void AddBookmarkDialog::selectCategory(const QModelIndex &index)
{
    ui->leCategory->setText(Core::instance()->bookmarks()->data(index, Qt::DisplayRole).toString());
}

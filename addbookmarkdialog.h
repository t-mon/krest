#ifndef ADDBOOKMARKDIALOG_H
#define ADDBOOKMARKDIALOG_H

#include <QDialog>

namespace Ui {
class AddBookmarkDialog;
}

class AddBookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookmarkDialog(QWidget *parent = 0);
    ~AddBookmarkDialog();

    QString category() const;
    QString name() const;

private slots:
    void selectCategory(const QModelIndex &index);

private:
    Ui::AddBookmarkDialog *ui;
};

#endif // ADDBOOKMARKDIALOG_H

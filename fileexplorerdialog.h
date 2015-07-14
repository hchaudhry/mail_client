#ifndef FILEEXPLORERDIALOG_H
#define FILEEXPLORERDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QTreeView>
#include <QDirModel>

namespace Ui {
class FileExplorerDialog;
}

class FileExplorerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileExplorerDialog(QWidget *parent = 0);
    ~FileExplorerDialog();

    QString getPathResult() {return path;}

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_buttonBox_accepted();

private:
    Ui::FileExplorerDialog *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;

    QString path;
};

#endif // FILEEXPLORERDIALOG_H

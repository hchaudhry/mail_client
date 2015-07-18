#include "fileexplorerdialog.h"
#include "ui_fileexplorerdialog.h"

FileExplorerDialog::FileExplorerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileExplorerDialog)
{
    ui->setupUi(this);

    QString sPath = "/";
    dirmodel = new QFileSystemModel();
    dirmodel->setRootPath(sPath);
    ui->treeView->setModel(dirmodel);
}

FileExplorerDialog::~FileExplorerDialog()
{
    delete ui;
}

void FileExplorerDialog::on_treeView_doubleClicked(const QModelIndex &index)
{
    QModelIndexList list = ui->treeView->selectionModel()->selectedIndexes();
    int row = -1;
    foreach (QModelIndex index, list)
    {
        if (index.row()!=row && index.column()==0)
        {
            path = dirmodel->fileInfo(index).absoluteFilePath();
            row = index.row();
        }
    }

    this->accept();
}

void FileExplorerDialog::on_buttonBox_accepted()
{
    QModelIndexList list = ui->treeView->selectionModel()->selectedIndexes();
    int row = -1;
    foreach (QModelIndex index, list)
    {
        if (index.row()!=row && index.column()==0)
        {
            path = dirmodel->fileInfo(index).absoluteFilePath();
            row = index.row();
        }
    }
}

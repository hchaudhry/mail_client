#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <vector>
#include "mail.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStandardItemModel* tableViewEmails(vector<vector<string>> listMessages);
    void setMail(Mail const & mail);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_sendButton_clicked();

    void on_file_explorer_btn_clicked();

private:
    Ui::MainWindow *ui;
    vector<string> pathes;
    Mail mail;
};

#endif // MAINWINDOW_H

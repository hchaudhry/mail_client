#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <vector>
#include "mail.h"
#include <string>
#include <vector>

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

    QStandardItemModel* tableViewEmails(vector<vector<string> > listMessages);
    void setMail(Mail const & mail);

    void prepareMail();

    void fetchMails(string _user, string _password);
    void mailThread(string userMail, string pwd);
    void runThread(string userMail, string pwd);


private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_sendButton_clicked();

    void on_file_explorer_btn_clicked();

    void on_delete_attachment_clicked();

    void update();

private:
    Ui::MainWindow *ui;
    vector<string> pathes;
    Mail mail;

    string userEmail;
    string userPwd;
    vector<vector<string>> messages;

    QStringList list;
};

#endif // MAINWINDOW_H

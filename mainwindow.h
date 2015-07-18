#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <vector>
#include "mail.h"
#include <string>
#include <vector>

#include <QtSql>

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

    void init();
    void prepareMail();

    void fetchMails(string _user, string _password);
    void mailThread(string userMail, string pwd);
    void runThread(string userMail, string pwd);

    void updateContactList();
    void checkUserExistInDB(QString _userMail);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_sendButton_clicked();

    void on_file_explorer_btn_clicked();

    void on_delete_attachment_clicked();

    void update();

    void on_add_contact_clicked();

    void on_delete_contact_clicked();

    void on_contact_listview_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    vector<string> pathes;
    Mail mail;

    string userEmail;
    string userPwd;
    string smtpServer;
    int smtpPort;
    string popServer;
    int popPort;
    vector<vector<string>> messages;

    QStringList list;

    QSqlDatabase database;
    int idUser;

    QStringList contactList;
};

#endif // MAINWINDOW_H

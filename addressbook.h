#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QMainWindow>

namespace Ui {
class AddressBook;
}

class AddressBook : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddressBook(QWidget *parent = nullptr);
    ~AddressBook();
    void get_dateAndtime(QString &date, QString &time);

private slots:
    void display_dateAndtime();
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void ListSqlite(bool q1);
    void DeleteSqlite();
    void Modify();
    void on_comboBox_textActivated(const QString &arg1);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::AddressBook *ui;
};

#endif // ADDRESSBOOK_H

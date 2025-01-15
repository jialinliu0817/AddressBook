#ifndef ADDNEW_H
#define ADDNEW_H

#include <QMainWindow>

namespace Ui {
class AddNew;
}

class AddNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddNew(QWidget *parent = nullptr);
    ~AddNew();

private slots:
    void on_pushButton_1_clicked(); //browser avtar
    void on_pushButton_2_clicked(); //confirm add
    void on_pushButton_3_clicked(); //close window
    void display_dateAndtime();
    void AddSqlite(QString s1); //add new contact
    void ModifySqlite(QString str); //modify the contact
    void ReceiveName(QString s1); //receive name that need to be modified
    void setName(); //set s1="" when add new contact

private:
    Ui::AddNew *ui;
};

#endif // ADDNEW_H

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
    void modifySqlite();

private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void display_dateAndtime();
    void AddSqlite();

private:
    Ui::AddNew *ui;
};

#endif // ADDNEW_H

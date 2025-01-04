#include "addnew.h"
#include "addressbook.h"
#include "ui_addnew.h"

#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

QString filename;
extern QTimer *timer;
extern QString s1;


AddNew::AddNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddNew)
{
    ui->setupUi(this);
    display_dateAndtime();
    ui->pushButton_1->setShortcut(Qt::Key_Return); //confirm add contact button

    connect(timer,SIGNAL(timeout()),this,SLOT(display_dateAndtime()));
    timer->start(1000);
}

AddNew::~AddNew()
{
    delete ui;
}

void AddNew::display_dateAndtime()
{
    AddressBook e1;
    QString date,time;
    e1.get_dateAndtime(date, time);
    ui->label_1->setText(QString("Date: %1").arg(date));
    ui->lcdNumber->display(time);
}

//add new contact
void AddNew::on_pushButton_1_clicked()
{
    AddSqlite();
}

void AddNew::AddSqlite()
{
    QString name = ui->lineEdit_1->text();
    QString phone = ui->lineEdit_2->text();
    QString email = ui->lineEdit_3->text();
    QString address = ui->lineEdit_4->text();
    AddressBook e1;
    QString date,time;
    e1.get_dateAndtime(date, time);
    date = date + ' ' + time;

    //if s1!= "", then modify the contact where contact name=s1
    if(s1 != "")
    {
        QSqlDatabase a1;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
        {
            a1 = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            a1 = QSqlDatabase::addDatabase("QSQLITE");

        }
        a1.setDatabaseName("addressbook.sqlite3");
        a1.setPassword("sa");
        a1.open();

        QSqlQuery b1;
        b1.exec(QString("Update contact "
                        "SET phone='%1', email='%2', address='%3' "
                        "Where name='%4'").arg(phone, email, address,name));
        if(!filename.isEmpty())
        {
            QStringList r1 = filename.split("/");
            QStringList r2 = r1.last().split(".");
            QString r3 = r2.last();
            QFile currentfile(filename);

            QFileInfo f1("./pic/");
            if(!f1.isDir())
            {
                QDir *path = new QDir;
                path->mkpath("./pic/");
            }

            currentfile.copy(QString("./pic/%1.%2").arg(name, r3));
        }
        ui->label_7->setText("successfully modified!");
    }
    else if(name == "" || phone == "")
    {
        ui->label_7->setText("name or phone can't be null!!");
    }
    else
    {
        QSqlDatabase a1;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
        {
            a1 = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            a1 = QSqlDatabase::addDatabase("QSQLITE");

        }
        a1.setDatabaseName("addressbook.sqlite3");
        a1.setPassword("sa");
        a1.open();

        QSqlQuery b1;
        b1.exec("Select * From contact");

        if(!b1.first())
        {
            b1.exec("Create Table contact"
                    "(name text primary key not null, "
                    "phone text unique not null, "
                    "email text,"
                    "address text,"
                    "date text)");
        }

        b1.exec(QString("Select * "
                        "From contact "
                        "Where phone = '%1' ").arg(phone));

        if(b1.first())
        {
            ui->label_7->setText("name and phone exists!!");
            ui->label_7->setStyleSheet("background: green");
        }
        else
        {
            QString contact = QString("'%1', '%2', '%3', '%4', '%5'").arg(name, phone, email, address, date);
            b1.exec(QString("Insert Into contact"
                            "(name, phone, email, address, date)"
                            "Values(%1)").arg(contact));

            if(!filename.isEmpty())
            {
                QStringList r1 = filename.split("/");
                QStringList r2 = r1.last().split(".");
                QString r3 = r2.last();
                QFile currentfile(filename);

                QFileInfo f1("./pic/");
                if(!f1.isDir())
                {
                    QDir *path = new QDir;
                    path->mkpath("./pic/");
                }

                currentfile.copy(QString("./pic/%1.%2").arg(name, r3));
            }
            ui->label_7->setText("successfully added!");
        }

        a1.close();
    }
}

//turn off addnew contact window
void AddNew::on_pushButton_2_clicked()
{
    this->close();
}

void AddNew::on_pushButton_3_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "Please choose an avatar", "*.png; *.jpg");
    if(!filename.isEmpty())
    {
        ui->lineEdit_5->setText(filename);
    }
}

void AddNew::modifySqlite()
{
    QSqlDatabase a1;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        a1 = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        a1 = QSqlDatabase::addDatabase("QSQLITE");

    }
    a1.setDatabaseName("addressbook.sqlite3");
    a1.setPassword("sa");
    a1.open();

    QSqlQuery b1;
    b1.exec(QString("Select * From contact where name = '%1' ").arg(s1));

    while(b1.next())
    {
        QString name = b1.value(0).toString();
        QString phone = b1.value(1).toString();
        QString email = b1.value(2).toString();
        QString address = b1.value(3).toString();
        ui->lineEdit_1->setText(name);
        ui->lineEdit_2->setPlaceholderText(phone);
        ui->lineEdit_3->setPlaceholderText(email);
        ui->lineEdit_4->setPlaceholderText(address);
    }
    a1.close();
}

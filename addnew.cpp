#include "addnew.h"
#include "ui_addnew.h"
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

QString filename;
QString s2;
extern QTimer *timer;

AddNew::AddNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddNew)
{
    ui->setupUi(this);
    display_dateAndtime();
    ui->pushButton_3->setShortcut(Qt::Key_Return); //confirm add contact button
    connect(timer,SIGNAL(timeout()),this,SLOT(display_dateAndtime()));
    timer->start(1000);
}

AddNew::~AddNew()
{
    delete ui;
}

void AddNew::display_dateAndtime()
{
    QString date,time;
    QDateTime t1 = QDateTime::currentDateTime();
    date = t1.date().toString();
    time = t1.time().toString();
    ui->label_1->setText(QString("Date: %1").arg(date));
    ui->lcdNumber->display(time);
}

void AddNew::AddSqlite(QString s1)
{
    QString name = ui->lineEdit_1->text();
    QString phone = ui->lineEdit_2->text();
    QString email = ui->lineEdit_3->text();
    QString address = ui->lineEdit_4->text();
    QString date,time;
    QDateTime t1 = QDateTime::currentDateTime();
    date = t1.date().toString();
    time = t1.time().toString();
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
        QString b2 = QString("Update contact\n"
                             "SET phone='%1', email='%2', address='%3' "
                             "Where name='%4'").arg(phone, email, address, name);

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

        if(b1.exec(b2))
        {
            ui->label_8->setFont(QFont("Source Code Pro",12));
            ui->label_8->setStyleSheet("background: green");
            ui->label_8->setText("successfully modified!");
        }
    }
    else if(name == "" || phone == "")
    {
        ui->label_8->setFont(QFont("Source Code Pro",12));
        ui->label_8->setStyleSheet("background: red");
        ui->label_8->setText("name or phone can't be null!!");
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
                    "(name text not null primary key, "
                    "phone text not null, "
                    "email text,"
                    "address text,"
                    "date text)");
        }

        b1.exec(QString("Select * "
                        "From contact "
                        "Where name = '%1' and phone = '%2'").arg(name, phone));

        if(b1.first())
        {
            ui->label_8->setFont(QFont("Source Code Pro",12));
            ui->label_8->setText("name and phone exists!!");
            ui->label_8->setStyleSheet("background: green");
        }
        else
        {
            QString contact = QString("'%1', '%2', '%3', '%4', '%5'").arg(name, phone, email, address, date);
            QString b2 = QString("Insert Into contact\n"
                                 "(name, phone, email, address, date) "
                                 "Values(%1)").arg(contact);

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

            if(b1.exec(b2))
            {
                ui->label_8->setFont(QFont("Source Code Pro",12));
                ui->label_8->setStyleSheet("background: green");
                ui->label_8->setText("successfully added!");
            }
        }

        a1.close();
    }
}

//choose avatar
void AddNew::on_pushButton_1_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "Please choose an avatar", "*.png; *.jpg");

    QImage p1;
    p1.load(filename);
    p1 = p1.scaled(160, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_7->setPixmap(QPixmap::fromImage(p1));
}

//add new contact
void AddNew::on_pushButton_2_clicked()
{
    AddSqlite(s2);
}

//clear window
void AddNew::on_pushButton_3_clicked()
{
    ui->lineEdit_1->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->label_7->clear();
    ui->label_8->clear();
    ui->label_8->setStyleSheet("background: rgb(255,255,255)");
}

void AddNew::ModifySqlite(QString s1)
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
        ui->lineEdit_2->setText(phone);
        ui->lineEdit_3->setText(email);
        ui->lineEdit_4->setText(address);

        QFileInfo t1(QString("./pic/%1.jpg").arg(name));
        if(t1.isFile())
        {
            QImage p1;
            p1.load(QString("./pic/%1.jpg").arg(s1));
            p1 = p1.scaled(160, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_7->setPixmap(QPixmap::fromImage(p1));
            ui->label_8->clear();
            ui->label_8->setStyleSheet("background: rgb(255,255,255)");
        }
        t1.setFile(QString("./pic/%1.png").arg(name));
        if(t1.isFile())
        {
            QImage p1;
            p1.load(QString("./pic/%1.jpg").arg(s1));
            p1 = p1.scaled(160, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_7->setPixmap(QPixmap::fromImage(p1));
            ui->label_8->clear();
            ui->label_8->setStyleSheet("background: rgb(255,255,255)");
        }
    }
    a1.close();
}

void AddNew::ReceiveName(QString s1)
{
    s2 = s1;
}

void AddNew::setName()
{
    s2 = "";
}

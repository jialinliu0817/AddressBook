#include "addnew.h"
#include "ui_addnew.h"
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

QString filename;  //old avatar path
QString filename1; //new avatar dir
QString s2;    //contact name that need to modify
bool stat1 = false;  //sucess modify or add if stat1 = true
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

//add or modify contact
void AddNew::AddSqlite(QString s2)
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

    //if s2!= ""(contact exist already), then modify the contact where contact name = s2
    if(s2 != "")
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

        QDir absDir(filename);
        QString oldAvatar = filename;
        filename = absDir.absolutePath();
        qDebug()<<"filename: "<<filename;
        qDebug()<<"filename1: "<<filename1;
        if(!filename1.isEmpty())
        {
            QStringList r1 = filename1.split("/"); //new chosen avatar dir
            QString com1=""; //new chosen avatar dir to compare
            QStringList r2 = r1.last().split(".");
            QString r3 = r2.last(); //new avatar suffix
            r1.removeLast();

            for(int i=0;i<r1.count();i++)
            {
                com1 = com1+r1.at(i)+"/";
            }
            qDebug()<<"com1: "<<com1;

            r1 = filename.split("/");     //old avatar dir
            QString com="";
            r1.removeLast();

            for(int i=0;i<r1.count();i++)
            {
                com = com+r1.at(i)+"/";
            }
            qDebug()<<"com: "<<com;

            if(com == com1)    //old and new avatar in same Dir
            {
                QFileInfo FileInfo(oldAvatar);

                if(FileInfo.isFile())    //delete old avatar
                {
                    QFile::remove(oldAvatar);
                }

                QFile currentfile(filename1);
                currentfile.copy(QString("./pic/%1.%2").arg(name, r3));  //copy new avatar
            }
            else  //old and new avatar not in same Dir
            {
                QFileInfo FileInfo(oldAvatar);

                if(FileInfo.isFile())    //delete old avatar
                {
                    QFile::remove(oldAvatar);
                }

                QFile currentfile(filename1);
                QFileInfo f1("./pic/");

                if(!f1.isDir())
                {
                    QDir *path = new QDir;
                    path->mkpath("./pic/");
                }
                currentfile.copy(QString("./pic/%1.%2").arg(name, r3));
            }
        }

        if(b1.exec(b2))
        {
            stat1 = true;
            filename1 = "";
        }
    }
    else if(name == "" || phone == "")    //check if follow adding rules when s2 doesn't exist
    {
        ui->label_8->setFont(QFont("Source Code Pro",12));
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

        if(!b1.first())  //if contact is empty, then create a new contact table
        {
            b1.exec("Create Table contact"
                    "(name text not null primary key, "
                    "phone text not null, "
                    "email text,"
                    "address text,"
                    "date text)");
        }
        else    //(contact table exist)query to find if name or phone exist
        {
            b1.exec(QString("Select * "
                            "From contact "
                            "Where name = '%1' and phone = '%2'").arg(name, phone));
        }

        if(b1.first())  //check if contact already exist
        {
            ui->label_8->setFont(QFont("Source Code Pro",12));
            ui->label_8->setText("name and phone exists!!");
        }
        else  //contact doesn't exist, then add new contact
        {
            QString contact = QString("'%1', '%2', '%3', '%4', '%5'").arg(name, phone, email, address, date);
            QString b2 = QString("Insert Into contact\n"
                                 "(name, phone, email, address, date) "
                                 "Values(%1)").arg(contact);

            if(!filename1.isEmpty())
            {
                QStringList r1 = filename1.split("/");
                QStringList r2 = r1.last().split(".");
                QString r3 = r2.last();  //suffix of new contact avatar
                QFile currentfile(filename1);

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
                stat1 = true;
                filename1 = "";
            }
        }

        a1.close();
    }
}

//choose avatar
void AddNew::on_pushButton_1_clicked()
{
    filename1 = QFileDialog::getOpenFileName(this, "Please choose an avatar", "*.png; *.jpg");

    QImage p1;
    p1.load(filename1);
    p1 = p1.scaled(160, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_7->setPixmap(QPixmap::fromImage(p1));
    ui->label_8->setFont(QFont("Source Code Pro",12));
    ui->label_8->setText(filename1);
}

//add new contact when s2="", modify contact when s2!=""
void AddNew::on_pushButton_2_clicked()
{
    AddSqlite(s2);
    if(stat1)
    {
        setName();  //after modify, set s2 = ""
        emit SwitchPage();  //after modify or add contact, return to main page
    }
}

//clear add new contact window
void AddNew::on_pushButton_3_clicked()
{
    if(s2 == "")
    {
        ui->lineEdit_1->clear();
    }
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->label_7->clear();
    ui->label_8->clear();
    ui->label_8->setStyleSheet("background: rgb(255,255,255)");
}

void AddNew::ModifySqlite(QString s2)
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
    b1.exec(QString("Select * From contact where name = '%1' ").arg(s2));

    while(b1.next())
    {
        QString name = b1.value(0).toString();
        QString phone = b1.value(1).toString();
        QString email = b1.value(2).toString();
        QString address = b1.value(3).toString();
        ui->lineEdit_1->setText(name);
        ui->lineEdit_1->setReadOnly(true);
        ui->lineEdit_2->setText(phone);
        ui->lineEdit_3->setText(email);
        ui->lineEdit_4->setText(address);

        QString path = "./pic";
        QDir dir(path);
        if(!dir.exists())
        {
            return;
        }
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QStringList filters;
        filters << QString("%1.png").arg(s2) << QString("%1.jpg").arg(s2);
        dir.setNameFilters(filters);
        QStringList list = dir.entryList();
        if (list.count() <= 0)
        {
            return;
        }
        qDebug()<<"old avatar list: "<<list;

        QStringList r2 = list.last().split(".");
        QString r3 = r2.last();  //old contact avatar suffix
        QFileInfo t1;
        filename = QString("./pic/%1.%2").arg(name, r3);
        t1.setFile(filename);
        if(t1.isFile())
        {
            QImage p1;
            p1.load(filename);
            p1 = p1.scaled(160, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_7->setPixmap(QPixmap::fromImage(p1));
            ui->label_8->setFont(QFont("Source Code Pro",12));
            ui->label_8->setText(filename);
            ui->label_8->setStyleSheet("background: rgb(255,255,255)");
        }
    }
    a1.close();
}

void AddNew::ReceiveName(QString s1)
{
    s2 = s1; //receive contact name value from addressbook page
}

void AddNew::setName()
{
    s2 = "";
    ui->lineEdit_1->clear();
    ui->lineEdit_1->setReadOnly(false);
}

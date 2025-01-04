#include "addressbook.h"
#include "ui_addressbook.h"
#include "addnew.h"
#include "QDateTime"
#include <qtimer.h>
#include <QMenu>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QStyle>
#include <QScrollBar>


QString s1 = "";
QString a1; //sort order
QTimer *timer = new QTimer();

AddressBook::AddressBook(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddressBook)
{
    ui->setupUi(this);
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: #d0d0d0;}");
    display_dateAndtime();
    ListSqlite(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(display_dateAndtime()));
    timer->start(1000);
}

AddressBook::~AddressBook()
{
    delete ui;
}

void AddressBook::get_dateAndtime(QString &date, QString &time)
{
    QDateTime t1 = QDateTime::currentDateTime();
    date = t1.date().toString();
    time = t1.time().toString();
}

void AddressBook::display_dateAndtime()
{
    QString date,time;
    get_dateAndtime(date, time);
    ui->label_1->setText(QString("Date: %1").arg(date));
    ui->lcdNumber->display(time);
}

//update sort in time(a1=1) or alphabet(a1=0)
void AddressBook::on_pushButton_1_clicked()
{

    a1 = ui->comboBox->currentText();
    if(a1 == "Time")
    {
        ListSqlite(1);
    }
    else ListSqlite(0);

}

//add new contact
void AddressBook::on_pushButton_2_clicked()
{
    AddNew *w1 = new AddNew;
    w1->show();
}

//query the contacts for someone
void AddressBook::on_pushButton_3_clicked()
{
    QInputDialog m1;
    m1.setWindowTitle("Search for Contact");
    m1.setLabelText("Input a name:");
    m1.exec();
    s1 = m1.textValue();
    m1.setAttribute(Qt::WA_DeleteOnClose);

    qint8 j = 0;
    QString res1;
    bool f1 = true;
    while(f1)
    {
        for(int i = 0; i < ui->listWidget->count();i++)
        {
            QString s2 =  ui->listWidget->item(i)->text();
            QStringList s3 = s2.split("\n");
            QString s4 = s3.at(1); //s4= (name: name)
            QString s5 = s3.at(0); //s5= (contact, index)
            QStringList s6 = s4.split(":");
            s4 = s6.last().simplified();  //name search for
            s6 = s5.split(":");
            s5 = s6.last().simplified();  //index

            if(s1 == s4)
            {
                res1 = s2;
                f1 = false;
                if(!f1) break;
            }
        }
    }

    QMessageBox m2;
    m2.setWindowTitle("contact found");
    m2.setText(res1);
    QPixmap p1(QString("./pic/%1.jpg").arg(s1));
    //160,140
    p1 = p1.scaled(160, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m2.setIconPixmap(p1);
    m2.exec();
}

//turn off app
void AddressBook::on_pushButton_4_clicked()
{
    this->close();
}

void AddressBook::on_comboBox_textActivated(const QString &arg1)
{
    QFileInfo f1("./pic/");
    if(!f1.isDir())
    {
        QDir *path = new QDir;
        path->mkpath("./pic/");
    }

    if(arg1 == "Time")
        ListSqlite(1);
    else ListSqlite(0);
}

void AddressBook::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu  *m1 = new QMenu(ui->listWidget);

    m1->setStyleSheet("QMenu{background-color: rgb(255,255,255); border-radius: 5px;}\
                       QMenu::item{border-bottom: 1px solid rgb(0,0,0); border-radius: 5px; background-color: #ffffff;}\
                       QMenu::item:hover,QMenu::item:selected{background-color: rgb(104,104,104);}");

    QFont f1;
    f1.setPointSize(12);
    m1->setFont(f1);
    QAction  *d1 = new QAction(tr("     Delete       "), this);
    QAction  *mod = new QAction(tr("     Modify       "), this);

    connect(d1, SIGNAL(triggered()), this, SLOT(DeleteSqlite()));
    connect(mod, SIGNAL(triggered()), this, SLOT(Modify()));

    m1->addAction(d1);
    m1->addAction(mod);
    m1->popup(ui->listWidget->mapToGlobal(pos));
}

void AddressBook::ListSqlite(bool q1)
{
    ui->listWidget->clear();
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

    QSqlQuery b1(a1);
    if(q1)
    {
        b1.exec("Select * From contact order by date ASC");
    }
    else
    {
        b1.exec("Select * From contact order by name ASC");
    }

    qint64 i=1;
    while(b1.next())
    {
        QString name = b1.value(0).toString();
        QString phone = b1.value(1).toString();
        QString email = b1.value(2).toString();
        QString address = b1.value(3).toString();
        QString date = b1.value(4).toString();


        QString a = QString("contact: %1").arg(i) + '\n'
                    +QString("name: %1   ").arg(name) + '\n'
                    +QString("phone: %1 ").arg(phone) + '\n'
                    +QString("e-mail: %1").arg(email) + '\n'
                    +QString("address: %1").arg(address)+ '\n'
                    +QString("date: %1").arg(date);

        ++i;

        QListWidgetItem *add_item = new QListWidgetItem(ui->listWidget);
        add_item->setSizeHint(QSize(400,140));
        add_item->setFont(QFont("Source Code Pro",14));

        QFileInfo t1(QString("./pic/%1.png").arg(name));
        if(t1.isFile())
        {
            add_item->setIcon(QIcon(QString("./pic/%1.png").arg(name)));
        }

        t1.setFile(QString("./pic/%1.jpg").arg(name));
        if(t1.isFile())
        {
            add_item->setIcon(QIcon(QString("./pic/%1.jpg").arg(name)));
        }

        ui->listWidget->setIconSize(QSize(160,140));
        add_item->setText(a);

        ui->listWidget->addItem(add_item);
        ui->listWidget->setStyleSheet("QListWidget{background:rgb(255,255,255); border: 2px solid #fff; border-radius: 5px}"
                                      "QListWidget::Item{background: rgb(255,255,255); border-radius: 5px; border: 1px solid rgb(198, 184, 255);}"
                                      "QListWidget::Item:hover,QListWidget::Item:selected{background: rgb(205,205,205);}");
        ui->listWidget->setSpacing(1);

    }
    a1.close();
}

void AddressBook::DeleteSqlite()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    foreach(QListWidgetItem* var, items)
    {
        s1 = var->text();
        QStringList s2 = s1.split("\n");
        s1 = s2.at(1);
        s2 = s1.split(":");
        s1 = s2.last().simplified();
    }


    QSqlDatabase a1;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        a1 = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        a1 = QSqlDatabase::addDatabase("SQLITECIPHER");

    }
    a1.setDatabaseName("addressbook.sqlite3");
    a1.setPassword("sa");
    a1.open();

    QSqlQuery b1;
    b1.exec(QString("Delete From contact where name = '%1' ").arg(s1));

    a1.close();

    on_pushButton_1_clicked();
}

void AddressBook::Modify()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    foreach(QListWidgetItem* var, items)
    {
        s1 = var->text();
        QStringList s2 = s1.split("\n");
        s1 = s2.at(1);
        s2 = s1.split(":");
        s1 = s2.last().simplified();
    }

    AddNew *w = new AddNew;
    w->setWindowTitle("Modify Contact");
    w->modifySqlite();
    w->show();
}

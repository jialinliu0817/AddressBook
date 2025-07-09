#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addressbook.h"
#include "addnew.h"

enum eStackedWidgetID{eForm1 = 0,eForm2,eForm3};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AddressBook *adb1 = new AddressBook();
    AddNew *an1 = new AddNew();

    ui->stackedWidget->insertWidget(eForm1,adb1);
    ui->stackedWidget->insertWidget(eForm2,an1);
    ui->stackedWidget->setCurrentIndex(eForm1);

    connect(adb1, SIGNAL(SwitchPage()), this, SLOT(SwitchPage2()));
    connect(adb1, SIGNAL(sendText(QString)), an1, SLOT(ReceiveName(QString))); //s1 receive modify contact name
    connect(adb1, SIGNAL(sendText(QString)), an1, SLOT(ModifySqlite(QString))); //modify contact, to modify page
    connect(an1, SIGNAL(SwitchPage()), this, SLOT(SwitchPage1()));


    QAction *p1 = new QAction(ui->menuMenu);
    p1->setText(tr("Home"));
    p1->setIcon(QIcon(":/icon/home.png"));
    ui->menuMenu->addAction(p1);

    QAction *p2 = new QAction(ui->menuMenu);
    p2->setText(tr("Add new Contact"));
    p2->setIcon(QIcon(":/icon/add.png"));
    ui->menuMenu->addAction(p2);

    connect(p1,SIGNAL(triggered()), this,SLOT(SwitchPage1()));
    connect(p1,SIGNAL(triggered()), an1,SLOT(setName()));

    connect(p2,SIGNAL(triggered()), an1,SLOT(on_pushButton_3_clicked())); //clear add new window when add new contact
    connect(p2,SIGNAL(triggered()), an1,SLOT(setName()));  // set s1="" when add new contact
    connect(p2,SIGNAL(triggered()), this,SLOT(SwitchPage2()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SwitchPage1()
{
    ui->stackedWidget->setCurrentIndex(eForm1);
}

void MainWindow::SwitchPage2()
{
    ui->stackedWidget->setCurrentIndex(eForm2);
}


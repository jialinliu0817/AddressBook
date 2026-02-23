#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addressbook.h"
#include "addnew.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

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

    // About menu
    QAction *a1 = new QAction(ui->menuAbout);
    a1->setText(tr("About AddressBook"));
    a1->setIcon(QIcon(":/icon/addressbook.png"));
    ui->menuAbout->addAction(a1);

    QAction *a2 = new QAction(ui->menuAbout);
    a2->setText(tr("Latest Release"));
    ui->menuAbout->addAction(a2);

    connect(a1, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(a2, SIGNAL(triggered()), this, SLOT(showRelease()));
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

void MainWindow::showAbout()
{
    QMessageBox mb(this);
    mb.setWindowTitle(tr("About AddressBook"));
    mb.setWindowIcon(QIcon(":/icon/addressbook.png"));
    mb.setIconPixmap(QIcon(":/icon/addressbook.png").pixmap(64, 64));
    mb.setTextFormat(Qt::RichText);
    mb.setText(
        "<h2 style='margin:0;'>AddressBook</h2>"
        "<hr/>"
        "<p><b>Version:</b> Qt6 (tag: Qt)</p>"
        "<p><b>Published:</b> 2025-01-15</p>"
        "<p><b>Environment:</b> Windows 10 &nbsp;|&nbsp; Qt 6.8.1 &nbsp;|&nbsp; SQLite3</p>"
        "<p><b>Description:</b><br/>"
        "A desktop address book application built with Qt6.<br/>"
        "Supports adding, editing, deleting and searching contacts,<br/>"
        "with avatar support and time/alphabet sorting.</p>"
        "<p><b>Repository:</b><br/>"
        "<a href='https://github.com/jialinliu0817/AddressBook'>"
        "https://github.com/jialinliu0817/AddressBook</a></p>"
    );
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

void MainWindow::showRelease()
{
    QDesktopServices::openUrl(
        QUrl("https://github.com/jialinliu0817/AddressBook/releases/latest"));
}

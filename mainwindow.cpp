#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addressbook.h"
#include "addnew.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QActionGroup>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

enum eStackedWidgetID{eForm1 = 0,eForm2,eForm3};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_adb = new AddressBook(this);  // ownership transferred to stackedWidget via insertWidget
    m_an  = new AddNew(this);

    ui->stackedWidget->insertWidget(eForm1, m_adb);
    ui->stackedWidget->insertWidget(eForm2, m_an);
    ui->stackedWidget->setCurrentIndex(eForm1);

    connect(m_adb, SIGNAL(SwitchPage()), this, SLOT(SwitchPage2()));
    connect(m_adb, SIGNAL(sendText(QString)), m_an, SLOT(ReceiveName(QString)));
    connect(m_adb, SIGNAL(sendText(QString)), m_an, SLOT(ModifySqlite(QString)));
    connect(m_an,  SIGNAL(SwitchPage()), this, SLOT(SwitchPage1()));

    // ── Menu menu ──────────────────────────────────────────────────────
    QAction *p1 = new QAction(ui->menuMenu);
    p1->setText(tr("Home"));
    p1->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::GoHome));
    ui->menuMenu->addAction(p1);

    QAction *p2 = new QAction(ui->menuMenu);
    p2->setText(tr("Add new Contact"));
    p2->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ContactNew));
    ui->menuMenu->addAction(p2);

    connect(p1, SIGNAL(triggered()), this, SLOT(SwitchPage1()));
    connect(p1, SIGNAL(triggered()), m_an, SLOT(setName()));

    connect(p2, SIGNAL(triggered()), m_an, SLOT(on_pushButton_3_clicked()));
    connect(p2, SIGNAL(triggered()), m_an, SLOT(setName()));
    connect(p2, SIGNAL(triggered()), this, SLOT(SwitchPage2()));

    // ── View menu ──────────────────────────────────────────────────────
    QAction *vOpacity = new QAction(ui->menuView);
    vOpacity->setText(tr("Opacity..."));
    vOpacity->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOriginal));
    ui->menuView->addAction(vOpacity);

    ui->menuView->addSeparator();

    QAction *vLight = new QAction(ui->menuView);
    vLight->setText(tr("Light Mode"));
    vLight->setCheckable(true);
    vLight->setChecked(true);
    vLight->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::WeatherClear));

    QAction *vDark = new QAction(ui->menuView);
    vDark->setText(tr("Dark Mode"));
    vDark->setCheckable(true);
    vDark->setChecked(false);
    vDark->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::WeatherClearNight));

    QActionGroup *themeGroup = new QActionGroup(this);
    themeGroup->setExclusive(true);
    themeGroup->addAction(vLight);
    themeGroup->addAction(vDark);

    ui->menuView->addAction(vLight);
    ui->menuView->addAction(vDark);

    connect(vOpacity, &QAction::triggered, this, &MainWindow::showOpacityDialog);
    connect(vLight,   &QAction::triggered, this, [this](){ applyTheme(false); });
    connect(vDark,    &QAction::triggered, this, [this](){ applyTheme(true); });

    // ── About menu ─────────────────────────────────────────────────────
    QAction *a1 = new QAction(ui->menuAbout);
    a1->setText(tr("About AddressBook"));
    a1->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
    ui->menuAbout->addAction(a1);

    QAction *a2 = new QAction(ui->menuAbout);
    a2->setText(tr("Latest Release"));
    a2->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::SoftwareUpdateAvailable));
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
    mb.setWindowIcon(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
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
        "AddressBook</a></p>"
    );
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

void MainWindow::showRelease()
{
    QDesktopServices::openUrl(
        QUrl("https://github.com/jialinliu0817/AddressBook/releases/latest"));
}

void MainWindow::showOpacityDialog()
{
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Window Opacity"));
    dlg.setFixedSize(320, 130);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setSpacing(12);
    layout->setContentsMargins(20, 16, 20, 16);

    int currentPct = qRound(this->windowOpacity() * 100);
    QLabel *label = new QLabel(tr("Opacity: %1%").arg(currentPct), &dlg);
    label->setAlignment(Qt::AlignCenter);

    QSlider *slider = new QSlider(Qt::Horizontal, &dlg);
    slider->setRange(20, 100);
    slider->setValue(currentPct);

    QPushButton *ok = new QPushButton(tr("OK"), &dlg);

    connect(slider, &QSlider::valueChanged, this, [this, label](int value){
        this->setWindowOpacity(value / 100.0);
        label->setText(tr("Opacity: %1%").arg(value));
    });
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);

    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(ok);

    dlg.exec();
}

void MainWindow::applyTheme(bool dark)
{
    m_darkMode = dark;

    // Apply to menu bar / title bar via app stylesheet
    if (dark) {
        qApp->setStyleSheet(
            "QMenuBar { background-color: #1E1B2E; color: #E5E7EB; }"
            "QMenuBar::item:selected { background-color: #3B3870; border-radius: 4px; }"
            "QMenu { background-color: #1E1B2E; color: #E5E7EB; "
            "        border: 1px solid #3B3870; border-radius: 8px; padding: 4px; }"
            "QMenu::item { padding: 6px 20px 6px 10px; border-radius: 4px; }"
            "QMenu::item:hover, QMenu::item:selected "
            "    { background-color: #3B3870; color: #A78BFA; }"
            "QMenu::separator { background-color: #3B3870; height: 1px; margin: 4px 8px; }"
            "QDialog { background-color: #1E1B2E; color: #E5E7EB; }"
            "QLabel  { color: #E5E7EB; }"
            "QSlider::groove:horizontal { background: #3B3870; height: 4px; border-radius: 2px; }"
            "QSlider::handle:horizontal { background: #7C3AED; width: 14px; height: 14px; "
            "    border-radius: 7px; margin: -5px 0; }"
            "QPushButton { background-color: #7C3AED; color: #FFFFFF; border: none; "
            "    border-radius: 6px; padding: 4px 16px; }"
            "QPushButton:hover { background-color: #6D28D9; }"
        );
    } else {
        qApp->setStyleSheet("");
    }

    if (m_adb) m_adb->applyTheme(dark);
    if (m_an)  m_an->applyTheme(dark);
}

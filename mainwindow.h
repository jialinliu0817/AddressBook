#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class AddressBook;
class AddNew;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void SwitchPage1();  //addressbook home
    void SwitchPage2();  //add new contact window

private slots:
    void showAbout();           //show about dialog
    void showRelease();         //open latest release page
    void showOpacityDialog();   //opacity adjustment dialog
    void applyTheme(bool dark); //apply light/dark theme

private:
    Ui::MainWindow *ui;
    AddressBook *m_adb = nullptr;
    AddNew *m_an = nullptr;
    bool m_darkMode = false;
};

#endif // MAINWINDOW_H

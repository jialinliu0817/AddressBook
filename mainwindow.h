#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

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
    void showAbout();    //show about dialog
    void showRelease();  //open latest release page

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

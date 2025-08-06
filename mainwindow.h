#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QDebug>


#include <funcs/scr.h>
#include <funcs/modes_area.h>



QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow;}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event); // свёртка в трей

private slots:
    // слот нажатия на иконку приложения в трее
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void openArea();

    void trayCheckBoxONFFMouse_stateChanged(int arg1);

    void on_radioButtonSCR_clicked();

    void on_radioButtonVID_clicked();

    void on_pushButtonScreenCapture_2_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    // объявление объекта будущей иконки для трея
    Screenshot *screenshotWidget;
    modes_area *areaShow;
};

#endif // MAINWINDOW_H

#include <QStyle>
#include <QMenu>
#include <QTime>
#include <QTimer>
#include <QSettings>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./funcs/ui_areacapture.h"


#define ORGANIZATION_NAME "EVILEG"
#define ORGANIZATION_DOMAIN "www.evileg.ru"
#define APPLICATION_NAME "QSettings Program"
#define SETTINGS_TRAY "settings/tray"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Xrecorder");

    //this->setWindowFlags(Qt::FramelessWindowHint); // скрыли шапку формы
    //this->setAttribute(Qt::WA_TranslucentBackground); // сделали форму прозрачной
    //this->setAttribute(Qt::WA_TransparentForMouseEvents);



    // инициализация иконки трея, установка иконки из
    // набора системных иконок, задаем всплывающую подсказку

    trayIcon = new QSystemTrayIcon(this);
    //trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setIcon(QIcon(":/new/"
                            "prefix1_buttons_circle/"
                            "pictures/"
                            "Xrecorder_astra_vers2 (1) (1).png"));
    trayIcon->setToolTip("Xrecorder");

    // контекстное меню из двух пунктов

    QMenu *menu = new QMenu(this);
    //QAction *viewWindow = new QAction(tr("Развернуть окно"), this);
    QAction *quitAction = new QAction(tr("Выход"), this);
    //QAction *settingsAction = new QAction(tr("Настройки"), this);

    /* сделать подменю для настроек */


    // подключаем сигналы нажатий на пункты меню к соответствующим слотам
    // п.1 меню разворачивает приложение из трея
    // п.2 меню завершает приложение

    //connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    //menu->addAction(viewWindow);
    menu->addAction(quitAction);

    // устанавливаем контекстное меню на иконку и показываем ее в трее

    trayIcon->setContextMenu(menu);
    trayIcon->show();

    // подключаем сигнал нажатия на иконку к обработчику данного нажатия

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));


    screenshotWidget = new Screenshot();

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    ui->trayCheckBoxONFFMouse->setChecked(settings.value(SETTINGS_TRAY, false).toBool());

}



MainWindow::~MainWindow()
{
    delete ui;
}



// метод обработки события закрытия окна приложения

void MainWindow::closeEvent(QCloseEvent *event)
{
    // если окно видимо и чекбокс отмечен, то завершение приложения
    // игнорируется, а окно просто скрывается, что сопровождается
    // соответствующим всплывающим сообщением

    if (this->isVisible() /*&& ui->trayCheckBox->isChecked()*/)
    {
        event->ignore();
        this->hide();
        QSystemTrayIcon::MessageIcon icon =
                QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("Xrecorder",
                              tr("Приложение свернуто в трей. "
       "Для того, чтобы развернуть окно приложения,"
                                 " щелкните по иконке приложения в трее"),
                              icon, 1500);
    }
}



void MainWindow::openAreaCapture()
{
    areaShow = new AreaCapture(this);
    areaShow->showFullScreen();
    qDebug() << "Window visible" << areaShow->isVisible();
    QTime dieTime = QTime::currentTime().addSecs(7);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    areaShow->close();
    /*areaShow->setWindowTitle("NewArea");
    areaShow->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    areaShow->setParent(0);
    areaShow->setAttribute(Qt::WA_NoSystemBackground, true);
    areaShow->setAttribute(Qt::WA_TranslucentBackground, true);
    areaShow->setAttribute(Qt::WA_PaintOnScreen);
    setGeometry(QGuiApplication::primaryScreen()->geometry());
    areaShow->setStyleSheet("background-color: rgba(0, 0, 0, 150);");*/
    //areaShow->show();
    /*QTime dieTime = QTime::currentTime().addSecs(7);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    areaShow->close();*/
}


// метод обработки нажатия на иконку приложения в трее

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        // событие игнориуется, если чекбокс не отмечен
        /*if (ui->trayCheckBox->isChecked())
        {*/
            // если окно видимо, то оно сворачивается
            // иначе - если скрыто, то разворачивается на экран
            if (!this->isVisible())
            {
                this->show();
            } else {
                this->hide();
            }
        //}
        break;
    default:
        break;
    }
}


// логика срабатывания захвата экрана


//  !
void MainWindow::trayCheckBoxONFFMouse_stateChanged(int arg1) {

    // чек-бокс запоминание предыдущего состояния (показ/скрытие мыши)
    // !
    // не проверено
    // нужно ставить deb-пакетом и смотреть за реакцией
    // !

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    if (ui->trayCheckBoxONFFMouse->isChecked()) {
        settings.setValue(SETTINGS_TRAY, true);
    } else {
        settings.setValue(SETTINGS_TRAY, false);
    }
    settings.sync();
}
//  !



//  !

void MainWindow::on_radioButtonSCR_clicked()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    if (ui->radioButtonSCR->isChecked()) {
        settings.setValue(SETTINGS_TRAY, true);
    } else {
        settings.setValue(SETTINGS_TRAY, false);
    }
    settings.sync();
}

//  !

void MainWindow::on_radioButtonVID_clicked()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    if (ui->radioButtonVID->isChecked()) {
        settings.setValue(SETTINGS_TRAY, true);
    } else {
        settings.setValue(SETTINGS_TRAY, false);
    }
    settings.sync();
}

//  !



void MainWindow::on_pushButtonScreenCapture_2_clicked()
{
    if (ui->radioButtonSelectedArea->isChecked() && ui->radioButtonSCR->isChecked()) {
        AreaSelector selector;
        selector.exec();
        QPixmap capturedArea = selector.getSelectedArea();
        this->hide();
        int checkState = ui->trayCheckBoxONFFMouse->checkState();
        selector.saveScreenshot(capturedArea/*, checkState*/);
        this->show();

    } else if (ui->radioButtonFullArea->isChecked() && ui->radioButtonSCR->isChecked()) {

        // сделать дефолтным в чекбоксе (при запуске)

        this->hide();
        int checkState = ui->trayCheckBoxONFFMouse->checkState();
        QRect fullScreenRect = QGuiApplication::primaryScreen()->geometry();
        screenshotWidget->shootScreen(fullScreenRect, checkState);
        this->show();
    }

    else if (ui->radioButtonVID->isChecked()) {
        //
        //
        //
    }


    qDebug() << "button is working";
}

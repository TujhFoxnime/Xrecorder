#include "areacapture.h"
#include "ui_areacapture.h"
#include <QScreen>

AreaCapture::AreaCapture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AreaCapture)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color: rgba(0, 0, 0, 150);");
    //setParent(nullptr);
    setGeometry(QGuiApplication::primaryScreen()->geometry());
    setWindowModality(Qt::ApplicationModal);
    setWindowState(Qt::WindowFullScreen);
    //setAttribute(Qt::WA_PaintOnScreen);
    setParent(nullptr);
}

AreaCapture::~AreaCapture()
{
    delete ui;
}

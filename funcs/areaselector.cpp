#include "areaselector.h"

#include <QGuiApplication>

AreaSelector::AreaSelector(QWidget *parent) : QWidget(parent)
{
    //
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    //
    QScreen *screen = QGuiApplication::primaryScreen();
    setGeometry(screen->geometry());

    //
    screenCapture = screen->grabWindow(0);

    showFullScreen();
    setCursor(Qt::CrossCursor);
}






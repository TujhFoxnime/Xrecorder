#ifndef AREASELECTOR_H
#define AREASELECTOR_H

#include <QWidget>
#include <QPixmap>
#include <QScreen>
#include <QStandardPaths>
#include <QDialog>

/*#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>*/

#include "drawcursorcheckbox.h"



class AreaSelector : public QDialog
{
    Q_OBJECT
public:
    explicit AreaSelector(QWidget *parent = nullptr);
    QPixmap getSelectedArea(/*QPixmap screen, int arg*/);
    void saveScreenshot(QPixmap screen/*, int arg*/);
    void getCursorOnArea(int arg); // protected
    QPixmap getCursorOnArea(const QRect& rect, int arg);
    void setCheckState(int state);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;  

protected slots:
    //paintEvent(QPaintEvent *event);

private:
    QPoint startPos;
    QPoint endPos;
    bool selecting = false;
    QPixmap screenCapture;
    QPixmap selectedArea;
    int currentCheckState;
};

#endif // AREASELECTOR_H

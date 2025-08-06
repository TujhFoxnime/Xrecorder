#ifndef VID_H
#define VID_H


#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include <QScreen>


class Videoshot : public QWidget {
    Q_OBJECT

public:
    Videoshot();

protected:
    //void resizeEvent(QResizeEvent *event) override;

public slots:
    //void newVideoshot();
    //void saveVideoshot();
    //QPixmap shootVideo(const QRect& rect, int arg);
    //void updateCheckBox();
    //void showCursor();
    void CaptureScreenImage();

private:

    QTimer *Timer;

    //void updateVideoshotLabel();

    //QPixmap originalPixmap;

    //QLabel *screenshotLabel;
    //QSpinBox *delaySpinBox;
    //QCheckBox *hideThisWindowCheckBox;
    //QPushButton *newVideoshotButton;
};





#endif // VID_H

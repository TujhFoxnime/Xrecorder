#ifndef SCR_VID_H
#define SCR_VID_H

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include <QScreen>


class Screenshot : public QWidget {
    Q_OBJECT

public:
    Screenshot();

protected:
    //void resizeEvent(QResizeEvent *event) override;

public slots:
    void newScreenshot();
    void saveScreenshot();
    QPixmap shootScreen(const QRect& rect, int arg);
    void updateCheckBox();
    void showCursor();

private:
    void updateScreenshotLabel();

    QPixmap originalPixmap;

    QLabel *screenshotLabel;
    QSpinBox *delaySpinBox;
    QCheckBox *hideThisWindowCheckBox;
    QPushButton *newScreenshotButton;
};

#endif // SCR_VID_H

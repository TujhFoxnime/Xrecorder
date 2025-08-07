#ifndef AREASELECTOR_H
#define AREASELECTOR_H

#include <QWidget>
#include <QPixmap>


class AreaSelector : public QWidget
{
    Q_OBJECT
public:
    explicit AreaSelector(QWidget *parent = nullptr);
    QPixmap getSelectedArea() const;

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
};

#endif // AREASELECTOR_H

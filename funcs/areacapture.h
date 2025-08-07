#ifndef AREACAPTURE_H
#define AREACAPTURE_H

#include <QDialog>

namespace Ui {
class AreaCapture;
}

class AreaCapture : public QDialog
{
    Q_OBJECT

public:
    explicit AreaCapture(QWidget *parent = nullptr);
    ~AreaCapture();

private:
    Ui::AreaCapture *ui;
};

#endif // AREACAPTURE_H

#ifndef MODES_AREA_H
#define MODES_AREA_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class modes_area;
}

class modes_area : public QDialog
{
    Q_OBJECT

public:
    explicit modes_area(QWidget *parent = 0);

    ~modes_area();

public slots:


private:
    Ui::modes_area *ui;
};

#endif // MODES_AREA_H

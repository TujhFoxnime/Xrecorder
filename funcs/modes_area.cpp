#include "modes_area.h"



modes_area::modes_area() : QDialog(parent), ui(new Ui::modes_area)
{


    ui->setupUi(this);
}


modes_area::~modes_area()
{
    delete ui;
}





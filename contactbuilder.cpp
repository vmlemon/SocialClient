#include "contactbuilder.h"
#include "ui_contactbuilder.h"

ContactBuilder::ContactBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactBuilder)
{
    ui->setupUi(this);
}

ContactBuilder::~ContactBuilder()
{
    delete ui;
}

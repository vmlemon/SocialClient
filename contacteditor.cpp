#include "contacteditor.h"
#include "ui_contacteditor.h"

ContactEditor::ContactEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactEditor)
{
    ui->setupUi(this);
}

ContactEditor::~ContactEditor()
{
    delete ui;
}

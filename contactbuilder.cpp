#include "contactbuilder.h"
#include "ui_contactbuilder.h"

#include <contact.h>
#include <QColorDialog>
#include <QDebug>

ContactBuilder::ContactBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactBuilder),
    iColour("")
{
    ui->setupUi(this);
}

ContactBuilder::~ContactBuilder()
{
    delete ui;
}

void ContactBuilder::on_buttonBox_accepted()
{
    Contact *tempContact = new Contact();
    tempContact->SetUid(tempContact->CountStoredContacts() + 1);
}

void ContactBuilder::on_SelectColour_clicked()
{
    QColorDialog *qcd = new QColorDialog();
    iColour = qcd->getColor().name();

}

#include "contactbuilder.h"
#include "ui_contactbuilder.h"

#include <contact.h>
#include <QColorDialog>
#include <QDebug>
#include <QColor>
#include <QLineEdit>

#include <Parsers/skype.h>

ContactBuilder::ContactBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactBuilder),
    iColour("")
{
    ui->setupUi(this);
    this->setWindowTitle("Create New Contact");
    qDebug() << iColour.length();
}

ContactBuilder::~ContactBuilder()
{
    delete ui;
}

void ContactBuilder::on_buttonBox_accepted()
{
    Contact *tempContact = new Contact();

    if (tempContact->EntryZeroExists() == false) {
        tempContact->SetUid(0);
    }

    if (tempContact->EntryZeroExists() == true && tempContact->CountStoredContacts() >=1){

       /* Nasty Hack */
            tempContact->SetUid(tempContact->CountStoredContacts() + 1 - 1);
    }


    if (iColour.length() != 0) {
        tempContact->SetVersion(1);
        tempContact->SetStatusColour(iColour);
    }

    else

    {
        tempContact->SetVersion(0);
    }

    tempContact->SetForename(ui->Forename->text());
    tempContact->SetSurname(ui->Surname->text());
    tempContact->SetEMailAddress(ui->EMail->text());
    tempContact->SetTwitterUrl(ui->TwitterHandle->text());
    tempContact->SetSkypeUserName(ui->SkypeHandle->text());
    tempContact->SetLastFmUserName(ui->LastFmHandle->text());
    tempContact->WriteContactFile();

    delete tempContact;
}

void ContactBuilder::on_SelectColour_clicked()
{
    QColorDialog *qcd = new QColorDialog();
    QColor colour = qcd->getColor();

    iColour = colour.name();
    qDebug() << iColour;

    if (!iColour.contains("#000000")) {
        ui->SelectColour->setStyleSheet("* { background-color:" +  iColour + "}");
    }

    else {
        ui->SelectColour->setStyleSheet("");
    }

    delete qcd;
}

void ContactBuilder::on_SkypeHandle_textEdited(const QString &aText)
{
    if (aText.length() != 0) {
        ui->SkypeHandle->setStyleSheet("* { background-color:" + Skype::GetOnlineColour() + "}");
    }

    else {
        ui->SkypeHandle->setStyleSheet("");
    }

    if (ui->CopyUserNames->isChecked()) {
     ui->TwitterHandle->setText(aText);
     ui->LastFmHandle->setText(aText);
    }
}

void ContactBuilder::on_TwitterHandle_textEdited(const QString &aText)
{
    if (ui->CopyUserNames->isChecked()) {
     ui->SkypeHandle->setText(aText);
     ui->LastFmHandle->setText(aText);
    }
}

void ContactBuilder::on_LastFmHandle_textEdited(const QString &aText)
{
    if (ui->CopyUserNames->isChecked()) {
     ui->SkypeHandle->setText(aText);
     ui->TwitterHandle->setText(aText);
    }
}

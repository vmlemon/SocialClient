#include "contacteditor.h"
#include "ui_contacteditor.h"
#include <Parsers/skype.h>
#include <QDebug>

ContactEditor::ContactEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactEditor)
{
    ui->setupUi(this);
    qint64 count = iTempContact->CountStoredContacts() + 1 - 1;
    ui->UidCount->setText("/" + QString::number(count));
    ui->UidField->setMaxLength(QString::number(count).length());
}

ContactEditor::~ContactEditor()
{
    delete ui;
}

void ContactEditor::on_UidField_textChanged(const QString &aText)
{
    iTempContact = new Contact();
    iTempContact->ReadContactFile(aText.toLongLong());
    ui->ForenameField->setText(iTempContact->GetForename());
    ui->SurnameField->setText(iTempContact->GetSurname());
    ui->EMailField->setText(iTempContact->GetEMailAddress());
    ui->SkypeUsernameField->setText(iTempContact->GetSkypeUserName());

    ui->SkypeStatusLabel->setText(Skype::ParseSkypeStatus(Skype::ReadFromCache(ui->SkypeUsernameField->text())));
    qDebug() << iTempContact->GetSkypeStatus();

}

void ContactEditor::on_buttonBox_accepted()
{
    if (ui->UidField->text().length() !=0) {
        iTempContact->SetForename(ui->ForenameField->text());
        iTempContact->SetSurname(ui->SurnameField->text());
        iTempContact->SetEMailAddress(ui->EMailField->text());
        iTempContact->SetSkypeUserName(ui->SkypeUsernameField->text());
        iTempContact->SetSkypeStatus(Skype::ReadFromCache(ui->SkypeUsernameField->text()).toInt());

        qDebug() << "Going to write: " << iTempContact->Serialise();

        iTempContact->WriteContactFile();
    }

    else {
        delete this;
    }

}

void ContactEditor::on_buttonBox_rejected()
{
    delete this;
}

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

    ui->SkypeStatusLabel->setText(Skype::ParseSkypeStatus(
                                      QString::number(iTempContact->GetSkypeStatus(aText.toLongLong()))));
    qDebug() << iTempContact->GetSkypeStatus();

}

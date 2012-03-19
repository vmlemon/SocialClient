#include "contacteditor.h"
#include "ui_contacteditor.h"
#include <Parsers/skype.h>
#include <Parsers/lastfm.h>
#include <QDebug>

ContactEditor::ContactEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactEditor)
{
    ui->setupUi(this);
    qint64 count = iTempContact->CountStoredContacts() + 1 - 1;
    ui->UidCount->setText("/" + QString::number(count));
    ui->UidField->setMaxLength(QString::number(count).length());
    ui->UidField->setText("0");
    ui->PreviousButton->setEnabled(false);
    ui->UidField->setVisible(false);
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
    ui->LastFmUserNameField->setText(iTempContact->GetLastFmUserName());

    ui->SkypeStatusLabel->setText(Skype::ParseSkypeStatus(Skype::ReadFromCache(ui->SkypeUsernameField->text())));
    qDebug() << iTempContact->GetSkypeStatus();

    ui->LastFmLastTrackLabel->setText(
                LastFm::GetLastFmLatestTrack(
                    LastFm::ReadFromCache(ui->LastFmUserNameField)));

    if (aText == "0" || aText.contains("-")) {
        ui->PreviousButton->setEnabled(false);
        ui->UidField->setText("0");
        ui->UidCount->setText("0/" + QString::number(iTempContact->CountStoredContacts()));
    }

    else {
        ui->UidCount->setText(aText + "/" + QString::number(iTempContact->CountStoredContacts()));
    }

    if (aText == QString::number(iTempContact->CountStoredContacts())) {
        ui->NextButton->setEnabled(false);
    }

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

void ContactEditor::on_PreviousButton_clicked()
{
    if (iTempContact->CountStoredContacts() == 0) {
        ui->PreviousButton->setEnabled(false);
    }

    else {

        if (ui->UidField->text() == "1") {
            ui->UidField->setText("0");
        }

        if (ui->UidField->text() == "0" || ui->UidField->text().contains("-")) {
            ui->PreviousButton->setEnabled(false);
            ui->UidField->setText("0");
        }

        if (ui->UidField->text() != "0" || ui->UidField->text() != "1") {
            ui->PreviousButton->setEnabled(true);
            ui->UidField->setText(QString::number(iTempContact->GetUid() - 1));
        }


    }
}

void ContactEditor::on_NextButton_clicked()
{
    if (iTempContact->CountStoredContacts() != 0) {
       ui->UidField->setText(QString::number(iTempContact->GetUid() + 1));
       ui->PreviousButton->setEnabled(true);
    }

    if (ui->UidField->text() == QString::number(iTempContact->CountStoredContacts())) {
        ui->UidField->setText("0");
        ui->PreviousButton->setEnabled(false);
        ui->NextButton->setEnabled(true);
    }

}

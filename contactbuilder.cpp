#include "contactbuilder.h"
#include "ui_contactbuilder.h"

#include <QColorDialog>
#include <QDebug>
#include <QColor>
#include <QLineEdit>
#include <QClipboard>
#include <QMimeData>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QUrl>

#include <Parsers/skype.h>

ContactBuilder::ContactBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactBuilder),
    iColour("")
{
    ui->setupUi(this);

    QString windowTitle = "Create New Contact (" +
            QString::number(iTempContact->CountStoredContacts() + 1 - 1) +
            ")";

    this->setWindowTitle(windowTitle);

    qDebug() << iColour.length();

    QClipboard *clipboard = QApplication::clipboard();

    if (clipboard->mimeData(QClipboard::Clipboard)->data("SkypeIdentityList").length() != 0) {
        QByteArray mimeData = clipboard->mimeData(QClipboard::Clipboard)->data("SkypeIdentityList");
        ui->SkypeHandle->setText(Skype::ParseClipboardData(mimeData));
    }

    ui->SkypeHandle->setAcceptDrops(true);
}

ContactBuilder::~ContactBuilder()
{
    delete ui;
}

void ContactBuilder::on_buttonBox_accepted()
{
    iTempContact = new Contact();

    if (iTempContact->EntryZeroExists() == false) {
        iTempContact->SetUid(0);
    }

    if (iTempContact->EntryZeroExists() == true){

       /* Nasty Hack */
            iTempContact->SetUid(iTempContact->CountStoredContacts() + 1 - 1);
    }


    if (iColour.length() != 0) {
        iTempContact->SetVersion(1);
        iTempContact->SetStatusColour(iColour);
    }

    else

    {
        iTempContact->SetVersion(0);
    }

    iTempContact->SetForename(ui->Forename->text());
    iTempContact->SetSurname(ui->Surname->text());
    iTempContact->SetEMailAddress(ui->EMail->text());
    iTempContact->SetTwitterUrl(ui->TwitterHandle->text());
    iTempContact->SetSkypeUserName(ui->SkypeHandle->text());
    iTempContact->SetLastFmUserName(ui->LastFmHandle->text());
    iTempContact->WriteContactFile();

    delete iTempContact;
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
        Skype::GetStatusColour(aText);
        ui->SkypeHandle->setStyleSheet("* { background-color:" + Skype::GetStatusColour(aText) + "}");
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

/* Listen for DragEnter events */

void ContactBuilder::dragEnterEvent(QDragEnterEvent *aEvent) {
    if (aEvent->mimeData()->hasText() ||
            aEvent->mimeData()->hasUrls() ||
            aEvent->mimeData()->hasFormat("SkypeIdentityList") ||
            aEvent->mimeData()->hasFormat("UniformResourceLocator") ||
            aEvent->mimeData()->hasFormat("UniformResourceLocatorW")) {
        aEvent->acceptProposedAction();
    }
}

void ContactBuilder::dropEvent(QDropEvent *aEvent) {

    if (aEvent->mimeData()->hasFormat("SkypeIdentityList")) {

        QByteArray mimeData = aEvent->mimeData()->data("SkypeIdentityList");

        ui->SkypeHandle->setText(Skype::ParseClipboardData(mimeData));
    }

    if (aEvent->mimeData()->hasFormat("UniformResourceLocatorW")) {

        QByteArray mimeData = aEvent->mimeData()->data("UniformResourceLocatorW");
        QString rawData = Skype::ParseClipboardData(mimeData);

        qDebug() << rawData;

        if (rawData.contains("http://twitter.com/#!/")) {
            ui->TwitterHandle->setText(rawData.remove("http://twitter.com/#!/"));
        }
    }

    if (aEvent->mimeData()->hasFormat("UniformResourceLocator")) {

        QByteArray mimeData = aEvent->mimeData()->data("UniformResourceLocator");
        QString rawData = mimeData;

        qDebug() << rawData;

        if (rawData.contains("http://twitter.com/#!/")) {
            ui->TwitterHandle->setText(rawData.remove("http://twitter.com/#!/"));
        }
    }

    if (aEvent->mimeData()->hasUrls() == true) {

        QString firstRawUrl = aEvent->mimeData()->urls().first().toString();

        if (firstRawUrl.contains("http://www.last.fm/user")) {
            ui->LastFmHandle->setText(firstRawUrl.remove("http://www.last.fm/user/"));
        }

        if (firstRawUrl.contains("mailto:")) {
            ui->EMail->setText(firstRawUrl.remove("mailto:"));
        }
    }

    else if (aEvent->mimeData()->hasText() == true) {
        QString rawText = aEvent->mimeData()->text();

        if (rawText.contains("http://twitter.com/#!/")) {
            ui->TwitterHandle->setText(rawText.remove("http://twitter.com/#!/"));
        }
    }

}

#include "contactbuilder.h"
#include "ui_contactbuilder.h"

#include <QColorDialog>
#include <QDialog>
#include <QDebug>
#include <QColor>
#include <QLineEdit>
#include <QClipboard>
#include <QMimeData>
#include <QDropEvent>
#include <QVariant>
#include <QDragEnterEvent>
#include <QUrl>
//#include <QWindowsMime>
#include <Parsers/skype.h>
#include <Parsers/pidgin.h>
#include <Parsers/twitter.h>

/* Hack to enable loading in a contact... */
ContactBuilder::ContactBuilder(QWidget *parent, int aContactId):
    QDialog(parent),
    ui(new Ui::ContactBuilder),
    iColour("")
{
    qDebug() << "Working with contact ID: " << aContactId;

}

void ContactBuilder::PrepareCbData() {
    QClipboard *clipboard = QApplication::clipboard();

    qDebug() << "Got clipboard data. Data formats are: " << clipboard->mimeData()->formats();

    if (clipboard->mimeData(QClipboard::Clipboard)->data("SkypeIdentityList").length() != 0) {
        QByteArray mimeData = clipboard->mimeData(QClipboard::Clipboard)->data("SkypeIdentityList");
        ui->SkypeHandle->setText(Skype::ParseClipboardData(mimeData));
    }

    ui->SkypeHandle->setAcceptDrops(true);
    this->setAcceptDrops(true);
}

void ContactBuilder::UpdateWinTitle(QString aTitle) {
    this->setWindowTitle(aTitle);
}



/* Default case constructor (Create) */
ContactBuilder::ContactBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactBuilder),
    iColour("")
{

    ui->setupUi(this);
    UpdateWinTitle("Create New Contact (" +
                   QString::number(iTempContact->CountStoredContacts() + 1 - 1) +
                   ")");

    qDebug() << iColour.length();

    PrepareCbData();
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

/* Listen for DragEnter events */

void ContactBuilder::dragEnterEvent(QDragEnterEvent *aEvent) {

    qDebug() << "Got a dragEnter event. Data formats are: " << aEvent->mimeData()->formats();

//    if (aEvent->mimeData()->hasText() ||
//            aEvent->mimeData()->hasUrls() ||
//            aEvent->mimeData()->hasFormat("SkypeIdentityList") ||
//            aEvent->mimeData()->hasFormat("UniformResourceLocator")||
//            aEvent->mimeData()->hasFormat("FileGroupDescriptor")){
        aEvent->acceptProposedAction();
//    }
}

void ContactBuilder::dropEvent(QDropEvent *aEvent) {

    qDebug() << "Got a drop event. Data formats are: " << aEvent->mimeData()->formats();

    if (aEvent->mimeData()->hasFormat("SkypeIdentityList")) {
        QByteArray mimeData = aEvent->mimeData()->data("SkypeIdentityList");
        ui->SkypeHandle->setText(Skype::ParseClipboardData(mimeData));
    }

    if (aEvent->mimeData()->hasFormat("UniformResourceLocator")) {
        qDebug() << "UniformResourceLocator";
        QByteArray mimeData = aEvent->mimeData()->data("UniformResourceLocator");
        QString rawData = Poach(mimeData);
        DisperseUri(rawData);
    }

    if (aEvent->mimeData()->hasColor()) {
        iColour = aEvent->mimeData()->text();
        ui->SelectColour->setStyleSheet("* { background-color:" + iColour + "}");
        qDebug() << aEvent->mimeData()->colorData();

    }

    if (aEvent->mimeData()->hasFormat("application/x-im-contact")) {
        qDebug() << "application/x-im-contact";
        QByteArray mimeData = aEvent->mimeData()->data("application/x-im-contact");
        qDebug() << Pidgin::GetXImContactProtocol(mimeData) << Pidgin::GetXImContactUsername(mimeData);

        if (Pidgin::GetXImContactProtocol(mimeData).contains("msn")) {
            ui->EMail->setText(Pidgin::GetXImContactUsername(mimeData));
        }

        if (Pidgin::GetXImContactUsername(mimeData).contains("@gmail.com")) {
            ui->EMail->setText(Pidgin::GetXImContactUsername(mimeData));
        }
    }

    /* Hack for Empathy - needs updating later */
    if (aEvent->mimeData()->hasFormat("text/individual-id")) {
        QByteArray mimeData = aEvent->mimeData()->data("text/individual-id");
        if (QString(mimeData).contains("telepathy:jabber:")) {
            ui->EMail->setText(QString(mimeData).remove("telepathy:jabber:"));
        }

        qDebug() << mimeData;
    }

    if (aEvent->mimeData()->hasUrls() == true) {
        QString firstRawUrl = aEvent->mimeData()->urls().first().toString();
         qDebug() << "hasUrls()" << firstRawUrl;
        DisperseUri(firstRawUrl);
    }

    else if (aEvent->mimeData()->hasText() == true) {
        qDebug() << "hasText()";
        QString rawText = Poach(aEvent->mimeData()->text());
        DisperseUri(rawText);
    }
}

void ContactBuilder::DisperseUri(QString aUri) {

    qDebug() << "Dispersing" << aUri;

    if (aUri.contains("http://twitter.com") || aUri.contains("https://twitter.com")) {
        ui->TwitterHandle->setText(Poach(aUri));
    }

    if (aUri.contains("http://www.last.fm/user")) {
        ui->LastFmHandle->setText(Poach(aUri.remove("http://www.last.fm/user/")));
    }

    if (aUri.contains("mailto:")) {
        ui->EMail->setText(Poach(aUri.remove("mailto:")));
    }

    if (aUri.contains("skype:")) {
        ui->SkypeHandle->setText(aUri.remove("skype:"));
    }

}

QString ContactBuilder::Poach(QString aUri) {

    QString poached = aUri;

    if (poached.contains("http://twitter.com") || poached.contains("https://twitter.com")) {
        qDebug() << "Poached a Twitter URL" << Twitter::ReduceUrl(poached);
        poached = Twitter::ReduceUrl(poached);

        qDebug() << "Poacher has seen" << poached;

        return poached.simplified();
    }

    else {
        qDebug() << "Poacher has seen" << aUri.simplified();
        return poached.simplified();
    }
}

void ContactBuilder::on_TwitterHandle_textChanged(const QString &aText)
{
    if (ui->CopyUserNames->isChecked()) {
     ui->SkypeHandle->setText(aText);
     ui->LastFmHandle->setText(aText);
    }
}

void ContactBuilder::on_SkypeHandle_textChanged(const QString &aText)
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

void ContactBuilder::on_LastFmHandle_textChanged(const QString &aText)
{
    if (ui->CopyUserNames->isChecked()) {
     ui->SkypeHandle->setText(aText);
     ui->TwitterHandle->setText(aText);
    }
}

void ContactBuilder::on_buttonBox_rejected()
{
    delete this;
}

void ContactBuilder::on_EditExistingButton_toggled(bool aChecked)
{
    if (aChecked) {
        ui->CopyUserNames->setCheckState(Qt::Unchecked);
        ui->CopyUserNames->setEnabled(false);
    }
}

void ContactBuilder::on_CreateNewButton_toggled(bool aChecked)
{
    if (aChecked) {
        ui->CopyUserNames->setCheckState(Qt::Unchecked);
        ui->CopyUserNames->setEnabled(true);
    }
}

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
#include <QDragEnterEvent>
#include <QUrl>
#include <QWindowsMime>
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

    qDebug() << "Got clipboard data. Data formats are: " << clipboard->mimeData()->formats();

    if (clipboard->mimeData(QClipboard::Clipboard)->data("SkypeIdentityList").length() != 0) {
        QByteArray mimeData = clipboard->mimeData(QClipboard::Clipboard)->data("SkypeIdentityList");
        ui->SkypeHandle->setText(Skype::ParseClipboardData(mimeData));
    }

    ui->SkypeHandle->setAcceptDrops(true);
    this->setAcceptDrops(true);

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

/* Internet Explorer 9 drags
  ("application/x-qt-windows-mime;value="UntrustedDragDrop"",
 "application/x-qt-windows-mime;value="msSourceUrl"",
 "application/x-qt-windows-mime;value="FileGroupDescriptor"",
 "application/x-qt-windows-mime;value="FileGroupDescriptorW"",
 "application/x-qt-windows-mime;value="FileContents"", "text/uri-list")
 */


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

    if (aEvent->mimeData()->hasFormat("application/x-im-contact")) {
        qDebug() << "application/x-im-contact";
        QByteArray mimeData = aEvent->mimeData()->data("application/x-im-contact");
        qDebug() << mimeData;
    }

    /* Pidgin
Got a dragEnter event. Data formats are:  ("PURPLE_ACCOUNT")
Got a dragEnter event. Data formats are:  ("PURPLE_BLIST_NODE", "application/x-im-contact", "text/x-vcard")
*/

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

    if (aUri.contains("http://twitter.com")) {
        ui->TwitterHandle->setText(Poach(aUri));
    }

    if (aUri.contains("http://www.last.fm/user")) {
        ui->LastFmHandle->setText(Poach(aUri.remove("http://www.last.fm/user/")));
    }

    if (aUri.contains("mailto:")) {
        ui->EMail->setText(Poach(aUri.remove("mailto:")));
    }
}

QString ContactBuilder::Poach(QString aUri) {

    QString poached = aUri;

    if (poached.contains("http://twitter.com/#!/")) {
        qDebug() << "Poached a Twitter HashURL" <<  poached.remove("http://twitter.com/#!/");
        poached = poached.remove("http://twitter.com/#!/");

    /* Safari hack */

        qDebug() << poached.indexOf("\n\r") << poached.indexOf(" ");
        poached.chop(poached.indexOf(" "));
        poached.chop(poached.indexOf("@"));
        poached.chop(poached.indexOf("/"));
        poached.chop(poached.indexOf(" "));
        poached.chop(poached.trimmed().indexOf("/ "));

        qDebug() << "Poacher has seen" << poached;

        return poached.simplified();
    }

    if (poached.contains("http://twitter.com/")) {
        qDebug() << "Poached a legacy Twitter URL" <<  poached.remove("http://twitter.com/");
        poached = poached.remove("http://twitter.com/");

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

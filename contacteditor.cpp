#include "contacteditor.h"
#include "ui_contacteditor.h"

ContactEditor::ContactEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactEditor)
{
    ui->setupUi(this);
    qint64 count = iTempContact->CountStoredContacts() + 1 - 1;
    ui->UidCount->setText(QString::number(count));
}

ContactEditor::~ContactEditor()
{
    delete ui;
}

void ContactEditor::on_UidField_textChanged(const QString &aText)
{
    iTempContact = new Contact();
    iTempContact->ReadContactFile(aText.toLongLong());
}

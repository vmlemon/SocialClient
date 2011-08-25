#ifndef CONTACTEDITOR_H
#define CONTACTEDITOR_H

#include <QDialog>
#include <QString>
#include <contact.h>


namespace Ui {
    class ContactEditor;
}

class ContactEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ContactEditor(QWidget *parent = 0);
    ~ContactEditor();

private slots:
    void on_UidField_textChanged(const QString &aText);
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ContactEditor *ui;
    Contact *iTempContact;
};

#endif // CONTACTEDITOR_H

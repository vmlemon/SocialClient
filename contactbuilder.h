#ifndef CONTACTBUILDER_H
#define CONTACTBUILDER_H

#include <QDialog>
#include <QString>

#include <contact.h>

namespace Ui {
    class ContactBuilder;
}

class ContactBuilder : public QDialog
{
    Q_OBJECT

public:
    explicit ContactBuilder(QWidget *parent = 0);
    ~ContactBuilder();

private slots:
    void on_buttonBox_accepted();

    void on_SelectColour_clicked();

    void on_TwitterHandle_textChanged(const QString &Text);

    void on_SkypeHandle_textChanged(const QString &aText);

    void on_LastFmHandle_textChanged(const QString &Text);

    void on_buttonBox_rejected();

    void on_EditExistingButton_toggled(bool aChecked);

    void on_CreateNewButton_toggled(bool aChecked);

private:
    Ui::ContactBuilder *ui;
    QString iColour;
    Contact *iTempContact;
    void DisperseUri(QString aUri);
    QString Poach(QString aUri);

protected:
    void dropEvent(QDropEvent *aEvent);
    void dragEnterEvent(QDragEnterEvent *aEvent);
};

#endif // CONTACTBUILDER_H

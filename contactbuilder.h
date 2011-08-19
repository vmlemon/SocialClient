#ifndef CONTACTBUILDER_H
#define CONTACTBUILDER_H

#include <QDialog>
#include <QString>

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

    void on_SkypeHandle_textEdited(const QString &arg1);

private:
    Ui::ContactBuilder *ui;
    QString iColour;
};

#endif // CONTACTBUILDER_H

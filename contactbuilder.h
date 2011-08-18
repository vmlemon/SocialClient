#ifndef CONTACTBUILDER_H
#define CONTACTBUILDER_H

#include <QDialog>

namespace Ui {
    class ContactBuilder;
}

class ContactBuilder : public QDialog
{
    Q_OBJECT

public:
    explicit ContactBuilder(QWidget *parent = 0);
    ~ContactBuilder();

private:
    Ui::ContactBuilder *ui;
};

#endif // CONTACTBUILDER_H

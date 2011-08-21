#ifndef CONTACTEDITOR_H
#define CONTACTEDITOR_H

#include <QDialog>

namespace Ui {
    class ContactEditor;
}

class ContactEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ContactEditor(QWidget *parent = 0);
    ~ContactEditor();

private:
    Ui::ContactEditor *ui;
};

#endif // CONTACTEDITOR_H

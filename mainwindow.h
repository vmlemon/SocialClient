#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QResizeEvent>
#include <QDebug>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString BuildStatusItem(QString aText, QString aIconUri, QString aStatusColour);

private slots:

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
protected:
    void resizeEvent(QResizeEvent *aEvent);
};

#endif // MAINWINDOW_H

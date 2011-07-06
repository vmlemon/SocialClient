#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QResizeEvent>
#include <QTextStream>
#include <QDebug>

enum TSkypeStatus {
    EUnknown = 0,
    EOffline = 1,
    EOnline = 2,
    EAway = 3,
    ENotAvailable = 4,
    EDoNotDisturb = 5,
    EInvisible = 6,
    ESkypeMe = 7
};

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
    QString GetTwitterAvatarUri(QString aJsonData);
    QString GetTwitterLatestTweet(QString aJsonData);
    QString LoadDiskFeed(QString aFilePath);
    QString ParseSkypeStatus(QString aStatusData);
    QString LoadHttpFeed(QString aHttpUri);

private slots:

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
protected:
    void resizeEvent(QResizeEvent *aEvent);
};

#endif // MAINWINDOW_H

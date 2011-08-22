#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QResizeEvent>
#include <QTextStream>
#include <QDebug>
#include <QByteArray>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
    QString LoadHttpFeed(QString aHttpUri);

private slots:

    void on_lineEdit_textChanged(const QString &aText);
    void finishedSlot(QNetworkReply* aReply);

    void on_actionUpdate_Ticker_triggered();

    void on_actionCreate_Contact_triggered();

    void on_actionContact_Viewer_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    void PopulateRamCache();
    QString iStatusToRender;

protected:
    void resizeEvent(QResizeEvent *aEvent);
};

#endif // MAINWINDOW_H

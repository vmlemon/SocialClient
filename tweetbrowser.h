#ifndef TWEETBROWSER_H
#define TWEETBROWSER_H

#include <QDialog>

namespace Ui {
    class TweetBrowser;
}

class TweetBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit TweetBrowser(QWidget *parent = 0);
    ~TweetBrowser();

private:
    Ui::TweetBrowser *ui;
};

#endif // TWEETBROWSER_H

#include "tweetbrowser.h"
#include "ui_tweetbrowser.h"

TweetBrowser::TweetBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TweetBrowser)
{
    ui->setupUi(this);
}

TweetBrowser::~TweetBrowser()
{
    delete ui;
}

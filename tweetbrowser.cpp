#include "tweetbrowser.h"
#include "ui_tweetbrowser.h"

TweetBrowser::TweetBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TweetBrowser)
{
    ui->setupUi(this);

    ui->TweetList->setAlternatingRowColors(true);
    //ui->TweetList->addItems();
}

TweetBrowser::~TweetBrowser()
{
    delete ui;
}

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include<QFile>
#include<QTextStream>

#include<QMessageBox>



#include <QtNetwork/QNetworkRequest>

#include<QUrl>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    void read_local(QString get_city);

    void get_weather(QString city_number);

    ~Widget();

    QString string_city,url_city;

    QString text,code;

  //  QString temp_f[];


  //  QString text,code;
   // char code;
    /*
    void Func()
     {
QNetworkAccessManager *manager = new QNetworkAccessManager(this);
connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

QNetworkReply *reply=manager->get(QNetworkRequest(QUrl(text)));

         code=reply->readAll();
}
*/
private slots:
    void on_pushButton_clicked();

   // QString text;

 //   QNetworkAccessManager manager;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

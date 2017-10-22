#include "widget.h"
#include "ui_widget.h"

#include<QRegExp>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    

       ui->textBrowser->setPlainText("");  //清空textBrowser控件数据

       text=ui->lineEdit->text();   //获取lineEdit控件数据

       if (text=="")
            return;

       read_local(text);  //调用

}

//获取城市代码
void Widget::read_local(QString get_city)
{
    QString get_tr="\\w*\\d+="+get_city;

 
    QRegExp rx_city(get_tr);

    QFile city(":/new/prefix1/data/city.txt");//城市代码文件

    city.open(QIODevice::ReadOnly);  //打开文件，只读

    QTextStream s_sx(&city);
    while(!s_sx.atEnd())
    {
        QString line=s_sx.readLine();

        int pos(0);

        QString strList_city="";

        while ((pos = rx_city.indexIn(line, pos)) != -1)
        {
           strList_city.push_back(rx_city.capturedTexts().at(0));
           pos += rx_city.matchedLength();
        }

        if(strList_city!="")  //筛选城市代码
        {
     
            QRegExp rx_city_data("\\w*\\d+");

            int pos_c(0);

            QString strList_city_data="";

            while ((pos_c = rx_city_data.indexIn(strList_city, pos_c)) != -1)
            {
            strList_city_data.push_back(rx_city_data.capturedTexts().at(0));
            pos_c += rx_city_data.matchedLength();
            }
            if(strList_city_data!="")
            {
                city.close();

             
                get_weather(strList_city_data);

                return;
            }
        }
    }

   QMessageBox::critical(NULL,"警告","输入错误，请重新输入！",QMessageBox::Ok);
   city.close();
}


 

//获取城市天气资料
void Widget::get_weather(QString city_number)
{


    url_city="http://www.weather.com.cn/weather/"+city_number+".shtml";

    QUrl url(url_city);
    QNetworkAccessManager manager;
    QEventLoop loop;
    qDebug() << "Reading code form " << url;
    //发出请求
    QNetworkReply *reply;
    reply= manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();

    //将读到的信息写入文件

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
        //使用utf8编码, 这样可以显示中文
    code = codec->toUnicode(reply->readAll());

    QRegExp rx("id=\"hidden_title\".*°C\" />");  //气温一级筛选--------------------------
  //QRegExp rx(tr("\\w\\d{1,}/\\w\\d{1,}°C"));
    int pos(0);

    QString strList;
//qDebug("气温一级");
    while ((pos = rx.indexIn(code, pos)) != -1)
    {
    strList.push_back(rx.capturedTexts().at(0));
    pos += rx.matchedLength();    
    }
                                                    //---------------------------------

    //QRegExp rx_temp("\\w\\d{1,}/\\w\\d{1,}°C");     //气温二级筛选------

   // QRegExp rx_temp("[-]?\\d{0,}/.*\\d{0,}°C");

    QRegExp rx_temp("[-]?\\d{0,}/.*\\d{0,}°C");

    int pos_temp(0);

//qDebug("气温二级");

    QString temp_1;                            //获取当天气温

    while ((pos_temp = rx_temp.indexIn(strList, pos_temp)) != -1)
    {
        temp_1.push_back(rx_temp.capturedTexts().at(0));
        pos_temp += rx_temp.matchedLength();
      //  qDebug("3333");
    }
                                                   //--------------


    QRegExp rxss1("<div id=\"7d\" class=\"c7d\">.*<div class=\"btn\">");

    int poss1(0);
//qDebug("===");
    QString strList_7;

    while ((poss1 = rxss1.indexIn(code, poss1)) != -1)
    {
    strList_7.push_back(rxss1.capturedTexts().at(0));
    poss1 += rxss1.matchedLength();
    }


    //---------------------------------------最高温

    QRegExp rx_max("<span>[-]?\\d{1,2}");

    int pos_max(0);

//qDebug("s====");

    QString strList_max;

    while ((pos_max = rx_max.indexIn(strList_7, pos_max)) != -1)
    {
    strList_max.push_back(rx_max.capturedTexts().at(0));
    pos_max += rx_max.matchedLength();
    }


    //---------------------------------------最低温

   // QRegExp rx_min("[-]?\\d{1,2}℃");

    QRegExp rx_min("</span>.{1,5}<i>[-]?\\d{1,2}.{0,2}</i>");

    int pos_min(0);

//qDebug("s====");

    QString strList_min;

    while ((pos_min = rx_min.indexIn(strList_7, pos_min)) != -1)
    {
    strList_min.push_back(rx_min.capturedTexts().at(0));
    pos_min += rx_min.matchedLength();
    }

    //-----------------------------------------获取日期

    QRegExp rx_data("<h1>\\d+日....");

    int pos_data(0);

//qDebug("s====");

    QString strList_data;

    while ((pos_data = rx_data.indexIn(strList_7, pos_data)) != -1)
    {
    strList_data.push_back(rx_data.capturedTexts().at(0));
    pos_data += rx_data.matchedLength();
    }

    //----------------------------------------获取天气

   // QRegExp rx_weather("class=\"wea\">.{1,6}</p>");

    QRegExp rx_weather_first("</big>.{0,40}<p class=\"tem\">");

    int pos_weather_first(0);

    QString strList_weather_first;

    while ((pos_weather_first = rx_weather_first.indexIn(strList_7, pos_weather_first)) != -1)
    {
    strList_weather_first.push_back(rx_weather_first.capturedTexts().at(0));
    pos_weather_first += rx_weather_first.matchedLength();
    }

    //-----------------------------------------二级筛选
    QRegExp rx_weather("title=.{1,8}\"");//>多云</p><p class="tem">


    int pos_weather(0);


    QString strList_weather;

    while ((pos_weather = rx_weather.indexIn(strList_weather_first, pos_weather)) != -1)
    {
    strList_weather.push_back(rx_weather.capturedTexts().at(0));
    pos_weather += rx_weather.matchedLength();
    }

    //-----------------------------------指数7天

    QRegExp rx_7days_index_all("<li class=\"li1\">.*</ul>");

    int pos_7days_index_all(0);


    QString strList_7days_index_all;

    while ((pos_7days_index_all = rx_7days_index_all.indexIn(code, pos_7days_index_all)) != -1)
    {
    strList_7days_index_all.push_back(rx_7days_index_all.capturedTexts().at(0));
    pos_7days_index_all += rx_7days_index_all.matchedLength();
    }

    //----------------------------指数类型---------------

    QRegExp rx_index_type("<em>.{1,8}</em>");

    int pos_index_type(0);


    QString strList_index_type;

    while ((pos_index_type = rx_index_type.indexIn(strList_7days_index_all, pos_index_type)) != -1)
    {
    strList_index_type.push_back(rx_index_type.capturedTexts().at(0));
    pos_index_type += rx_index_type.matchedLength();
    }


    //----------------------------指数强度---------------

    QRegExp rx_index_strength("<span>.{1,5}</span>");

    int pos_index_strength(0);


    QString strList_index_strength;

    while ((pos_index_strength = rx_index_strength.indexIn(strList_7days_index_all, pos_index_strength)) != -1)
    {
    strList_index_strength.push_back(rx_index_strength.capturedTexts().at(0));
    pos_index_strength += rx_index_strength.matchedLength();
    }

    //----------------------------指数建议---------------

    QRegExp rx_index_suggestion("<p>.{1,60}</p>");

    int pos_index_suggestion(0);



    QString strList_index_suggestion;

    while ((pos_index_suggestion = rx_index_suggestion.indexIn(strList_7days_index_all, pos_index_suggestion)) != -1)
    {
    strList_index_suggestion.push_back(rx_index_suggestion.capturedTexts().at(0));
    pos_index_suggestion += rx_index_suggestion.matchedLength();
    }


  

    QStringList list_temp_1=temp_1.split("/");  //当天气温截取

    QStringList list_max=strList_max.split("<span>");  //最高温截取

    QStringList list_data=strList_data.split("<h1>");  //日期截取

    QStringList list_change=strList_weather.split("title=\"");  //天气截取

    QString strList_change_all=list_change.join("");

    QStringList list_weather=strList_change_all.split("\"");

    list_change=strList_index_type.split("<em>"); //指数类型截取

    strList_change_all = list_change.join("");

    QStringList list_index_type = strList_change_all.split("</em>");

    list_change=strList_index_strength.split("<span>"); //指数强度截取

    strList_change_all = list_change.join("");

    QStringList list_index_strength = strList_change_all.split("</span>");

    list_change=strList_index_suggestion.split("<p>"); //指数建议截取

    strList_change_all = list_change.join("");

    QStringList list_index_suggestion = strList_change_all.split("</p>");

    list_change=strList_min.split("</span>/<i>");  //最低温截取

    strList_change_all = list_change.join("");

    QStringList list_min=strList_change_all.split("</i>");


    /*
    ui->textBrowser->insertPlainText(list_index_type[0]+" 强度："+list_index_strength[0]+" 建议："+list_index_suggestion[0]+"\n"+
                                     list_index_type[1]+" 强度："+list_index_strength[1]+" 建议："+list_index_suggestion[1]+"\n"+
                                     list_index_type[2]+" 强度："+list_index_strength[2]+" 建议："+list_index_suggestion[2]+"\n"+
                                     list_index_type[3]+" 强度："+list_index_strength[3]+" 建议："+list_index_suggestion[3]+"\n"+
                                     list_index_type[4]+" 强度："+list_index_strength[4]+" 建议："+list_index_suggestion[4]+"\n"+
                                     list_index_type[5]+" 强度："+list_index_strength[5]+" 建议："+list_index_suggestion[5]);
    */
    QString strList_index_all;
  //  ui->textBrowser->insertPlainText(strList_weather_first);
  for (int n=0;n<6;n++)
    {
        strList_index_all += list_index_type[n]+"\t\t强度："+list_index_strength[n]+"\t建议："+list_index_suggestion[n]+"\n";
    }

    ui->textBrowser->insertPlainText(strList_index_all);

    ui->label_temp_1->setText("最高温："+list_temp_1[1]+" 最低温："+list_temp_1[0]);
    ui->label_temp_2->setText("最高温："+list_max[1]+" 最低温："+list_min[0]);
    ui->label_temp_3->setText("最高温："+list_max[2]+" 最低温："+list_min[1]);
    ui->label_temp_4->setText("最高温："+list_max[3]+" 最低温："+list_min[2]);

    ui->label_data_1->setText(list_data[1]+list_weather[0]);
    ui->label_data_2->setText(list_data[2]+list_weather[1]);
    ui->label_data_3->setText(list_data[3]+list_weather[2]);
    ui->label_data_4->setText(list_data[4]+list_weather[3]);
}

//天气来源（中国天气网）：http://www.weather.com.cn/
//代码没精简

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QWidgetAction>
#include <QWindow>
#include <QPainter>
#include <QPen>
#include "weathertool.h"

#define SPACE_Y         3
#define RADIUS          6
#define TEXT_OFFSET_X   12
#define TEXT_OFFSET_Y   5
#define TIME_INTERVAL   (1800 * 1000)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);//设置窗口无边框
    setFixedSize(width(), height());        //设置固定大小
    ui->lineEdit->setCursorPosition(0);

    exit_menu = new QMenu(this);            //菜单栏
    exit_action = new QAction(this);        //菜单选项
    stickyAction = new QAction(this);

    ui->searchBtn->setFlat(true);
    ui->searchBtn->setStyleSheet("QPushButton{border-image:url(:/res/png/search.png);}"
                                 "QPushButton:hover{border-image:url(:/res/png/search_hover.png);}");

    exit_action->setText("Exit");
    exit_action->setIcon(QIcon(":/res/exit.ico"));
    exit_menu->addAction(exit_action);
    stickyAction->setText("置顶");
    stickyAction->setCheckable(true);
    stickyAction->setChecked(false);
    exit_menu->addAction(stickyAction);

    /* 将label 控件添加到数组里 */
    weekList << ui->lbWeek0 << ui->lbWeek1 << ui->lbWeek2 << ui->lbWeek3 << ui->lbWeek4 << ui->lbWeek5;
    dateList << ui->lbDate0 << ui->lbDate1 << ui->lbDate2 << ui->lbDate3 << ui->lbDate4 << ui->lbDate5;
    typeList << ui->lbType0 << ui->lbType1 << ui->lbType2 << ui->lbType3 << ui->lbType4 << ui->lbType5;
    typeIconList << ui->lbTypeIcon0 << ui->lbTypeIcon1 << ui->lbTypeIcon2 << ui->lbTypeIcon3 << ui->lbTypeIcon4 << ui->lbTypeIcon5;
    aqiList << ui->lbAqi0 << ui->lbAqi1 << ui->lbAqi2 << ui->lbAqi3 << ui->lbAqi4 << ui->lbAqi5;
    fxList << ui->lbFx0 << ui->lbFx1 << ui->lbFx2 << ui->lbFx3 << ui->lbFx4 << ui->lbFx5;
    flList << ui->lbFl0 << ui->lbFl1 << ui->lbFl2 << ui->lbFl3 << ui->lbFl4 << ui->lbFl5;

    /* 图片资源文件管理 */
    typeMapSmall.insert("晴", ":/res/weatherSmall/A01_Qing.png");
    typeMapSmall.insert("多云", ":/res/weatherSmall/A04_DuoYun.png");
    typeMapSmall.insert("阴", ":/res/weatherSmall/A05_Ying.png");
    typeMapSmall.insert("有风", ":/res/weatherSmall/A06_YouFeng.png");
    typeMapSmall.insert("飓风", ":/res/weatherSmall/A17_JuFeng.png");
    typeMapSmall.insert("霾", ":/res/weatherSmall/A19_Mai.png");
    typeMapSmall.insert("阵雨", ":/res/weatherSmall/B01_ZhenYu.png");
    typeMapSmall.insert("雷阵雨", ":/res/weatherSmall/B02_LeiZhenYu.png");
    typeMapSmall.insert("雷阵雨并伴有冰雹", ":/res/weatherSmall/B03_LeiYuBingBao.png");
    typeMapSmall.insert("小雨", ":/res/weatherSmall/B04_XiaoYu.png");
    typeMapSmall.insert("中雨", ":/res/weatherSmall/B05_ZhongYu.png");
    typeMapSmall.insert("大雨", ":/res/weatherSmall/B06_DaYu.png");
    typeMapSmall.insert("暴雨", ":/res/weatherSmall/B07_BaoYu.png");
    typeMapSmall.insert("大暴雨", ":/res/weatherSmall/B08_DaBaoYu.png");
    typeMapSmall.insert("特大暴雨", ":/res/weatherSmall/B09_TeDaBaoYu.png");
    typeMapSmall.insert("雨夹雪", ":/res/weatherSmall/B21_YuJiaXue.png");
    typeMapSmall.insert("冻雨", ":/res/weatherSmall/B23_DongYu.png");
    typeMapSmall.insert("阵雪", ":/res/weatherSmall/C02_ZhenXue.png");
    typeMapSmall.insert("小雪", ":/res/weatherSmall/C03_XiaoXue.png");
    typeMapSmall.insert("中雪", ":/res/weatherSmall/C04_ZhongXue.png");
    typeMapSmall.insert("大雪", ":/res/weatherSmall/C05_DaXue.png");
    typeMapSmall.insert("暴雪", ":/res/weatherSmall/C06_BaoXue.png");
    typeMapSmall.insert("浮尘", ":/res/weatherSmall/C10_FuChen.png");
    typeMapSmall.insert("扬沙", ":/res/weatherSmall/C11_YangSha.png");
    typeMapSmall.insert("沙尘暴", ":/res/weatherSmall/C12_ShaChenBao.png");
    typeMapSmall.insert("强沙尘暴", ":/res/weatherSmall/C13_QiangShaChenBao.png");
    typeMapSmall.insert("雾", ":/res/weatherSmall/C14_Wu.png");
    typeMapSmall.insert("大暴雪", ":/res/weatherSmall/DaBaoXue.png");
    typeMapSmall.insert("雷暴雪", ":/res/weatherSmall/LeiBaoDaYu.png");
    typeMapSmall.insert("特大暴雪", ":/res/weatherSmall/TeDaBaoXue.png");
    typeMapSmall.insert("冰雹", ":/res/weatherSmall/BingBao.png");

    /* BIG */
    typeMapBig.insert("晴", ":/res/weatherBig/A01_Qing.png");
    typeMapBig.insert("多云", ":/res/weatherBig/A04_DuoYun.png");
    typeMapBig.insert("阴", ":/res/weatherBig/A05_Ying.png");
    typeMapBig.insert("有风", ":/res/weatherBig/A06_YouFeng.png");
    typeMapBig.insert("飓风", ":/res/weatherBig/A17_JuFeng.png");
    typeMapBig.insert("霾", ":/res/weatherBig/A19_Mai.png");
    typeMapBig.insert("阵雨", ":/res/weatherBig/B01_ZhenYu.png");
    typeMapBig.insert("雷阵雨", ":/res/weatherBig/B02_LeiZhenYu.png");
    typeMapBig.insert("雷阵雨并伴有冰雹", ":/res/weatherBig/B03_LeiYuBingBao.png");
    typeMapBig.insert("小雨", ":/res/weatherBig/B04_XiaoYu.png");
    typeMapBig.insert("中雨", ":/res/weatherBig/B05_ZhongYu.png");
    typeMapBig.insert("大雨", ":/res/weatherBig/B06_DaYu.png");
    typeMapBig.insert("暴雨", ":/res/weatherBig/B07_BaoYu.png");
    typeMapBig.insert("大暴雨", ":/res/weatherBig/B08_DaBaoYu.png");
    typeMapBig.insert("特大暴雨", ":/res/weatherBig/B09_TeDaBaoYu.png");
    typeMapBig.insert("雨夹雪", ":/res/weatherBig/B21_YuJiaXue.png");
    typeMapBig.insert("冻雨", ":/res/weatherBig/B23_DongYu.png");
    typeMapBig.insert("阵雪", ":/res/weatherBig/C02_ZhenXue.png");
    typeMapBig.insert("小雪", ":/res/weatherBig/C03_XiaoXue.png");
    typeMapBig.insert("中雪", ":/res/weatherBig/C04_ZhongXue.png");
    typeMapBig.insert("大雪", ":/res/weatherBig/C05_DaXue.png");
    typeMapBig.insert("暴雪", ":/res/weatherBig/C06_BaoXue.png");
    typeMapBig.insert("浮尘", ":/res/weatherBig/C10_FuChen.png");
    typeMapBig.insert("扬沙", ":/res/weatherBig/C11_YangSha.png");
    typeMapBig.insert("沙尘暴", ":/res/weatherBig/C12_ShaChenBao.png");
    typeMapBig.insert("强沙尘暴", ":/res/weatherBig/C13_QiangShaChenBao.png");
    typeMapBig.insert("雾", ":/res/weatherBig/C14_Wu.png");
    typeMapBig.insert("大暴雪", ":/res/weatherBig/DaBaoXue.png");
    typeMapBig.insert("雷暴雪", ":/res/weatherBig/LeiBaoDaYu.png");
    typeMapBig.insert("特大暴雪", ":/res/weatherBig/TeDaBaoXue.png");
    typeMapBig.insert("冰雹", ":/res/weatherBig/BingBao.png");

    /*********************************************/
    net_accessManager = new QNetworkAccessManager(this);
    //槽函数连接
    connect(exit_action, &QAction::triggered, this, [=]{
        qApp->exit(0);
    });
    //置顶槽函数连接
    connect(stickyAction, &QAction::triggered, this, &MainWindow::on_stickyAction_triggered);
    /* 网络信息接收触发 */
    connect(net_accessManager, &QNetworkAccessManager::finished, this,  &MainWindow::net_reply);
    /* 回车键触发函数 */
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, [=]{
        QString cityName = ui->lineEdit->text();
        if(get_weatherInfo(cityName) != 0)
        {
            ui->lineEdit->clear();
        }
    });

    get_weatherInfo(this->cityName);
//    paintHighTempChange();
//    paintLowTempChange();
    /* 给标签添加事件过滤器 否则无法获取坐标 */
    ui->lbHigh->installEventFilter(this);
    ui->lbLow->installEventFilter(this);

    /* 开始定时器 30分钟刷新一次 */
    timerId = this->startTimer(1800 * 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    //弹出右键菜单
    exit_menu->exec(QCursor::pos());
    //    event->accept();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    this->m_offset = event->globalPos() - this->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->m_offset);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == this->timerId)
    {
        get_weatherInfo(this->cityName);
    }
}


/* 网络相关函数 */
void MainWindow::net_reply(QNetworkReply *reply)
{
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << status_code;
    qDebug() << reply->url();
//    qDebug() << reply->operation();
//    qDebug() << reply->rawHeaderList();
//    qDebug() << reply->error();
    if((reply->error() != QNetworkReply::NoError) || (status_code != 200))
    {
        qDebug() << reply->errorString().toLatin1().data();
        QMessageBox::warning(this, "天气", "请求数据失败！", QMessageBox::Ok);
    }
    else
    {
        QByteArray json = reply->readAll();
//        qDebug() << json.data();
        /* json 解析 */
        jsonParse(json);
        uiUpdate();
        this->killTimer(this->timerId);
        this->startTimer(TIME_INTERVAL);
    }
    reply->deleteLater();
}

int MainWindow::get_weatherInfo(QString cityName)
{
    this->cityName = cityName;
    QString cityCode = WeatherTool::getCityCode(this->cityName);
    qDebug() << "cityCode: " << cityCode;
    if(cityCode.isEmpty())
    {
        QMessageBox::warning(this, "天气", "没有该地区的天气!", QMessageBox::Ok);
        return -1;
    }
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    net_accessManager->get(QNetworkRequest(url));
    return 0;
}

void MainWindow::jsonParse(QByteArray json)
{
    qDebug() << QString(json).toUtf8().data();
    //判断是否是json 对象
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug() << "Json Error";
        return ;
    }

    //获取Json 对象 rootObj
    QJsonObject rootObj = doc.object();

    int status = rootObj.value("status").toDouble();
    if(status != 200)
    {
        QMessageBox::warning(this, "天气", "请求数据失败！", QMessageBox::Ok);
        return ;
    }

    //解析 日期和城市
    today.city = rootObj.value("cityInfo").toObject().value("city").toString();

    //获取 data 对象
    QJsonObject dataObj = rootObj.value("data").toObject();
    today.shidu = dataObj.value("shidu").toString();
    today.pm25 = dataObj.value("pm25").toDouble();
    today.quality = dataObj.value("quality").toString();
    today.wendu = dataObj.value("wendu").toString().toDouble();
    today.ganmao = dataObj.value("ganmao").toString();
    //解析 yesterday
    QJsonObject yesterdayObj = dataObj.value("yesterday").toObject();
    QString str;
    str = yesterdayObj.value("high").toString().split(" ").at(1);
    str = str.left(str.length() - 1);
    day[0].high = str.toInt();
    str = yesterdayObj.value("low").toString().split(" ").at(1);
    str = str.left(str.length() - 1);
    day[0].low = str.toInt();
    day[0].fx = yesterdayObj.value("fx").toString();
    day[0].fl = yesterdayObj.value("fl").toString();
    day[0].type = yesterdayObj.value("type").toString();
    day[0].aqi = yesterdayObj.value("aqi").toDouble();
    //解析forecast
    QJsonArray forecastArray = dataObj.value("forecast").toArray();
    for(int i = 0; i < 5; i++)
    {
        QJsonObject forecastObj = forecastArray[i].toObject();
        str = forecastObj.value("high").toString().split(" ").at(1);
        str = str.left(str.length() - 1);
        day[i + 1].high = str.toInt();

        str = forecastObj.value("low").toString().split(" ").at(1);
        str = str.left(str.length() - 1);
        day[i + 1].low = str.toInt();

        day[i + 1].ymd = forecastObj.value("ymd").toString();
        day[i + 1].week = forecastObj.value("week").toString();
        day[i + 1].sunrise = forecastObj.value("sunrise").toString();
        day[i + 1].sunset = forecastObj.value("sunset").toString();
        day[i + 1].aqi = forecastObj.value("aqi").toDouble();
        day[i + 1].fx = forecastObj.value("fx").toString();
        day[i + 1].fl = forecastObj.value("fl").toString();
        day[i + 1].type = forecastObj.value("type").toString();
        day[i + 1].notice = forecastObj.value("notice").toString();
//        qDebug() << "high: " << day[i].high;
//        qDebug() << "low: " << day[i].low;
//        qDebug() << "ymd: " << day[i].ymd;
//        qDebug() << "week: " << day[i].week;
//        qDebug() << "aqi: " << day[i].aqi;
//        qDebug() << "fx: " << day[i].fx;
//        qDebug() << "fl: " << day[i].fl;
//        qDebug() << "type: " << day[i].type;
//        qDebug() << "notice: " << day[i].notice;
    }
    today.high = day[1].high;
    today.low = day[1].low;
    today.ymd = day[1].ymd;
    today.week = day[1].week;
    today.sunrise = day[1].sunrise;
    today.sunset = day[1].sunset;
    today.aqi = day[1].aqi;
    today.fx = day[1].fx;
    today.fl = day[1].fl;
    today.type = day[1].type;
    today.notice = day[1].notice;
    qDebug() << "city: " << today.city;
    qDebug() << "humidity: " << today.shidu;
    qDebug() << "pm25: " << today.pm25;
    qDebug() << "quality: " << today.quality;
    qDebug() << "temp: " << today.wendu;
    qDebug() << "high: " << today.high;
    qDebug() << "low: " << today.low;
    qDebug() << "ymd: " << today.ymd;
    qDebug() << "week: " << today.week;
    qDebug() << "sunrise: " << today.sunrise;
    qDebug() << "sunset: " << today.sunset;
    qDebug() << "aqi: " << today.aqi;
    qDebug() << "fx: " << today.fx;
    qDebug() << "fl: " << today.fl;
    qDebug() << "type: " << today.type;
    qDebug() << "ganmao: " << today.ganmao;
    qDebug() << "notice: " << today.notice;
}

void MainWindow::uiUpdate()
{
    /* 更新今天的详细天气数据 */
    ui->lbCity->setText(today.city);
    ui->lbDate->setText(QDateTime::fromString(today.ymd, "yyyy-MM-dd").toString("yyyy/MM/dd"));
    ui->lbWeek->setText(today.week);
    ui->lbWendu->setText(QString::number(today.wendu) + "°");
    ui->lbType->setText(today.type + " " + QString::number(today.low) + "°~" + QString::number(today.high) + "°");
    ui->lbsunRise->setText(today.sunrise);
    ui->lbsunSet->setText(today.sunset);
    ui->lbTips->setText("感冒指数: " + today.notice);
    ui->lbFx->setText(today.fx);
    ui->lbFl->setText(today.fl);
    ui->lbPm->setText("PM2.5");
    ui->lbPmValue->setText(QString::number(today.pm25));
    ui->lbHum->setText("湿度");
    ui->lbHumValue->setText(today.shidu);
    ui->lbAqi->setText("空气质量");
    ui->lbAqiValue->setText(today.quality);
    ui->lbTypeIcon->setPixmap(typeMapBig[today.type]);

    /* 更新forecast 天气数据 */
    for(int i = 0; i < 6; i++)
    {
        weekList[i]->setText("周" + day[i].week.right(1));
        ui->lbWeek0->setText("昨天");
        ui->lbWeek1->setText("今天");
        ui->lbWeek2->setText("明天");
        QStringList ymdList = day[i].ymd.split("-");
        dateList[i]->setText(ymdList[1] + "/" + ymdList[2]);
        typeList[i]->setText(day[i].type);
        typeIconList[i]->setPixmap(typeMapSmall[day[i].type]);
        if(day[i].aqi <= 50)
        {
            aqiList[i]->setText("优");
            aqiList[i]->setStyleSheet("background-color: rgb(121, 182, 0);");
        }
        else if(day[i].aqi <= 100)
        {
            aqiList[i]->setText("良");
            aqiList[i]->setStyleSheet("background-color: rgb(255, 187, 23);");
        }
        else if(day[i].aqi <= 150)
        {
            aqiList[i]->setText("轻度");
            aqiList[i]->setStyleSheet("background-color: rgb(182, 121, 182);");
        }
        else if(day[i].aqi <= 200)
        {
            aqiList[i]->setText("中度");
            aqiList[i]->setStyleSheet("background-color: rgb(182, 60, 91);");
        }
        else if(day[i].aqi <= 250)
        {
            aqiList[i]->setText("重度");
            aqiList[i]->setStyleSheet("background-color: rgb(182, 0, 0);");
        }
        else
        {
            aqiList[i]->setText("严重");
            aqiList[i]->setStyleSheet("background-color: rgb(120, 0, 0);");
        }
        fxList[i]->setText(day[i].fx);
        flList[i]->setText(day[i].fl);
    }
    //更新标签
    ui->lbHigh->update();
    ui->lbLow->update();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->lbHigh && event->type() == QEvent::Paint)
    {
        paintHighTempChange();
    }
    else if(watched == ui->lbLow && event->type() == QEvent::Paint)
    {
        paintLowTempChange();
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::paintHighTempChange()
{
    QPainter painter(ui->lbHigh);   /* 创建标签的画布 */
    painter.setRenderHint(QPainter::Antialiasing, true);    //设置抗锯齿
    //1. 获取x坐标
    int pointX[6] = {0};
    for(int i = 0; i < 6; i++)
    {
        pointX[i] = aqiList[i]->pos().x() + aqiList[i]->width() / 2;
    }
    //2.获取y坐标
    //2.1 获取所有温度的平均温度
    int tempAverage  = 0;
    for(int i = 0; i < 6; i++)
    {
        tempAverage += day[i].high;
    }
    tempAverage /= 6;
    int pointY[6];
    int yCenter = ui->lbHigh->height() / 2;
    for(int i = 0; i < 6; i++)
    {
        pointY[i] = yCenter - (day[i].high - tempAverage) * SPACE_Y;
    }
    //3.开始绘制
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(255, 187, 23));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(255, 187, 23), Qt::SolidPattern));//填充的颜色
    /* 画点和文本 */
    for(int i = 0; i < 6; i++)
    {
        painter.drawEllipse(pointX[i], pointY[i], RADIUS, RADIUS);
        painter.drawText(pointX[i] - TEXT_OFFSET_X, pointY[i] - TEXT_OFFSET_Y, QString::number(day[i].high) + "°C");
    }
    /* 画线 */
    for(int i = 0; i < 5; i++)
    {
        if(i == 0)
        {
            pen.setStyle(Qt::DotLine);  //虚线
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);  //实线
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i], pointY[i] + RADIUS / 2, pointX[i + 1], pointY[i + 1] + RADIUS / 2);
    }

}

void MainWindow::paintLowTempChange()
{
    QPainter painter(ui->lbLow);   /* 创建标签的画布 */
    painter.setRenderHint(QPainter::Antialiasing, true);    //设置抗锯齿
    //1. 获取x坐标
    int pointX[6] = {0};
    for(int i = 0; i < 6; i++)
    {
        pointX[i] = aqiList[i]->pos().x() + aqiList[i]->width() / 2;
    }
    //2.获取y坐标
    //2.1 获取所有温度的平均温度
    int tempAverage  = 0;
    for(int i = 0; i < 6; i++)
    {
        tempAverage += day[i].low;
    }
    tempAverage /= 6;
    int pointY[6];
    int yCenter = ui->lbLow->height() / 2;
    for(int i = 0; i < 6; i++)
    {
        pointY[i] = yCenter - (day[i].low - tempAverage) * SPACE_Y;
    }
    //3.开始绘制
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(0, 0xbf, 0xff));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(0, 0xbf, 0xff), Qt::SolidPattern));//填充的颜色
    /* 画点和文本 */
    for(int i = 0; i < 6; i++)
    {
        painter.drawEllipse(pointX[i], pointY[i], RADIUS, RADIUS);
        painter.drawText(pointX[i] - TEXT_OFFSET_X, pointY[i] - TEXT_OFFSET_Y, QString::number(day[i].low) + "°C");
    }
    /* 画线 */
    for(int i = 0; i < 5; i++)
    {
        if(i == 0)
        {
            pen.setStyle(Qt::DotLine);  //虚线
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);  //实线
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i], pointY[i] + RADIUS / 2, pointX[i + 1], pointY[i + 1] + RADIUS / 2);
    }
}


void MainWindow::on_searchBtn_clicked()
{
    QString cityName = ui->lineEdit->text();
    if(get_weatherInfo(cityName) != 0)
    {
        ui->lineEdit->clear();
    }
}

/* 窗口置顶无闪烁 */
void MainWindow::on_stickyAction_triggered(bool checked)
{
    QWindow *pWin = windowHandle();
    Qt::WindowFlags mFlags = windowFlags();
    if(checked)
    {
//        qDebug() << "checked";
        pWin->setFlags(mFlags | Qt::WindowStaysOnTopHint);

    }
    else
    {
//        qDebug() << "no checked";
        pWin->setFlags(mFlags & ~Qt::WindowStaysOnTopHint);
    }
    pWin->show();
}


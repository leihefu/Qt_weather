#ifndef WEATHERDAY_H
#define WEATHERDAY_H

#include <QString>

class WeatherDay{

public:
    WeatherDay(){
        /* forecast day data */
        high = 30;
        low = 22;
        ymd = "2024-08-11";
        week = "周五";
        sunrise = "06:06";
        sunset = "18:06";
        aqi = 60;
        fx = "东北风";
        fl = "2级";
        type = "多云";
        notice = "感冒指数";

        /* today yestoday data */
        city = "广州";
        shidu = "60%";
        pm25 = 0;
        quality = "优";
        wendu = 28;
        ganmao = "初始值";
    }
    /* forecast day data */
    int high;
    int low;
    QString ymd;
    QString week;
    QString sunrise;
    QString sunset;
    int aqi;
    QString fx;
    QString fl;
    QString type;
    QString notice;

    /* today yestoday data */
    QString city;
    QString shidu;
    int pm25;
    QString quality;
    int wendu;
    QString ganmao;
};


#endif // WEATHERDAY_H

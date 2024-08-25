#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H

#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

class WeatherTool {
private:
    static QMap<QString, QString> cityMap;
    static void initCityMap()
    {
        //读取文件
        QString filePath = ":/res/my_cityCode.json";
//        QString filePath = ":/res/citycode.json";
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray json = file.readAll();
        file.close();
//        qDebug() << json.data();
//        qDebug() << QString(json).toUtf8().data();
        //解析Json
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(json, &err);
        if(err.error != QJsonParseError::NoError)
        {
            qDebug() << "Json Error";
            return ;
        }

        if(!doc.isArray())
        {
            qDebug() << "Json Error";
            return ;
        }

        //获取Json
        QJsonArray rootArray = doc.array();
        for(int i = 0; i < rootArray.size(); i++)
        {
            QJsonObject jsonObj = rootArray[i].toObject();
            QString cityName = jsonObj.value("cityName").toString();
            QString cityCode = jsonObj.value("cityCode").toString();
            if(cityCode.size() > 0)
                cityMap.insert(cityName, cityCode);
        }
    }

public:
    static QString getCityCode(QString cityName)
    {
        if(cityMap.isEmpty())
        {
            initCityMap();
        }

        QMap<QString, QString>::iterator it = cityMap.find(cityName);
        if(it == cityMap.end())
        {
            it = cityMap.find(cityName + "市");
        }
        if(it != cityMap.end())
        {
            return it.value();
        }
        return "";
    }

};

QMap<QString, QString> WeatherTool::cityMap = {};

#endif // CITYCODE_H

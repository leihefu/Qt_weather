#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QLabel>
#include <QTimer>
#include "weatherday.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override; //重写虚函数实现鼠标右键功能
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private slots:
    void on_searchBtn_clicked();
    void on_stickyAction_triggered(bool checked);
private:
    QMenu *exit_menu;       //右键弹出菜单
    QAction *exit_action;   //退出行为
    QAction *stickyAction;//置顶
    QPoint m_offset;
    WeatherDay day[6];
    WeatherDay today;
    QString cityName = "广州";
    int timerId;

    QList<QLabel *> weekList;
    QList<QLabel *> dateList;
    QList<QLabel *> typeList;
    QList<QLabel *> typeIconList;
    QList<QLabel *> aqiList;
    QList<QLabel *> fxList;
    QList<QLabel *> flList;

    QMap<QString, QString> typeMapSmall;
    QMap<QString, QString> typeMapBig;

    /* 网络类 */
    QNetworkAccessManager *net_accessManager;
    void net_reply(QNetworkReply *reply);
    int get_weatherInfo(QString cityCode);

    /* json 解析 */
    void jsonParse(QByteArray json);

    /* UI 更新 */
    void uiUpdate(void);

    /* 绘图函数 */
    bool eventFilter(QObject *watched, QEvent *event);  //重写父类函数
    void paintHighTempChange(void);
    void paintLowTempChange(void);
};
#endif // MAINWINDOW_H

#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include "dataClass.h"

namespace Ui {
class ServerStatus;
}

class ServerStatus : public QWidget
{
    Q_OBJECT
signals:
    void closeServer();

private slots:
    //新连接
    void getNewConn();

    //玩家请求创建主机
    void requstCreateHost(QString);

    //玩家加入主机
    void playerJoinHost(QString,QString);

    //对方玩家退出主机
    void playerQuitHost(QString,QString);

    //对方玩家连接丢失
    void playerLost(QString);

    //更新当前连接数
    void updataCurrConn();

    void closeEvent(QCloseEvent *event);


public:
    explicit ServerStatus(QWidget *parent = nullptr);
    ~ServerStatus();

    //更新大厅信息
    void updataGameInfo();

    //设置服务
    void setGameServer(QTcpServer *);
private:
    Ui::ServerStatus *ui;

    QTcpServer *player_server;

    QList<QPair<QString,QString>> player_conn_info;

    //发送已连接信息
    void sendGameInfo(QTcpSocket *);

};

#endif // SERVERSTATUS_H

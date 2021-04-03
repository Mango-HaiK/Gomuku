#ifndef SERVERSATUS_H
#define SERVERSATUS_H

#include <QDialog>
#include <QTableWidget>
#include <QTcpSocket>
#include "dataClass.h"
#include "HomePage.h"

namespace Ui {
class ServerStatus;
}
/**
 * @brief The ServerSatus class
 * 用于建立游戏和查看游戏大厅
 */
class ServerStatus : public QDialog
{
    Q_OBJECT

public:
    ServerStatus(QDialog *parent = nullptr);
    ~ServerStatus();

    //设置Socket
    void setSocket(QTcpSocket *socket);

    //设置用户名
    void setUserName(QString username);

    //关闭服务器连接
    void colseServerConn();

private slots:

    //建立房间
    void on_btn_create_room_clicked();

    void on_btn_quit_lobby_clicked();

    void on_tbw_lobby_info_itemDoubleClicked(QTableWidgetItem *item);

signals:
    //与服务器断开信号
    void disConnByserver();
private:
    Ui::ServerStatus *ui;

    //用于连接服务器的socket
    QTcpSocket *conn_Server_Socket;

    //存储玩家对战信息
    QList<QPair<QString,QString>> player_conn_info;

    //数据包
    MsgRequestType *mrt;

    //用户名
    QString username;

    //游戏状态
    HomePage *game_status;

    //从socket获取游戏大厅信息
    void getGameInfoData();

    //设置大厅信息
    void setLobbyInfo(QString &);
};

#endif // SERVERSATUS_H

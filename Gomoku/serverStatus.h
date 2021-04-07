#ifndef SERVERSATUS_H
#define SERVERSATUS_H

#include <QDialog>
#include <QTableWidget>
#include <QTcpSocket>
#include "dataClass.h"
#include "loginServer.h"

namespace Ui {
class ServerStatus;
}
/**
 * @brief The ServerSatus class
 * 处理服务器大厅的数据，并显示
 * 创建和加入对局
 * 返回对手的信息
 */
class ServerStatus : public QDialog
{
    Q_OBJECT

public:
    ServerStatus(QDialog *parent = nullptr);
    ~ServerStatus();

    NetPlayerInfo* getNetPlayerInfo();

    //设置玩家类型
    void setPlayerRole();

    //关闭服务器连接
    void colseServerConn();

private slots:

    //建立房间
    void on_btn_create_room_clicked();

    void on_btn_quit_lobby_clicked();

    void on_tbw_lobby_info_itemDoubleClicked(QTableWidgetItem *item);

    void on_btn_login_server_clicked();

    //设置与服务器的socket
    void setSocket(QTcpSocket*);
signals:
    //与服务器断开信号
    void disConnByserver();

    //创建房间
    void createRoom();
private:
    Ui::ServerStatus *ui;

    //用于连接服务器的socket
    QTcpSocket *conn_Server_Socket;

    //存储玩家对战信息
    QList<QPair<QString,QString>> player_conn_info;

    //数据包
    MsgRequestType *mrt;

    //登陆服务器
    LoginServer *login_server;

    //是否连接服务器
    bool conn_flag;

    //网络玩家对战结构
    NetPlayerInfo *net_player_info;

    //玩家角色 - HOST or GUEST
    PlayerRole player_role;

    //从socket获取游戏大厅信息
    void getGameInfoData();

    //设置大厅信息
    void setLobbyInfo(QString &);
};

#endif // SERVERSATUS_H

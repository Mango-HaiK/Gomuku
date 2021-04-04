#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPainter>
#include "dataClass.h"
#include "gameMode.h"
#include "startGame.h"

namespace Ui {
class HomePage;
}
/**
 * @brief The GameStatus class
 *  用于游戏状态
 * */
class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

    //棋盘
    //棋盘大小

    //人 - 人对抗
    //设置socket
    void setSocket(QTcpSocket *socket);

    //从服务器获取新连接
    void getNewDataFromServer();

    //设置玩家角色
    void setPlayerRole(PlayerRole);

    //设置玩家信息 - 参数为对手的信息
    void setPlayerInfo(QString);


    //服务器状态窗口
     //ServerStatus serverForm;

    //新连接
    void getNewConn();

    //连接成功
    void connSucceed();

    //连接失败
    void connFail();
protected:
    //绘制棋盘
    void paintEvent(QPaintEvent *event) override;

    //监听鼠标移动情况
    void mouseMoveEvent(QMouseEvent *event) override;

    //落子
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::HomePage *ui;

    //启动界面
    StartGame *startForm;
    //
    GameMode *game;

    //游戏类型
    GameType game_type;


    //落子标记坐标
    int clickPosRow,clickPosCol;



    //数据
    MsgRequestType *mrt;

    //用于连接的服务器的socket
    QTcpSocket *conn_server_socket;

    //初始化游戏信息
    void initGameInfo();

    //Host 玩家 用于创建服务
    QTcpServer *host_server;

    //Guest 玩家 用于连接socket
    QTcpSocket *guest_socket;

    //当前玩家状态
    //PlayerSatus player_status;

    //当前玩家在网络对战中的角色
    PlayerRole player_role;

private slots:
    //初始化PVE
    void initPVEGame();

    //初始化PVP
    void initPVPGame();

    //人执行
    void chessOneByPerson();

    //AI执行
    void chessOneByAI();



};

#endif // HOMEPAGE_H

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPainter>
#include "dataClass.h"
#include "gameMode.h"
#include "startGame.h"
#include "serverStatus.h"

class QDateTime;

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

protected:
    //绘制棋盘
    void paintEvent(QPaintEvent *event) override;

    //监听鼠标移动情况
    void mouseMoveEvent(QMouseEvent *event) override;

    //落子
    void mouseReleaseEvent(QMouseEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::HomePage *ui;

    //启动界面
    StartGame *startForm;
    //
    GameMode *game;

    //游戏类型
    GameType game_type;

    //服务器大厅
    ServerStatus *server_status;

    //落子标记坐标
    int clickPosRow,clickPosCol;


    //初始化游戏信息
    void initGameInfo();

    //当前玩家在网络对战中的角色
    PlayerRole player_role;

    void setTextInfo(QString);

private slots:

    //关于 ui
    //游戏准备
    void recvMsgGameReady(PlayerRole);

    //游戏开始
    void recvMsgGameStart();

    //玩家加入
    void recvPlayerJoin();

    void recvMsgChat(QString);
    //聊天

    //监听失败
    void listenErrorDispos();


    //初始化PVE
    void initPVEGame();

    //初始化PVP
    void initPVPGame();

    //人执行
    void chessOneByPerson();

    //AI执行
    void chessOneByAI();

    //悔棋
    void undo();

    //退出到大厅
    void PlayerQuit();

    void isAgreeUndo(bool);

    void on_btn_send_char_msg_clicked();
    void on_btn_ready_clicked();
    void on_btn_undo_clicked();
    void on_btn_currender_clicked();
};

#endif // HOMEPAGE_H

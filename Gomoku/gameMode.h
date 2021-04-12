#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <vector>
#include "serverStatus.h"
#include "dataClass.h"

using std::vector;

//游戏类型
enum GameType
{
    PERSON,
    BOT
};

//游戏状态
enum GameStatus
{
    PLAYING,
    WIN,
    READ
};

class GameMode : public QObject
{
    Q_OBJECT
public:
    GameMode();

    //存储当前棋盘棋子的情况 - 无棋子 -> 0 | 黑棋 -> -1 | 白棋 -> 1
    vector<vector<int>> boardStatusVec;

    //存储各点位评分情况，用于AI下棋
    vector<vector<int>> scoreVec;

    //当前下棋的一方 - AI
    bool playerFlag;

    //游戏模式
    GameType gameType;

    //游戏状态
    GameStatus gameStatus;

    //玩家用于与对手通信的socket
    QTcpSocket *player_socket;

    //初始化棋盘
    void initBoard();
    //开始游戏
    void readyGame(GameType type);

    //判断输赢
    bool isWin(int row, int col);

    //判断死局
    bool isDead();

    //落子 - 人
    void actionByPerson(int row,int col);

    //落子 - AI
    void actionByAI(int row,int col);

    //计算评分
    void calculateScore();

    //落子之后更新棋盘
    void updateBoardVec(int row, int col);

    /**************/

    //人 - 人对抗
    //设置socket
    void setSocket(QTcpSocket *socket);

    //从服务器获取新连接
    void getNewDataFromServer();

    //当前下棋的一方 -VS Person
    PlayerRole turnFlag;

    //设置玩家信息 - 参数为对手的信息
    void setPlayerInfo(QString);

    //新连接
    void getNewConn();

    //连接成功
    void connSucceed();

    //连接失败
    void connFail();

    //获取当前玩家角色 HOST ？ GUSET
    PlayerRole getCurrRole();

    //接受到信息开始游戏
    void recvMsgGameStart();

private:
    //数据
    MsgRequestType *mrt;


    //主机玩家信息
    NetPlayerInfo *host_player_info;

    //客机玩家信息
    NetPlayerInfo *guest_player_info;

    //玩家角色
    PlayerRole player_role;

    //游戏服务器
    ServerStatus *server_status;

    //用于连接的服务器的socket
    QTcpSocket *conn_server_socket;

    //Host 玩家 用于创建服务
    QTcpServer *host_server;

    //-----
    //接受从对手客户的数据并处理
    void getNewDataFromClient();

    //落子
    void recvMsgOnchess();

private slots:
    //设置玩家角色
    void setPlayerRole(PlayerRole);

signals:
    void listenError();

    //游戏开始
    void gameStart();

    //游戏准备
    void sendMsgGameReady(PlayerRole);

    //
    void recvMsgChat(QString);

    //玩家加入
    void recvPlayerJoin();


};

#endif // GAMEMODE_H

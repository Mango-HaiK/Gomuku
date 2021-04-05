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

    //当前下棋的一方
    bool playerFlag;

    //游戏模式
    GameType gameType;

    //游戏状态
    GameStatus gameStatus;

    //开始游戏
    void startGame(GameType type);

    //判断输赢
    bool isWin(int row, int col);

    //判断死局
    bool isDead();

    //落子 - 人
    void actionByPerson(int row,int col);

    //落子 - AI
    void actionByAI(int row,int col);

    //落子之后更新棋盘
    void updateBoardVec(int row, int col);

    /**************/

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
private:
    //数据
    MsgRequestType *mrt;

    //玩家角色
    PlayerRole player_role;
    //游戏服务器
    ServerStatus *server_status;

    //用于连接的服务器的socket
    QTcpSocket *conn_server_socket;

    //Host 玩家 用于创建服务
    QTcpServer *host_server;

    //Guest 玩家 用于连接socket
    QTcpSocket *guest_socket;

signals:
    void listenError();
};

#endif // GAMEMODE_H

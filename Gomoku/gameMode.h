#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "serverStatus.h"
#include "dataClass.h"
#include <QStack>

class QTcpServer;
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

    ~GameMode();

    //存储当前棋盘棋子的情况 - 无棋子 -> 0 | 黑棋 -> -1 | 白棋 -> 1
    vector<vector<int>> boardStatusVec;

    //存储各点位评分情况，用于AI下棋
    vector<vector<int>> scoreVec;

    int actionNum;

    //存储棋盘信息，用于悔棋
    QStack<vector<vector<int>>> boardRecord;

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

    //是否同意悔棋
    bool isUndo();

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
    //设置与服务器的socket
    void setSocket();

    //从服务器获取新连接
    void getNewDataFromServer();

    //当前下棋的一方 -VS Person
    PlayerRole turnFlag;

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

    //悔棋
    void undo();

    void actionUndo();

    //悔棋请求
    void recvMsgUndo();

    //同意悔棋
    void recvMsgUndoYes();

    //拒绝悔棋
    void recvMsgUndoNo();

    void sendUndoInfo(bool);

    //退出房间
    void playerQuit();

    //返回服务器窗口
    ServerStatus *getServerStatus();
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

    //-----
    //接受从对手客户的数据并处理
    void getNewDataFromClient();

    //落子
    void recvMsgOnchess();

    void recvMsgPlayerQuit();

private slots:
    //设置玩家角色
    void setPlayerRole(PlayerRole);

signals:
    void listenError();

    //游戏开始
    void gameStart();

    //游戏准备
    void MsgGameReady(PlayerRole);

    //
    void MsgChat(QString);

    //玩家加入
    void PlayerJoin();

    //悔棋
    void MsgUndo();

    void isAgreeUndo(bool);

    void MsgPlayerQuit();
};

#endif // GAMEMODE_H

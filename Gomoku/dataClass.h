#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTcpServer>
#include <QMessageBox>
//记录棋子是由哪位玩家下的
enum PlayerRole
{
    //主机玩家
    HOST,
    //非主机玩家
    GUEST,
    //AI
    AI
};

//网络对战玩家连接信息
struct NetPlayerInfo{
    QTcpSocket *socket;
    QString username;
    PlayerRole role;
};

//通信数据的类型
enum CommRequestType
{
    //服务器方
    /*连接成功*/
    MSG_SERVER_CONNYES,
    //连接失败
    MSG_SERVER_COMMNO,
    //发送大厅信息
    MSG_SERVER_GAMEINFO,
    //服务器关闭
    MSG_SERVER_CLOSE,

    //客户端方-连接阶段
    //建立主机
    MSG_CLIENT_CREATE,
    //请求连接服务器
    MSG_CLIENT_ACKCONN,
    //请求加入游戏
    MSG_CLIENT_JOIN,
    //请求断开连接
    MSG_CLIENT_OFFCONN,
    //退出当前游戏
    MSG_CLIENT_QUITGAME,

    //客户端方-游戏进行阶段
    //双方已完成准备，游戏开始
    MSG_CLIENT_GAMESTART,
    //已有玩家获胜，游戏结束
    MSG_CLIENT_GAMEOVER,
    //玩家落子
    MSG_CLIENT_ONCHESS,
    //发送聊天信息
    MSG_CLIENT_SENCHAT,
    //悔棋
    MSG_CLIENT_UNDO,
    //拒绝悔棋
    MSG_CLIENT_UNDONO,
    //同意悔棋
    MSG_CLIENT_UNDOYES,
    //玩家认输
    MSG_CLIENT_LOSE,
    //对方掉线
    MSG_CLIENT_CONNLOSE
};

//发送数据
struct MsgRequestType
{
    qint8 request;
    QString data;

};

class DataClass
{
public:
    //用户昵称
    static QString username;

    //服务器连接端口号
    static int port;

    //IP正则表达式
    const static QRegExp regIP;

    //确认对话框
    static bool checkMsgBox(QString title,QString text);
/*
*  @brief   发送数据
*  @param   commRequestType,QString,QTcpSocket
* */
    static void sendMsg(CommRequestType type,
                 QString context,
                 QTcpSocket* tcpsocket);

};

#endif // DATATYPE_H

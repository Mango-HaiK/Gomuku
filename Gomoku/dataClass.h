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
    GUEST
};

//玩家状态
enum PlayerSatus
{
    //游戏中
    PLAYING,
    //等待中
    READY,
};

//通信数据的类型
enum CommRequestType
{
    //服务器方
    /*连接成功*/
    COMM_SERVER_CONNYES,
    //连接失败
    COMM_SERVER_COMMNO,
    //发送大厅信息
    COMM_SERVER_GAMEINFO,
    //服务器关闭
    COMM_SERVER_CLOSE,

    //客户端方-连接阶段
    //建立主机
    COMM_CLIENT_CREATE,
    //请求连接服务器
    COMM_CLIENT_ACKCONN,
    //请求加入游戏
    COMM_CLIENT_JOIN,
    //请求断开连接
    COMM_CLIENT_OFFCONN,

    //客户端方-游戏进行阶段
    //双方已完成准备，游戏开始
    COMM_CLIENT_GAMESTART,
    //已有玩家获胜，游戏结束
    COMM_CLIENT_GAMEOVER,
    //玩家落子
    COMM_CLIENT_ONCHESS,
    //发送聊天信息
    COMM_CLIENT_SENCHAT,
    //悔棋
    COMM_CLIENT_UNDO,
    //拒绝悔棋
    COMM_CLIENT_UNDONO,
    //同意悔棋
    COMM_CLIENT_UNDOYES,
    //玩家认输
    COMM_CLIENT_LOSE,
    //对方掉线
    COMM_CLIENT_CONNLOSE
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

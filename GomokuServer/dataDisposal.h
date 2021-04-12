#ifndef DATADISPOSAL_H
#define DATADISPOSAL_H

#include <QThread>

#include "dataClass.h"

class DataDisposal : public QThread
{
    Q_OBJECT

public:
    DataDisposal(QObject *parent);

    //设置待处理的已连接Socket
    void setPalyerSocket(QTcpSocket *socket);
signals:
    //玩家请求建立主机
    void sign_RequestCreatHost(QString);

    //玩家加入主机
    void sign_PlayerJoinHost(QString, QString);

    //玩家退出
    void sign_PlayerQuit(QString,QString);

    //对方玩家连接丢失
    void sign_PlayerLost(QString);

    //更新当前连接数
    void sign_UpdataCurrConn();

private:
    //待处理的已连接Socket
    QTcpSocket *player_socket;

    MsgRequestType *mrt;
protected:

    void run() override;

};

#endif // DATADISPOSAL_H

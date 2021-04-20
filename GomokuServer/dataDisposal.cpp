#include "dataDisposal.h"

DataDisposal::DataDisposal(QObject *parent) : QThread(parent),player_socket(NULL),mrt(NULL)
{

}

void DataDisposal::setPalyerSocket(QTcpSocket *socket)
{
    player_socket = socket;
}

void DataDisposal::run()
{
    QDataStream in;
    mrt = new MsgRequestType();
    in.setDevice(player_socket);
    in>>mrt->request>>mrt->data;
    //qDebug()<< __FUNCTION__ << mrt->request;
    //qDebug()<<MSG_CLIENT_JOIN;
    QString clientAddr = player_socket->peerAddress().toString();

    switch(mrt->request)
    {
    case MSG_CLIENT_CREATE:
        emit sign_RequestCreatHost(clientAddr);
        break;
    case MSG_CLIENT_JOIN:
        emit sign_PlayerJoinHost(clientAddr, mrt->data);
        break;
    case MSG_CLIENT_LOSE:
        emit sign_PlayerLost(mrt->data);
    case MSG_CLIENT_QUITGAME:
        emit sign_PlayerQuit(mrt->data,clientAddr);
    default:
        break;
    }
    delete mrt;
    mrt = nullptr;
}

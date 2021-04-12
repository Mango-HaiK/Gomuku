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
    //QByteArray sbuff = player_socket->readAll();
    //QString str = sbuff;
    //qDebug() << __FUNCTION__ <<" "<< str;


    QDataStream in;
    mrt = new MsgRequestType();
    in.setDevice(player_socket);
    in>>mrt->request>>mrt->data;
    qDebug()<< __FUNCTION__ << mrt->request;
    qDebug()<<COMM_CLIENT_JOIN;
    QString clientAddr = player_socket->peerAddress().toString();

    switch(mrt->request)
    {
    case COMM_CLIENT_CREATE:
        emit sign_RequestCreatHost(clientAddr);
        break;
    case COMM_CLIENT_JOIN:
        emit sign_PlayerJoinHost(mrt->data,clientAddr);
        break;
    case COMM_CLIENT_LOSE:
        emit sign_PlayerLost(mrt->data);
    case COMM_CLIENT_OFFCONN:
        DataClass::currConnCount--;
        emit sign_PlayerQuit(mrt->data,clientAddr);
        player_socket->close();
        player_socket = nullptr;
    default:
        break;
    }
    delete mrt;
    mrt = nullptr;
}

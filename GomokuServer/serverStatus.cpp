#include "serverStatus.h"
#include "ui_serverStatus.h"
#include "dataDisposal.h"
#include <QSharedPointer>
#include <QCloseEvent>

ServerStatus::ServerStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerStatus)
{
    ui->setupUi(this);
    ui->lab_curr_conn->setNum(0);
}

ServerStatus::~ServerStatus()
{
    if(player_server)
    {
        QList<QTcpSocket*> all_socket = player_server->findChildren<QTcpSocket*>();

        for (int i = 0;i < all_socket.size(); ++i) {
            all_socket[i]->close();
            all_socket[i] = NULL;
        }
        player_server->close();
        player_server = NULL;
    }
    delete ui;
}

void ServerStatus::closeEvent(QCloseEvent *event)
{
    if(!DataClass::checkMsgBox("退出","确认关闭服务？"))
    {
        event->ignore();
        return ;
    }
    emit closeServer();
}

void ServerStatus::updataGameInfo()
{
    ui->tbw_lobby_info->setRowCount(0);
    for(int i=0; i < player_conn_info.size(); i++)
    {
        QTableWidgetItem *player1, *player2, *playStatus;
        int row = ui->tbw_lobby_info->rowCount();
        ui->tbw_lobby_info->setRowCount(row + 1);
        player1 = new QTableWidgetItem(player_conn_info[i].first);
        player1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        ui->tbw_lobby_info->setItem(row,0,player1);

        player2 = new QTableWidgetItem(player_conn_info[i].second);
        player2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        ui->tbw_lobby_info->setItem(row,1,player2);

        playStatus = new QTableWidgetItem(player_conn_info[i].second == "-" ? "等待玩家" : "对战中");
        playStatus->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        ui->tbw_lobby_info->setItem(row,2,playStatus);
    }
    ui->lab_curr_conn->setNum(DataClass::currConnCount);

    QList<QTcpSocket*> allSocket = player_server->findChildren<QTcpSocket*>();
    for(int i = 0; i < allSocket.size(); ++i)
        sendGameInfo(allSocket[i]);
}

void ServerStatus::setGameServer(QTcpServer *_server)
{
    player_server = _server;
}

void ServerStatus::getNewConn()
{
    //获取下一个待处理的已连接socket
    QTcpSocket *player_socket = player_server->nextPendingConnection();

    ++DataClass::currConnCount;
    //连接数达到最大值处理
    if(DataClass::currConnCount > DataClass::maxConnLimit)
    {
        --DataClass::currConnCount;
        player_socket->close();
        player_socket = nullptr;
        return;
    }
    //显示当前连接
    int n = DataClass::currConnCount;
    ui->lab_curr_conn->setNum(n);
    sendGameInfo(player_socket);

   //处理数据
   DataDisposal *dD = new DataDisposal(NULL);
   dD->setPalyerSocket(player_socket);
   //TODO`
   connect(dD,SIGNAL(sign_RequestCreatHost(QString)),
           this,
           SLOT(requstCreateHost(QString)));

   connect(dD,SIGNAL(sign_PlayerJoinHost(QString,QString)),
           this,
           SLOT(playerJoinHost(QString,QString)));

   connect(dD,SIGNAL(sign_PlayerQuit(QString,QString)),
           this,
           SLOT(playerQuitGame(QString,QString)));

   connect(dD,SIGNAL(sign_PlayerLost(QString)),
           this,
           SLOT(playerLost(QString)));

   connect(dD,SIGNAL(sign_UpdataCurrConn()),
           this,
           SLOT(updataCurrConn()));

   //当有可读信号时发送到start槽
   connect(player_socket, SIGNAL(readyRead()), dD, SLOT(start()));

}

void ServerStatus::requstCreateHost(QString clientAddr)
{
    //
    qDebug() << __FUNCTION__ << "" << clientAddr;
    //此时另一方玩家未加入
    QString qsNull = "-";
    player_conn_info.push_back(qMakePair(clientAddr,qsNull));
    updataGameInfo();
    //获取所有socket
    QList<QTcpSocket*> allSocket = player_server->findChildren<QTcpSocket*>();
    for(int i = 0; i < allSocket.size(); ++i)
        sendGameInfo(allSocket[i]);

}

void ServerStatus::playerJoinHost(QString hostAddr, QString guestAddr)
{
    for (int i = 0;i < player_conn_info.size(); ++i)
    {
        if(player_conn_info[i].first == hostAddr)
        {
            player_conn_info[i].second = guestAddr;
            updataGameInfo();
            QList<QTcpSocket*> allSocket = player_server->findChildren<QTcpSocket*>();
            for(int i = 0; i < allSocket.size(); ++i)
            {
                sendGameInfo(allSocket[i]);
                if(allSocket[i]->peerAddress().toString() == hostAddr)
                        DataClass::sendMsg(MSG_CLIENT_JOIN,guestAddr,allSocket[i]);
            }
        }
    }
}

void ServerStatus::playerQuitGame(QString playerRoly, QString clientAddr)
{
    if(playerRoly == "G")
    {
        for(int i = 0; i < player_conn_info.size(); ++i)
        {
            if(player_conn_info[i].second == clientAddr)
            {
                player_conn_info[i].second = "-";
                updataGameInfo();
                QList<QTcpSocket*> allSocket = player_server->findChildren<QTcpSocket*>();
                for(int i = 0; i < allSocket.size(); ++i)
                    sendGameInfo(allSocket[i]);

            }
        }
    }
    else if(playerRoly == "H")
    {
        for(int i = 0; i < player_conn_info.size(); ++i)
        {
            if(player_conn_info[i].first == clientAddr)
            {
                player_conn_info.removeAt(i);
                updataGameInfo();
                QList<QTcpSocket*> allSocket = player_server->findChildren<QTcpSocket*>();
                for(int i = 0; i < allSocket.size(); ++i)
                    sendGameInfo(allSocket[i]);
            }
        }
    }

}

void ServerStatus::playerLost(QString playerRoly)
{
    DataClass::currConnCount--;
    updataCurrConn();
}

void ServerStatus::updataCurrConn()
{
    ui->lab_curr_conn->setNum(DataClass::currConnCount);
}


void ServerStatus::sendGameInfo(QTcpSocket *socket)
{
    QString gamedata;
   //转换为字符串
    for (int i = 0; i < player_conn_info.size(); ++i)
    {
        gamedata+=player_conn_info[i].first+" "+player_conn_info[i].second+"#";
    }
    DataClass::sendMsg(MSG_SERVER_GAMEINFO,gamedata, socket);
}

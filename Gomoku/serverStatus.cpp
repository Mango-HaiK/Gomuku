#include "serverStatus.h"
#include "ui_serverStatus.h"

ServerStatus::ServerStatus(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::ServerStatus)
{
    ui->setupUi(this);

}

ServerStatus::~ServerStatus()
{
    if(conn_Server_Socket)
    {
        delete conn_Server_Socket;
        conn_Server_Socket = nullptr;
    }
    delete ui;
}

void ServerStatus::setSocket(QTcpSocket *socket)
{
    conn_Server_Socket = socket;
}

void ServerStatus::setUserName(QString username)
{
    this->username = username;
}

void ServerStatus::on_btn_create_room_clicked()
{
    //建立主机
    DataClass::sendMsg(COMM_CLIENT_CREATE," ",conn_Server_Socket);

    game_status->setSocket(conn_Server_Socket);

    connect(conn_Server_Socket,&QTcpSocket::readyRead,
            game_status,&HomePage::getNewDataFromServer);

    //设置玩家角色
    game_status->setPlayerRole(HOST);


}

void ServerStatus::on_btn_quit_lobby_clicked()
{
    if(DataClass::checkMsgBox("提示","确定退出吗？"))
        return;

    if(conn_Server_Socket)
    {
        //告知服务器关闭连接
        DataClass::sendMsg(COMM_CLIENT_OFFCONN,"",conn_Server_Socket);

        conn_Server_Socket->close();
        conn_Server_Socket = nullptr;
    }

    //发送与服务器连接断开信号
    emit disConnByserver();

}

void ServerStatus::getGameInfoData()
{
    QDataStream in;
    in.setDevice(conn_Server_Socket);

    mrt = new MsgRequestType();

    in>>mrt->request>>mrt->data;

    if(mrt->request == COMM_SERVER_GAMEINFO)
    {
        if(mrt->data == "")
        {
            ui->tbw_lobby_info->setRowCount(0);
        }
    }else if(mrt->request == COMM_CLIENT_JOIN)
    {
        setLobbyInfo(mrt->data);
    }

    delete  mrt;
    mrt = nullptr;
}

void ServerStatus::setLobbyInfo(QString &data)
{
    ui->tbw_lobby_info->setRowCount(0);
    int start = 0;
    int index_block = data.indexOf(' ');
    int index_data_end = data.indexOf('#');

    while(index_data_end != -1)
    {
        QString player1_name = data.mid(start,index_block - start);
        QString player2_name = data.mid(index_block + 1,index_data_end - index_block - 1);

        player_conn_info.push_back(qMakePair(player1_name,player2_name));

        start = index_data_end + 1;
        index_block = data.indexOf(' ',start);
        index_data_end = data.indexOf('#',start);

        QTableWidgetItem *player1,*player2,*player_status;
        int row = ui->tbw_lobby_info->rowCount();

        ui->tbw_lobby_info->setRowCount(row + 1);

        player1 = new QTableWidgetItem(player1_name);
        player1->setTextAlignment(Qt::AlignCenter);
        ui->tbw_lobby_info->setItem(row,0,player1);

        player2 = new QTableWidgetItem(player2_name);
        player2->setTextAlignment(Qt::AlignCenter);
        ui->tbw_lobby_info->setItem(row,1,player2);

        player_status = new QTableWidgetItem(player2_name == "-" ? "等待中" : "游戏中");
        player_status->setTextAlignment(Qt::AlignCenter);
        ui->tbw_lobby_info->setItem(row,3,player_status);
    }
    //如果正在游戏中将不可选
    int row = ui->tbw_lobby_info->currentRow();
    for(int i  = 0; i < ui->tbw_lobby_info->rowCount(); ++i)
    {
        if(ui->tbw_lobby_info->item(row,1)->text()  == "-")
        {
            //TODO
        }
    }
}

void ServerStatus::on_tbw_lobby_info_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = ui->tbw_lobby_info->currentRow();

    DataClass::sendMsg(COMM_CLIENT_JOIN,ui->tbw_lobby_info->item(row,0)->text(),conn_Server_Socket);
    game_status->setSocket(conn_Server_Socket);

    connect(conn_Server_Socket,&QTcpSocket::readyRead,
            game_status,&HomePage::getNewDataFromServer);

    game_status->setPlayerRole(HOST);

    game_status->setPlayerInfo(ui->tbw_lobby_info->item(row,0)->text());

}

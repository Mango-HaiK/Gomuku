#include "serverStatus.h"
#include "ui_serverStatus.h"

ServerStatus::ServerStatus(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::ServerStatus),conn_flag(false)
{
    ui->setupUi(this);
    //将此类的窗口设置为模态
    setWindowFlags(Qt::Tool |Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint|Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);

    ui->tbw_lobby_info->setSelectionBehavior ( QAbstractItemView::SelectRows);
    ui->tbw_lobby_info->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tbw_lobby_info->setEditTriggers(QAbstractItemView::NoEditTriggers);

    login_server = new LoginServer();

    connect(login_server,SIGNAL(connectYes(QTcpSocket*)),
            this,SLOT(setSocket(QTcpSocket*)));

    if(!conn_flag) ui->btn_create_room->setDisabled(true);
}

ServerStatus::~ServerStatus()
{
    if(login_server)
    {
        delete login_server;
    }
    if(conn_Server_Socket)
    {
        delete conn_Server_Socket;
        conn_Server_Socket = nullptr;
    }
    delete ui;
}

void ServerStatus::setSocket(QTcpSocket* _socket)
{
    qDebug() <<__FUNCTION__<< " this";
    conn_Server_Socket = _socket;
    conn_flag = true;

    ui->btn_create_room->setDisabled(false);


    //socket有待处理信息 - 更新游戏大厅
    connect(conn_Server_Socket,&QTcpSocket::readyRead,
            this,&ServerStatus::getGameInfoData);
}

NetPlayerInfo* ServerStatus::getNetPlayerInfo()
{
    return net_player_info;
}

QString ServerStatus::getHostSocket()
{
    return  host_socket;
}

void ServerStatus::on_btn_login_server_clicked()
{
    login_server->exec();
}
void ServerStatus::on_btn_create_room_clicked()
{
    //建立主机
    DataClass::sendMsg(COMM_CLIENT_CREATE,"",conn_Server_Socket);

    //connect(conn_Server_Socket,&QTcpSocket::readyRead,
    //       game_status,&GameMode::getNewDataFromServer);
    net_player_info = new NetPlayerInfo();

    net_player_info->username = DataClass::username;
    net_player_info->socket = nullptr;
    net_player_info->role = HOST;

    emit createRoom(HOST);

    this->close();
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

    conn_flag = false;

    //发送与服务器连接断开信号
    emit disConnByserver();

}

void ServerStatus::getGameInfoData()
{

    //收到的是游戏大厅数据还是 玩家加入Host

    QDataStream in;
    in.setDevice(conn_Server_Socket);

    mrt = new MsgRequestType();

    in>>mrt->request>>mrt->data;

    if(mrt->request == COMM_SERVER_GAMEINFO)
    {
        if(mrt->data == "")
        {
            qDebug() << "commmmm";
            ui->tbw_lobby_info->setRowCount(0);
        }
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
}

void ServerStatus::on_tbw_lobby_info_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = ui->tbw_lobby_info->currentRow();
    for(int i  = 0; i < ui->tbw_lobby_info->rowCount(); ++i)
    {
        if(ui->tbw_lobby_info->item(row,1)->text()  != "-")
        {
            QMessageBox::information(this,"提示","该房间不可加入！");
            return;
        }
    }

    //加入Host 的房间 - 请求与Host建立连接
    DataClass::sendMsg(COMM_CLIENT_JOIN,ui->tbw_lobby_info->item(row,0)->text(),conn_Server_Socket);

    emit createRoom(GUEST);

    host_socket = ui->tbw_lobby_info->item(row,0)->text();

    this->close();
}



#include "server.h"

<<<<<<< HEAD
Server::Server(QWidget *parent, Qt::WindowFlags flags)
=======
Server::Server(QWidget *parent, Qt::WFlags flags)
>>>>>>> 3dd4e108f06ad8470b2d7dfe3717ff43cb0a2378
                     : QMainWindow(parent, flags), tcpServer_player(NULL), serverStatus(NULL)
{
    ui.setupUi(this);

    widgetInit();
}

Server::~Server()
{

}

void Server::widgetInit()
{
    ui.connPort_server_LE->setValidator(new QRegExpValidator(DataClass::regPort, this));
    ui.connLimit_server_LE->setValidator(new QRegExpValidator(DataClass::regConnLimit, this));
}

void Server::on_startServer_server_BTN_clicked()
{
    if(ui.connPort_server_LE->text() == "" || ui.connLimit_server_LE->text() == "")
    {
        QMessageBox::information(this,"����","��������������Ϣ��");
        return ;
    }

    tcpServer_player = new QTcpServer(this);
    if(!(tcpServer_player->listen(QHostAddress::Any, ui.connPort_server_LE->text().toInt())))
    {
        QMessageBox::information(this,"����","�����������˿�ʧ�ܣ�");
        return ;
    }

    DataClass::maxConnectionLimit = ui.connLimit_server_LE->text().toInt();
    DataClass::connPort = ui.connPort_server_LE->text().toInt();
    serverStatus = new ServerStatus();
    serverStatus->setPara(tcpServer_player);
    //���ܵ��µ�����
    connect(tcpServer_player, SIGNAL(newConnection()), serverStatus, SLOT(getNewConn()));
    connect(serverStatus, SIGNAL(stopService()), this, SLOT(stopService()));
    serverStatus->show();
    this->hide();
}

void Server::stopService()
{
    DataClass::curConnCount = 0;
    if(serverStatus)
    {
        delete serverStatus;
        serverStatus = NULL;
    }
    this->show();
}

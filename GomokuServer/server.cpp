#include <QMessageBox>

#include "server.h"
#include "ui_server.h"


Server::Server(QWidget *parent)
    : QMainWindow(parent),player_server(nullptr)
    , ui(new Ui::server)
{
    ui->setupUi(this);

    ui->edit_port->setText("1101");
    ui->edit_maxconn->setText("50");
}

Server::~Server()
{
    delete ui;
}


void Server::on_btn_startServer_clicked()
{
    if(ui->edit_port->text() == "")
    {
        QMessageBox::information(this,tr("错误"),"请输入端口");
        return;
    }
    player_server = new QTcpServer();
    int port = ui->edit_port->text().toInt();
    DataClass::maxConnLimit = ui->edit_maxconn->text().toInt();
    if(!player_server->listen(QHostAddress::AnyIPv4,port))
    {
        QMessageBox::information(this,tr("错误"),"服务器开启失败！");
        return;
    }

    QMessageBox::information(this,tr("提示"),"服务器开启成功！");
    DataClass::serverPort =port;
    server_status = new ServerStatus();

    server_status->setGameServer(player_server);

    // 开启服务器状态 -- 绑定
    connect(player_server,SIGNAL(newConnection()),server_status,SLOT(getNewConn()));
    connect(server_status,SIGNAL(closeServer()),this,SLOT(closeServer()));

    //显示服务器状态
    server_status->show();
    this->hide();
}

void Server::closeServer()
{
    DataClass::currConnCount = 0;
    if(server_status)
    {
        delete  server_status;
        server_status = NULL;
    }
    this->show();
}

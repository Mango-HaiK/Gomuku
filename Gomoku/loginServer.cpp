#include "loginServer.h"
#include "ui_loginserver.h"
#include <QMessageBox>
#include <QIntValidator>

LoginServer::LoginServer(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginServer)
{
    ui->setupUi(this);
}

LoginServer::~LoginServer()
{
    delete ui;
}

QTcpSocket *LoginServer::getConnServerSocket()
{
    return conn_Server_Socket;
}

void LoginServer::connServerYes()
{
    DataClass::username = ui->edit_username->text();
    DataClass::port = ui->edit_port->text().toInt();

    //发送连接成功信号，更新游戏大厅
    emit connectYes();
    return;
}

void LoginServer::connServerNo()
{
    //this->show();
    QMessageBox::information(this,"错误","连接丢失！\n"
        "可能的原因：\n"
        "1.服务器地址或端口错误！\n"
        "2.服务器达到连接上限！\n"
        "3.服务器关闭！");
    return;
}

void LoginServer::disConn()
{

}

void LoginServer::on_btn_join_clicked()
{
    if(ui->edit_username->text() == "" ||
            ui->edit_ipadds->text() == "" ||
            ui->edit_port->text() == "")
    {
        QMessageBox::information(this,"错误","请输入相关信息");
        return;
    }

    conn_Server_Socket = new QTcpSocket(this);

    QString ipaddr = ui->edit_ipadds->text();
    int port = ui->edit_port->text().toInt();

    //连接服务器
    conn_Server_Socket->connectToHost(ipaddr,port);

    //连接成功
    connect(conn_Server_Socket,&QTcpSocket::connected,
            this,&LoginServer::connServerYes);

    //连接失败
    connect(conn_Server_Socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(connServerNo));
}

void LoginServer::initWidget()
{
    ui->edit_port->setValidator(new QIntValidator(0,65535,this));
    ui->edit_ipadds->
            setValidator(new QRegExpValidator(DataClass::regIP,this));
}

#include "loginServer.h"
#include "ui_loginserver.h"
#include <QMessageBox>
#include <QIntValidator>
//#include <QNetworkProxy>

LoginServer::LoginServer(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginServer), conn_Server_Socket(nullptr)
{
    ui->setupUi(this);
    conn_Server_Socket = new QTcpSocket(this);
    ui->label_show_status->hide();
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

    qDebug() << "连接成功";
    //发送连接成功信号，更新游戏大厅
    this->hide();
    emit connectYes();
    return;
}

void LoginServer::connServerNo()
{
    //this->show();
    qDebug()<<"连接失败";

    QMessageBox::information(this,"错误","连接丢失！\n"
        "可能的原因：\n"
        "1.服务器地址或端口错误！\n"
        "2.服务器达到连接上限！\n"
        "3.服务器关闭！");
    ui->btn_join->setDisabled(false);
    ui->label_show_status->hide();

    return;
}

void LoginServer::disConn()
{
    //发送与服务器断开连接信号

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

    //关闭原有连接
    if(conn_Server_Socket) conn_Server_Socket->abort();

    QString ipaddr = ui->edit_ipadds->text();
    int port = ui->edit_port->text().toInt();

    ui->btn_join->setDisabled(true);
    ui->label_show_status->show();

    //连接服务器
    //去掉代理
    //conn_Server_Socket->setProxy(QNetworkProxy::NoProxy);
    conn_Server_Socket->connectToHost(ipaddr,port);

    //TODO 开个线程
    //int rec = conn_Server_Socket->waitForConnected(30);

    //if(!rec) connServerNo();

    //连接成功
    connect(conn_Server_Socket,&QTcpSocket::connected,
            this,&LoginServer::connServerYes);

    //断开连接
    connect(conn_Server_Socket,&QTcpSocket::disconnected,
            this,&LoginServer::disConn);

    //连接失败
    connect(conn_Server_Socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(connServerNo()));
}

void LoginServer::initWidget()
{
    ui->edit_port->setValidator(new QIntValidator(0,65535,this));
    ui->edit_ipadds->
            setValidator(new QRegExpValidator(DataClass::regIP,this));
}

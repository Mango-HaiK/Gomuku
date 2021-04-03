#include "client.h"
#include "ui_client.h"
#include <QIntValidator>
#include <QMessageBox>

Client::Client(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    initWidget();
}

Client::~Client()
{
    delete ui;
}

void Client::connServerYes()
{
     DataClass::username = ui->edit_username->text();
    //TODO - ServerSatus
}

void Client::connServerNo()
{

}

void Client::disConn()
{

}

void Client::on_btn_join_clicked()
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

    //绑定槽和信号
    connect(conn_Server_Socket,&QTcpSocket::connected,
            this,&Client::connServerYes);
    connect(conn_Server_Socket,&QTcpSocket::connected,
            this,&Client::connServerNo);

}

void Client::initWidget()
{
    ui->edit_port->setValidator(new QIntValidator(0,65535,this));
    ui->edit_ipadds->
            setValidator(new QRegExpValidator(DataClass::regIP,this));
}

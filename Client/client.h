#ifndef CLIENT_H
#define CLIENT_H

#include "ui_client.h"
#include "dataClass.h"
#include "serverStatus.h"

#include <QWidget>

class Client : public QMainWindow
{
	Q_OBJECT

public:
    Client(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~Client();

private slots:
	void on_conn_client_BTN_clicked();

	//连接服务器成功
	void connSuccessful();
	//连接服务器失败 
	void connFailed();

	//主动断开连接
	void disconnection();

private:
	Ui::ClientClass ui;
	//用于交互服务器
	QTcpSocket *tcpSocket_server;

	//服务器状态窗口 
	ServerStatus serverStatus;


	//初始化控件属性
	void widgetInit();
};

#endif // CLIENT_H

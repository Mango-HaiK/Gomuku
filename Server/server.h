#ifndef SERVER_H
#define SERVER_H

#include "ui_server.h"
#include "dataClass.h"
#include "serverStatus.h"

class Server : public QMainWindow
{
	Q_OBJECT

public:
	Server(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Server();

private slots:
	void on_startServer_server_BTN_clicked();

	//关闭服务
	void stopService();

private:
	Ui::ServerClass ui;
	//TCPServer 接受client的请求
	QTcpServer *tcpServer_player;

	//服务器状态窗口
	ServerStatus *serverStatus;


	//初始化控件属性
	void widgetInit();
};

#endif // SERVER_H

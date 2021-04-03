#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H

#include <QWidget>

#include "dataClass.h"
#include "gameStatus.h"
#include "ui_serverStatus.h"

class ServerStatus : public QDialog
{
	Q_OBJECT

public:
    ServerStatus(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ServerStatus();

	//设置参数
	void setPara(QTcpSocket *_tcpSocket_server);
	//设置昵称
	void setNickName(QString);

	//服务器关闭连接
	void connClosedByServer();

private slots:
	//双击别人的主机加入
	void on_clientStatus_client_TBW_itemDoubleClicked(QTableWidgetItem*);

	//收到新数据
	void getNewData();

	void on_quit_client_BTN_clicked();
	void on_create_client_BTN_clicked();

signals:
	void disconnection();

protected:
	//响应关闭事件
	void closeEvent(QCloseEvent *);

private:
	Ui::ServerStatusClass ui;

	//游戏大厅
    GameStatus gameStatus;

	//保存玩家对战状态
	QList<QPair<QString, QString> >playerFightInfo;

	//数据包申明
	msg_request_struct* msg_req_struct;

	//用于交互服务器
	QTcpSocket *tcpSocket_server;

	//设置游戏大厅信息
	void setGameInfo(QString& data);
};

#endif // SERVERSTATUS_H

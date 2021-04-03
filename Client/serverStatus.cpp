#include "serverStatus.h"

ServerStatus::ServerStatus(QWidget *parent, Qt::WindowFlags flags)
											: QDialog(parent, flags), tcpSocket_server(NULL), 
											msg_req_struct(NULL), gameStatus(NULL)
{
	ui.setupUi(this);
}

ServerStatus::~ServerStatus()
{
	if(tcpSocket_server)
	{
		tcpSocket_server->close();
		tcpSocket_server = NULL;
	}
}

void ServerStatus::closeEvent(QCloseEvent* event)
{
	if(!DataClass::checkDialog("退出","确认退出游戏？"))
	{
		event->ignore();
		return ;
	}

	if(tcpSocket_server)
	{
		DataClass::sendMsg(COMM_CLIENT_DISCONN, "", tcpSocket_server);

		tcpSocket_server->close();
		tcpSocket_server = NULL;
	}

	emit disconnection();
	event->accept();
}

void ServerStatus::setPara(QTcpSocket *_tcpSocket_server)
{
	tcpSocket_server = _tcpSocket_server;
}

void ServerStatus::getNewData()
{
	QDataStream in;

	msg_req_struct = new msg_request_struct();
	in.setDevice(tcpSocket_server);
	in >> msg_req_struct->request >> msg_req_struct->data;

	switch (msg_req_struct->request)
	{
	case COMM_SERVER_GAMEINFO:
		if(msg_req_struct->data == "")
		{
			ui.clientStatus_client_TBW->setRowCount(0);
			break;
        }
		setGameInfo(msg_req_struct->data);
		break;
// 	case COMM_CLIENT_JOIN:
// 		gameStatus->setPlayerInfo(msg_req_struct->data);
// 		DataClass::dbgBox(1,1);
// 		break;
	default:
		break;
	}

	delete msg_req_struct;
	msg_req_struct = NULL;
}

void ServerStatus::setGameInfo(QString& data)
{
	ui.clientStatus_client_TBW->setRowCount(0);
	int posStart = 0;
	int posEnd = data.indexOf("_");
    int posBlock = data.indexOf(" ");
	while(posEnd != -1)
	{
		QString player1name = data.mid(posStart, posBlock - posStart);
		QString player2name = data.mid(posBlock + 1, posEnd - posBlock - 1);
		playerFightInfo.push_back(qMakePair(player1name, player2name));

		posStart = posEnd + 1;
		posEnd = data.indexOf("_", posStart);
		posBlock = data.indexOf(" ", posStart);

		QTableWidgetItem *player1, *player2, *playStatus;
		int row = ui.clientStatus_client_TBW->rowCount();
		ui.clientStatus_client_TBW->setRowCount(row + 1);
		player1 = new QTableWidgetItem(player1name);
		player1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
		ui.clientStatus_client_TBW->setItem(row,0,player1);
		player2 = new QTableWidgetItem(player2name);
		player2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
		ui.clientStatus_client_TBW->setItem(row,1,player2);
		playStatus = new QTableWidgetItem(player2name == "-" ? "等待玩家" : "对战中");
		playStatus->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
		ui.clientStatus_client_TBW->setItem(row,2,playStatus);
	}
}

void ServerStatus::connClosedByServer()
{
	gameStatus.hide();
}

void ServerStatus::on_quit_client_BTN_clicked()
{
	if(!DataClass::checkDialog("退出","确认退出游戏？"))
		return ;

	if(tcpSocket_server)
	{
		DataClass::sendMsg(COMM_CLIENT_DISCONN, "", tcpSocket_server);

		tcpSocket_server->close();
		tcpSocket_server = NULL;
	}

	emit disconnection();
}

void ServerStatus::on_create_client_BTN_clicked()
{
	DataClass::sendMsg(COMM_CLIENT_CREATE, "", tcpSocket_server);
	gameStatus.setPara(tcpSocket_server);
	connect(tcpSocket_server, SIGNAL(readyRead()), &gameStatus, SLOT(getNewDataFromServer()));
	gameStatus.setPlayerRole(HOST);
	gameStatus.exec();
}

void ServerStatus::on_clientStatus_client_TBW_itemDoubleClicked(QTableWidgetItem*)
{
	int row = ui.clientStatus_client_TBW->currentRow();
	if(ui.clientStatus_client_TBW->item(row,1)->text() != "-")
	{
		QMessageBox::information(this,"错误","此主机正在对战中...");
		return ;
	}
	
	DataClass::sendMsg(COMM_CLIENT_JOIN,ui.clientStatus_client_TBW->item(row, 0)->text(), tcpSocket_server);
	gameStatus.setPara(tcpSocket_server);
	connect(tcpSocket_server, SIGNAL(readyRead()), &gameStatus, SLOT(getNewDataFromServer()));
	gameStatus.setPlayerInfo(ui.clientStatus_client_TBW->item(row,0)->text());
	gameStatus.setPlayerRole(GUEST);
	gameStatus.exec();
}

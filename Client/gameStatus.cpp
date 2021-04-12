#include "gameStatus.h"

//每个五子棋小方框的高宽
const int GameStatus::paneHeight = 30;
const int GameStatus::paneWidth= 30;

//棋子半径
const int GameStatus::pieceSize = 24;

//游戏网格个数  m x m
const int GameStatus::gameCellCount = 19;
//单元网格大小
const int GameStatus::gameCellSize = 30;

//画图偏移量，横向和纵向
const int GameStatus::paintOffset_H = 20;
const int GameStatus::paintOffset_W = 20;

//鼠标位置偏移量，横向和纵向
const int GameStatus::mouseMoveOffset_H = 25;
const int GameStatus::mouseMoveOffset_W = 17;

//画鼠标范围框偏移
const int GameStatus::halfCellWidth = 2 * gameCellSize / 3;
const int GameStatus::frameWidth = gameCellSize / 6;

GameStatus::GameStatus(QWidget *parent, Qt::WindowFlags flags)
						: QDialog(parent, flags), tcpServer_player(NULL),
						tcpSocket_player(NULL), tcpSocket_server(NULL),
						win(0), lose(0), plyStatus(readyToPlayingGame),
						closeByServer(false), whosTurn(HOST)
{
	ui.setupUi(this);

	widgetInit();
}

GameStatus::~GameStatus()
{
}

void GameStatus::closeEvent(QCloseEvent *event)
{
	if(closeByServer)
	{
		timer.stop();
		dataInit();
		ui.gameReady_client_BTN->setDisabled(false);
		ui.undo_client_BTN->setDisabled(true);
		ui.lose_client_BTN->setDisabled(true);
		ui.send_client_BTN->setDisabled(true);
		QMessageBox::information(NULL,"退出","玩家退出，游戏结束！");
	}
	else
	{
		if(!DataClass::checkDialog("退出","确认退出游戏？"))
		{
			event->ignore();
			return ;
		}
		timer.stop();
		dataInit();
		ui.gameReady_client_BTN->setDisabled(false);
		ui.undo_client_BTN->setDisabled(true);
		ui.lose_client_BTN->setDisabled(true);
        ui.send_client_BTN->setDisabled(true);
		//发送退出游戏通知
		if(plyRole == GUEST)
		{
			//关闭和HOST方的连接
			//然后向HOST方发送退出信息
			DataClass::sendMsg(COMM_CLIENT_QUITGAME,"GUEST",tcpSocket_server);
			if(tcpSocket_player)
			{
				DataClass::sendMsg(COMM_CLIENT_QUITGAME,"",tcpSocket_player);
				tcpSocket_player->close();
				tcpSocket_player = NULL;
			}
		}
		else if(plyRole == HOST)
		{
			//关闭和GUEST方的连接
			//然后向GUEST方发送退出信息
			DataClass::sendMsg(COMM_CLIENT_QUITGAME,"HOST",tcpSocket_server);
			if(tcpSocket_player)
			{
				DataClass::sendMsg(COMM_CLIENT_QUITGAME,"",tcpSocket_player);
				tcpSocket_player->close();
				tcpSocket_player = NULL;
			}
			if(tcpServer_player)
			{
				tcpServer_player->close();
				tcpServer_player = NULL;
			}
		}
	}
	//code here
	//如果tcpServer_player 和 tcpSocket_player不为NULL的话 还要处理
	event->accept();
}

void GameStatus::dataInit()
{
	for(int i=0; i<19; i++)
		for(int j=0; j<19; j++)
			chessboardRecord[i][j] = 0;
	pieceRecord.clear();
}

void GameStatus::widgetInit()
{
	for(int i=0; i<gameCellCount; i++)
		for(int j=0; j<gameCellCount; j++)
			chessboardRecord[i][j] = 0;

	//设置自己的信息
	ui.addr_me_client_LE->setText("127.0.0.1");
	ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");

	//透明画图区域的QGroupBox
// 	QPalette palette;
// 	QBrush brush(QColor(0, 0, 0, 0));
// 	brush.setStyle(Qt::SolidPattern);
// 	palette.setBrush(QPalette::All, QPalette::Base, brush);
//	ui.gameArea_client_GPB->setPalette(palette);
}

void GameStatus::setPlayerInfo(QString rivarAddr)
{
	//设置自己的信息
	ui.addr_me_client_LE->setText("127.0.0.1");
	ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");

	//设置对手的信息
	ui.addr_rivar_client_LE->setText(rivarAddr);
	ui.record_rivar_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");
}

void GameStatus::setPlayerRole(playerRole _plyRole)
{
	//添加玩家之间的连接
	//如果plyRole是HOST的话，则建立一个新的监听等待对方加入
	//如果plyRole是GUTST的话，则发起一个向主机的连接
	plyRole = _plyRole;
	win = lose = 0;
	ui.chatLog_client_TE->clear();
	closeByServer = false;
	ui.gameStatus_client_LB->setText("");
	plyStatus = readyToPlayingGame;
	if(plyRole == HOST)
	{
		ui.addr_rivar_client_LE->setText("");
		ui.record_rivar_client_LE->setText("");
		ui.color_me_client_LE->setText("黑色");
		ui.color_rivar_client_LE->setText("白色");
		tcpServer_player = new QTcpServer(this);
		if(!(tcpServer_player->listen(QHostAddress::Any, DataClass::connPort - 1)))
		{
			QMessageBox::information(this,"错误","主机监听端口失败！");
			this->hide();
			return ;
		}
		connect(tcpServer_player, SIGNAL(newConnection()), this, SLOT(getNewConn()));

		ui.gameReady_client_BTN->setDisabled(true);
	}
	else if(plyRole == GUEST)
	{
		ui.color_me_client_LE->setText("白色");
		ui.color_rivar_client_LE->setText("黑色");
		tcpSocket_player = new QTcpSocket(this);
		tcpSocket_player->connectToHost(ui.addr_rivar_client_LE->text(),DataClass::connPort - 1);
		//连接成功
		connect(tcpSocket_player,SIGNAL(connected()),this,SLOT(connSuccessful()));
		//连接出错
		connect(tcpSocket_player,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connFailed()));
	}
}

void GameStatus::getNewConn()
{
	tcpSocket_player = tcpServer_player->nextPendingConnection();
	connect(tcpSocket_player, SIGNAL(readyRead()), this, SLOT(getNewDataFromClient()));
	ui.send_client_BTN->setDisabled(false);
}

void GameStatus::connSuccessful()
{
	connect(tcpSocket_player, SIGNAL(readyRead()), this, SLOT(getNewDataFromClient()));
	ui.send_client_BTN->setDisabled(false);
}

void GameStatus::connFailed()
{
	//和主机建立连接失败
// 	QMessageBox::information(NULL,"错误","和主机建立连接失败！");
// 	return ;
}

void GameStatus::setPara(QTcpSocket* _tcpSocket_server)
{
	tcpSocket_server = _tcpSocket_server;
}

void GameStatus::getNewDataFromClient()
{
	timer.stop();
	QDataStream in;
	msg_req_struct = new msg_request_struct();
	in.setDevice(tcpSocket_player);
	in >> msg_req_struct->request >> msg_req_struct->data;

	switch(msg_req_struct->request)
	{
	case COMM_CLIENT_QUITGAME:
		recvMsg_quitGame();
		break;
	case COMM_CLIENT_GAMESTART:
		recvMsg_gameStart();
		break;
    case COMM_CLIENT_GAMEOP:
		recvMsg_gameOP();
		break;
	case COMM_CLIENT_LOSE:
		recvMsg_lose();
		break;
	case COMM_CLIENT_GAMEOVER:
		recvMsg_gameOver();
		break;
	case COMM_CLIENT_CHAT:
		recvMsg_chat();
		break;
	case COMM_CLIENT_UNDO:
		recvMsg_undo();
		break;
	case COMM_CLIENT_UNDO_YES:
		recvMsg_undoYes();
		break;
	case COMM_CLIENT_UNDO_NO:
		recvMsg_undoNo();
		break;
	default:
		break;
	}
}

void GameStatus::recvMsg_quitGame()
{
	dataInit();
	ui.undo_client_BTN->setDisabled(true);
	ui.lose_client_BTN->setDisabled(true);
	ui.send_client_BTN->setDisabled(true);
	if(plyRole == HOST)
	{
		plyStatus = readyToPlayingGame;
		ui.chatLog_client_TE->clear();
		ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");
		ui.record_rivar_client_LE->setText("");
		ui.addr_rivar_client_LE->setText("");
		QMessageBox::information(NULL,"提示","对方退出游戏！");
	}
	else if(plyRole == GUEST)
	{
		ui.gameReady_client_BTN->setDisabled(false);
		closeByServer = true;
		this->close();
	}
}

void GameStatus::recvMsg_gameStart()
{
	dataInit();
	ui.gameReady_client_BTN->setDisabled(true);
	ui.undo_client_BTN->setDisabled(false);
	ui.lose_client_BTN->setDisabled(false);
	ui.send_client_BTN->setDisabled(false);
	ui.gameStatus_client_LB->setText("游戏开始！");
	plyStatus = playingGame;
	whosTurn = HOST;
	ui.lose_client_BTN->setDisabled(false);
	ui.undo_client_BTN->setDisabled(false);
}

void GameStatus::recvMsg_gameOP()
{	
	int pos = msg_req_struct->data.indexOf("_");
	int x = msg_req_struct->data.mid(0,pos).toInt();
	int y = msg_req_struct->data.mid(pos + 1, msg_req_struct->data.size() - pos).toInt();
	chessboardRecord[x - 1][y - 1] = (plyRole == HOST? -1 : 1);
	pieceRecord.push_back(qMakePair(QPoint(x,y), plyRole == HOST ? GUEST : HOST));
	whosTurn = plyRole;
	ui.gameStatus_client_LB->setText("该你下棋了！");

	update();
}

void GameStatus::recvMsg_lose()
{
	QMessageBox::information(NULL,"胜利","对方认输！");
	ui.gameStatus_client_LB->setText("对方认输！");
	win += 1;
	ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");
	ui.record_rivar_client_LE->setText("胜利：" + QString::number(lose) + "场，失败：" + QString::number(win) + "场！");
	if(plyRole == GUEST)
		ui.gameReady_client_BTN->setDisabled(false);
	ui.undo_client_BTN->setDisabled(true);
	ui.lose_client_BTN->setDisabled(true);
	plyStatus = readyToPlayingGame;
}

void GameStatus::recvMsg_gameOver()
{
	QMessageBox::information(NULL,"胜利","对方胜利！");
	ui.gameStatus_client_LB->setText("对方胜利！");
	lose += 1;
	ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");
	ui.record_rivar_client_LE->setText("胜利：" + QString::number(lose) + "场，失败：" + QString::number(win) + "场！");
	if(plyRole == GUEST)
		ui.gameReady_client_BTN->setDisabled(false);
	ui.undo_client_BTN->setDisabled(true);
	ui.lose_client_BTN->setDisabled(true);
	plyStatus = readyToPlayingGame;
}

void GameStatus::recvMsg_chat()
{
	ui.chatLog_client_TE->append("对手：  " + msg_req_struct->data);
}

void GameStatus::recvMsg_undo()
{
	if(DataClass::checkDialog("悔棋请求","是否答应对方悔棋请求？"))
	{
		DataClass::sendMsg(COMM_CLIENT_UNDO_YES,"",tcpSocket_player);
		if(pieceRecord.size() > 0)
		{	
			chessboardRecord[pieceRecord[pieceRecord.size() - 1].first.x()-1][pieceRecord[pieceRecord.size() - 1].first.y()-1] = 0;
			pieceRecord.removeAt(pieceRecord.size() - 1);
		}
		if(pieceRecord.size() > 0)
		{	
			chessboardRecord[pieceRecord[pieceRecord.size() - 1].first.x()-1][pieceRecord[pieceRecord.size() - 1].first.y()-1] = 0;
			pieceRecord.removeAt(pieceRecord.size() - 1);
		}
		update();
	}
	else
		DataClass::sendMsg(COMM_CLIENT_UNDO_NO,"",tcpSocket_player);
}

void GameStatus::recvMsg_undoYes()
{
	QMessageBox::information(NULL,"提示","对方同意悔棋！");
	whosTurn = plyRole;
	if(pieceRecord.size() > 0)
	{
		chessboardRecord[pieceRecord[pieceRecord.size() - 1].first.x()-1][pieceRecord[pieceRecord.size() - 1].first.y()-1] = 0;
		pieceRecord.removeAt(pieceRecord.size() - 1);
	}
	if(pieceRecord.size() > 0)
	{
		chessboardRecord[pieceRecord[pieceRecord.size() - 1].first.x()-1][pieceRecord[pieceRecord.size() - 1].first.y()-1] = 0;
		pieceRecord.removeAt(pieceRecord.size() - 1);
	}
	update();
}

void GameStatus::recvMsg_undoNo()
{
	whosTurn = plyRole;
	QMessageBox::information(NULL,"错误","对方不同意悔棋请求!");
}

void GameStatus::getNewDataFromServer()
{
	QDataStream in;
	msg_req_struct = new msg_request_struct();
	in.setDevice(tcpSocket_server);
	in >> msg_req_struct->request >> msg_req_struct->data;

	switch (msg_req_struct->request)
	{
	case COMM_CLIENT_JOIN:
		setPlayerInfo(msg_req_struct->data);
		break;
	default:
		break;
	}

	delete msg_req_struct;
	msg_req_struct = NULL;
}

void GameStatus::on_quit_client_BTN_clicked()
{
	closeByServer = false;
	this->close();
}

void GameStatus::on_send_client_BTN_clicked()
{
	if(ui.sendwords_client_LE->text() == "")
	{
		QMessageBox::information(NULL,"错误","请先输入文字！");
		return ;
	}
	ui.chatLog_client_TE->append("自己：  " + ui.sendwords_client_LE->text());
	DataClass::sendMsg(COMM_CLIENT_CHAT,ui.sendwords_client_LE->text(),tcpSocket_player);
	ui.sendwords_client_LE->setText("");
}

void GameStatus::on_lose_client_BTN_clicked()
{
	if(!(DataClass::checkDialog("确认","确认认输？")))
		return ;
	DataClass::sendMsg(COMM_CLIENT_LOSE,"",tcpSocket_player);		
	
	if(plyRole == GUEST)
		ui.gameReady_client_BTN->setDisabled(false);
	ui.undo_client_BTN->setDisabled(true);
	ui.lose_client_BTN->setDisabled(true);
	plyStatus = readyToPlayingGame;
	lose += 1;
	ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");
	ui.record_rivar_client_LE->setText("胜利：" + QString::number(lose) + "场，失败：" + QString::number(win) + "场！");
	return ;
}

void GameStatus::on_undo_client_BTN_clicked()
{
	if(whosTurn != plyRole)
	{
		QMessageBox::information(NULL,"错误","轮到你下棋的时候你才能发出悔棋请求！");
		return ;
	}
	if(!(DataClass::checkDialog("确认","确认悔棋？")))
		return ;
	whosTurn = (plyRole == HOST ? GUEST : HOST);
	DataClass::sendMsg(COMM_CLIENT_UNDO,"",tcpSocket_player);
}

void GameStatus::on_gameReady_client_BTN_clicked()
{
	if(plyRole == GUEST)
	{
		whosTurn = HOST;
		ui.gameStatus_client_LB->setText("游戏开始！");
		plyStatus = playingGame;
		DataClass::sendMsg(COMM_CLIENT_GAMESTART,"",tcpSocket_player);
		dataInit();
		ui.gameReady_client_BTN->setDisabled(true);
		ui.lose_client_BTN->setDisabled(false);
		ui.undo_client_BTN->setDisabled(false);
		ui.send_client_BTN->setDisabled(false);
	}
}

void GameStatus::paintEvent(QPaintEvent *)
{
 	QPainter painter(this);
	drawGrid(&painter);
	drawMouseArea(&painter);
	drawPieceRecord(&painter);
}

void GameStatus::mousePressEvent(QMouseEvent *)
{
	if(plyStatus != playingGame)
	{
		ui.gameStatus_client_LB->setText("游戏尚未开始！");
		return ;
	}
	if(whosTurn != plyRole)
	{
		ui.gameStatus_client_LB->setText("现在该对方操作！");
		return ;
	}
	QPoint point = mapFromGlobal(QCursor::pos());
	int curArrPos_x, curArrPos_y;
	curArrPos_x = qRound(float(point.x() - mouseMoveOffset_H) / gameCellSize);
	curArrPos_y = qRound(float(point.y() - mouseMoveOffset_W) / gameCellSize);
	if(curArrPos_x < 1 || curArrPos_x > 19 || curArrPos_y < 1 || curArrPos_y > 19)
		return ;
	if(chessboardRecord[curArrPos_x - 1][curArrPos_y - 1] != 0)
	{
		ui.gameStatus_client_LB->setText("此处已有棋子！");
		return ;
	}
	chessboardRecord[curArrPos_x - 1][curArrPos_y - 1] = (plyRole == HOST? 1 : -1);

	QPoint point_tmp(curArrPos_x, curArrPos_y);
	pieceRecord.push_back(qMakePair(point_tmp, plyRole));
	update();
	//检查是否胜利
	if(checkWin())
	{
		DataClass::sendMsg(COMM_CLIENT_GAMEOVER, "", tcpSocket_player);
		QMessageBox::information(NULL,"胜利","你胜利了！");
		ui.gameStatus_client_LB->setText("你胜利了！");
		if(plyRole == GUEST)
			ui.gameReady_client_BTN->setDisabled(false);
		ui.undo_client_BTN->setDisabled(true);
		ui.lose_client_BTN->setDisabled(true);
		plyStatus = readyToPlayingGame;
		win += 1;
		ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");
		ui.record_rivar_client_LE->setText("胜利：" + QString::number(lose) + "场，失败：" + QString::number(win) + "场！");
		return ;
	}
	ui.gameStatus_client_LB->setText("该对方下棋了！");
	whosTurn = (whosTurn == HOST? GUEST : HOST);
	//发送刚刚的落子信息
	DataClass::sendMsg(COMM_CLIENT_GAMEOP, QString::number(curArrPos_x) + "_" + QString::number(curArrPos_y), tcpSocket_player);

	connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
	timer.start(30000);
}

void GameStatus::timeout()
{
	//如果这个Socket还有效，说明其实没有超时，只是对方不知道干嘛去了！
	if(tcpSocket_player->isValid())
		return ;

	QMessageBox::information(NULL,"提示","对方超时!");
	dataInit();
	ui.undo_client_BTN->setDisabled(true);
	ui.lose_client_BTN->setDisabled(true);
	ui.send_client_BTN->setDisabled(true);
	//发送对方退出游戏通知
	if(plyRole == GUEST)
	{
		DataClass::sendMsg(COMM_CLIENT_LOSTCONN,"HOST " + ui.addr_rivar_client_LE->text(),tcpSocket_server);
		if(tcpSocket_player)
		{
			tcpSocket_player->close();
			tcpSocket_player = NULL;
		}
		this->close();
	}
	else if(plyRole == HOST)
	{
		DataClass::sendMsg(COMM_CLIENT_LOSTCONN,"GUEST " + ui.addr_rivar_client_LE->text(),tcpSocket_server);
		plyStatus = readyToPlayingGame;
		ui.chatLog_client_TE->clear();
		ui.record_me_client_LE->setText("胜利：" + QString::number(win) + "场，失败：" + QString::number(lose) + "场！");
		ui.record_rivar_client_LE->setText("");
		ui.addr_rivar_client_LE->setText("");
		if(tcpSocket_player)
		{
			tcpSocket_player->close();
			tcpSocket_player = NULL;
		}
		if(tcpServer_player)
		{
			tcpServer_player->close();
			tcpServer_player = NULL;
		}
	}
}

bool GameStatus::checkWin()
{
	int last_x = pieceRecord[pieceRecord.size() - 1].first.x() - 1;
	int last_y = pieceRecord[pieceRecord.size() - 1].first.y() - 1;
	int winColor = (plyRole == HOST ? 1 : -1);
	if(checkWinLURD(last_x, last_y, winColor))
		return true;
	if(checkWinUD(last_x, last_y, winColor))
		return true;
	if(checkWinRULD(last_x, last_y, winColor))
		return true;
	if(checkWinRL(last_x, last_y, winColor))
		return true;
	return false;
}

bool GameStatus::checkWinLURD(int last_x, int last_y, int winColor)
{
	for(int j=4; j>-1; j--)
	{
		if(last_x - j <0 || last_y - j < 0)
			continue;
		for(int z=0; z<5; z++)
		{
			if(last_x - j + z > 18 || last_y - j + z > 18)
				break;
			if(chessboardRecord[last_x - j + z][last_y - j + z] != winColor)
				break;
			else if(z == 4)
				return true;
		}
	}
	return false;
}

bool GameStatus::checkWinUD(int last_x, int last_y, int winColor)
{
	for(int j=4; j>-1; j--)
	{
		if(last_y - j < 0)
			continue;
		for(int z=0; z<5; z++)
		{
			if(last_y - j + z > 18)
				break;
			if(chessboardRecord[last_x][last_y - j + z] != winColor)
				break;
			else if(z == 4)
				return true;
		}
	}
	return false;
}

bool GameStatus::checkWinRULD(int last_x, int last_y, int winColor)
{
	for(int j=4; j>-1; j--)
	{
		if(last_x + j > 18 || last_y - j < 0)
			continue;
		for(int z=0; z<5; z++)
		{
			if(last_x + j - z < 0 || last_y - j + z > 18)
				break;
			if(chessboardRecord[last_x + j - z][last_y - j + z] != winColor)
				break;
			else if(z == 4)
				return true;
		}
	}
	return false;
}

bool GameStatus::checkWinRL(int last_x, int last_y, int winColor)
{
	for(int j=4; j>-1; j--)
	{
		if(last_x + j > 18)
			continue;
		for(int z=0; z<5; z++)
		{
			if(last_x + j - z < 0)
				break;
			if(chessboardRecord[last_x + j - z][last_y] != winColor)
				break;
			else if(z == 4)
				return true;
		}
	}
	return false;
}

void GameStatus::drawPieceRecord(QPainter* painter)
{
	for(int i=0; i<pieceRecord.size(); i++)
	{
		painter->setBrush(QBrush(pieceRecord[i].second == HOST ? Qt::black : Qt::white,Qt::SolidPattern));
		painter->setPen(Qt::black);
		int x = pieceRecord[i].first.x() * gameCellSize + mouseMoveOffset_H - 6;
		int y = pieceRecord[i].first.y() * gameCellSize + mouseMoveOffset_W + 2;
		painter->drawEllipse(x - pieceSize / 2,y  - pieceSize / 2, pieceSize, pieceSize);
	}

	if(pieceRecord.size() == 0)
		return ;
	int x = pieceRecord[pieceRecord.size() - 1].first.x() * gameCellSize + mouseMoveOffset_H - 6;
	int y = pieceRecord[pieceRecord.size() - 1].first.y() * gameCellSize + mouseMoveOffset_W + 2;
	painter->setPen(QPen(Qt::red,2));
	painter->drawLine(x - halfCellWidth, y - halfCellWidth, x - frameWidth, y - halfCellWidth);
	painter->drawLine(x - halfCellWidth, y + halfCellWidth, x - frameWidth, y + halfCellWidth);
	painter->drawLine(x + frameWidth, y - halfCellWidth, x + halfCellWidth, y - halfCellWidth);
	painter->drawLine(x + frameWidth, y + halfCellWidth, x + halfCellWidth, y + halfCellWidth);
	painter->drawLine(x - halfCellWidth, y - halfCellWidth, x - halfCellWidth, y - frameWidth);
	painter->drawLine(x - halfCellWidth, y + frameWidth, x - halfCellWidth, y + halfCellWidth);
	painter->drawLine(x + halfCellWidth, y - halfCellWidth, x + halfCellWidth, y - frameWidth);
	painter->drawLine(x + halfCellWidth, y + frameWidth, x + halfCellWidth, y + halfCellWidth);
}

void GameStatus::drawMouseArea(QPainter* painter)
{
	QPoint point = mapFromGlobal(QCursor::pos());
	int curArrPos_x, curArrPos_y;
	curArrPos_x = qRound(float(point.x() - mouseMoveOffset_H) / gameCellSize);
	curArrPos_y = qRound(float(point.y() - mouseMoveOffset_W) / gameCellSize);
	if(curArrPos_x < 1 || curArrPos_x > 19 || curArrPos_y < 1 || curArrPos_y > 19)
	{
		this->setCursor(QCursor(Qt::ArrowCursor));
		return ;
	}

	this->setCursor(QCursor(Qt::PointingHandCursor));
	int x, y;
	x = curArrPos_x* gameCellSize + mouseMoveOffset_H - 6;
	y = curArrPos_y* gameCellSize + mouseMoveOffset_W + 2;

	painter->setPen(QPen(Qt::green,1));
	painter->drawLine(x - halfCellWidth, y - halfCellWidth, x - frameWidth, y - halfCellWidth);
	painter->drawLine(x - halfCellWidth, y + halfCellWidth, x - frameWidth, y + halfCellWidth);
	painter->drawLine(x + frameWidth, y - halfCellWidth, x + halfCellWidth, y - halfCellWidth);
	painter->drawLine(x + frameWidth, y + halfCellWidth, x + halfCellWidth, y + halfCellWidth);
	painter->drawLine(x - halfCellWidth, y - halfCellWidth, x - halfCellWidth, y - frameWidth);
	painter->drawLine(x - halfCellWidth, y + frameWidth, x - halfCellWidth, y + halfCellWidth);
	painter->drawLine(x + halfCellWidth, y - halfCellWidth, x + halfCellWidth, y - frameWidth);
	painter->drawLine(x + halfCellWidth, y + frameWidth, x + halfCellWidth, y + halfCellWidth);
	update();
}

void GameStatus::drawGrid(QPainter* painter)
{
	//先画左边框
	int i=0;
	painter->setPen(QPen(Qt::black,4));
	painter->drawLine(i * gameCellSize + paintOffset_H, paintOffset_W, i * gameCellSize + paintOffset_H, 
		gameCellSize * (gameCellCount + 1) + paintOffset_W);

	//再画主棋谱
	painter->setPen(QPen(Qt::blue,2));
	for(i=1; i<gameCellCount + 1; i++)
	{
		painter->drawLine(i * gameCellSize + paintOffset_H, paintOffset_W, i * gameCellSize + paintOffset_H, 
			gameCellSize * (gameCellCount + 1) + paintOffset_W);
	}

	//最后画右边框
	painter->setPen(QPen(Qt::black,4));
	painter->drawLine(i * gameCellSize + paintOffset_H, paintOffset_W, i * gameCellSize + paintOffset_H, 
		gameCellSize * (gameCellCount + 1) + paintOffset_W);


	//先画上边框
	int j=0;
	painter->setPen(QPen(Qt::black,4));
	painter->drawLine(paintOffset_H + 3, j* gameCellSize + paintOffset_W, 
		gameCellSize * (gameCellCount + 1) + paintOffset_H - 3, j * gameCellSize + paintOffset_W);

	//再画主棋谱
	painter->setPen(QPen(Qt::blue,2));
	for(j=1; j<gameCellCount + 1; j++)
	{
		painter->drawLine(paintOffset_H + 3, j* gameCellSize + paintOffset_W, 
			gameCellSize * (gameCellCount + 1) + paintOffset_H - 3, j * gameCellSize + paintOffset_W);
	}

	//最后画下边框
	painter->setPen(QPen(Qt::black,4));
	painter->drawLine(paintOffset_H + 3, j* gameCellSize + paintOffset_W, 
		gameCellSize * (gameCellCount + 1) + paintOffset_H - 3, j * gameCellSize + paintOffset_W);
}

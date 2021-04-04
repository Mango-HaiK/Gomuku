#include "HomePage.h"
#include "ui_HomePage.h"
#include <QMouseEvent>
#include <QDebug>
#include <QtMath>
//
const int BoardSize = 15;

// 棋盘左右边缘空隙
const int BoardMarginLeft = 250;

// 棋盘上下边缘空隙
const int BoardMarginTop = 30;

// 棋子半径
const int Radius = 15;

// 落子标记边长
const int MarkSize = 6;

// 格子的大小
const int BlockSize = 40;
const int BlockHalfSize = 20;

// 鼠标点击的模糊距离上限
const int PosDelta = 20;


HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    //固定游戏界面大小
    setFixedSize(BoardMarginLeft * 2 + BlockSize * BoardSize,
                 BoardMarginTop * 2 + BlockSize *BoardSize);

    setMouseTracking(true);

    startForm = new StartGame();

    //绑定信号
    connect(startForm,&StartGame::actionPVEMode,this,&HomePage::initPVEGame);
    connect(startForm,&StartGame::actionPVPMode,this,&HomePage::initPVPGame);


    initGameInfo();
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //绘制棋盘
    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    for(int i = 0; i <= BoardSize; ++i)
    {
        painter.drawLine(BoardMarginLeft + BlockSize * i,BoardMarginTop,
                         BoardMarginLeft + BlockSize * i, size().height() - BoardMarginTop);

        painter.drawLine(BoardMarginLeft, BoardMarginTop + BlockSize * i,
                         size().width() - BoardMarginLeft, BoardMarginTop + BlockSize * i);
    }

    //绘制落子标记
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow > 0 && clickPosRow < BoardSize &&
            clickPosCol > 0 && clickPosCol < BoardSize &&
            game->boardStatusVec[clickPosRow][clickPosCol] == 0)
    {
        game->playerFlag ? brush.setColor(Qt::white) : brush.setColor(Qt::black);
        qDebug("hello");
        painter.setBrush(brush);
        painter.drawRect(BoardMarginLeft + clickPosCol * BlockSize - (MarkSize / 2),
                         BoardMarginTop + clickPosRow * BlockSize - (MarkSize / 2 ),
                         MarkSize,MarkSize);
    }

    //绘制棋子

    //判断对局是否结束
    if(clickPosRow > 0 && clickPosRow < BoardSize &&
            clickPosCol > 0 && clickPosCol < BoardSize &&
            (game->boardStatusVec[clickPosRow][clickPosCol] == 1 ||
             game->boardStatusVec[clickPosRow][clickPosCol] == -1))
    {
        if(game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            game->gameStatus = WIN;

            QString str = NULL;
            if(game->boardStatusVec[clickPosRow][clickPosCol] == 1)
                //白旗赢
                str = "White Win";
            else if(game->boardStatusVec[clickPosRow][clickPosCol] == -1)
                //黑棋赢
                str = "Black Win";

            //TODO 对局结束

            //？？？？
        }
    }

    //判断死局
    //if(game->isDead())
    {
        //TODO 对局结束
    }
}
void HomePage::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    //只能在棋盘落子
    if(x >= BoardMarginLeft + BlockHalfSize &&
            x < size().width() - BoardMarginLeft &&
            y >= BoardMarginTop + BlockHalfSize &&
            y < size().height() - BoardMarginTop)
    {
        //获取点的行和列
        int col = x / BlockSize;
        int row = y / BlockSize;

        //获取点的具体坐标
        int leftTopPosX = BoardMarginLeft + col * BlockSize;
        int leftTopPosY = BoardMarginTop + row * BlockSize;
        qDebug() << __FUNCTION__ << leftTopPosX << leftTopPosY;
        clickPosCol = clickPosRow = -1;
        int len = 0;
        qDebug() << __FUNCTION__ << len;
        //一个方框分成四个区域，根据区域内算出离哪个点最近，从而确定点击位置
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if(len < PosDelta)
        {
            qDebug("he");
            clickPosCol = col;
            clickPosRow = row;
        }

        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) +
                   (y - leftTopPosY) * (y - leftTopPosY));
        if(len < PosDelta)
        {
            clickPosCol = col + 1;
            clickPosRow = row;
        }

        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) +
                   (y - leftTopPosY - BlockSize) * (y - leftTopPosY- BlockSize));
        if(len < PosDelta)
        {
            clickPosCol = col;
            clickPosRow = row + 1;
        }

        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) +
                   (y - leftTopPosY- BlockSize) * (y - leftTopPosY- BlockSize));
        if(len < PosDelta)
        {
            clickPosCol = col + 1;
            clickPosRow = row + 1;
        }
    }

    update();
}

void HomePage::mouseReleaseEvent(QMouseEvent *event)
{

}

void HomePage::initGameInfo()
{
    game = new GameMode();

    //startForm->exec();

    initPVEGame();
}

void HomePage::initPVEGame()
{
    game_type = BOT;
    //人机模式直接开始不需要准备
    game->gameStatus = PLAYING;

    game->startGame(game_type);

    update();
}

void HomePage::initPVPGame()
{

}

void HomePage::chessOneByPerson()
{

}

void HomePage::chessOneByAI()
{

}

void HomePage::setSocket(QTcpSocket *socket)
{
    conn_server_socket = socket;
}

void HomePage::getNewDataFromServer()
{
    QDataStream in;
    mrt = new MsgRequestType();
    in.setDevice(conn_server_socket);
    in>>mrt->request>>mrt->data;


}

void HomePage::setPlayerRole(PlayerRole role)
{
    //role == Host => 建立连接等待Guest加入
    //role == Guest => 发起向主机的连接

    player_role = role;

    //TODO 初始化游戏信息
    initGameInfo();

    game->gameStatus = READ;

    if(role == HOST)
    {
        host_server = new QTcpServer(this);
        //TODO 在这里设置随机随机端口值
        //int rec = host_server->listen(QHostAddress::Any,DataClass::port + 1);
        if(1)
        {
            QMessageBox::information(this,"错误","主机监听端口失败,请重试!");
            //TODO 显示服务器状态窗口
            return;
        }
        connect(host_server,&QTcpServer::newConnection,this,&HomePage::getNewConn);
    }
    else
    {
        guest_socket = new QTcpSocket(this);
        //TODO 获取主机信息
        //guest_socket->connectToHost("127.0.0.1",DataClass::port + 1);

        //连接成功
        connect(guest_socket,&QTcpSocket::connected,this,&HomePage::connSucceed);

        //连接失败
        typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
        connect(guest_socket,static_cast<QAbstractSocketErrorSignal>(&QTcpSocket::error),
                this,&HomePage::connFail);

    }
}

void HomePage::setPlayerInfo(QString)
{
    //设置玩家信息
}

void HomePage::getNewConn()
{

}

void HomePage::connSucceed()
{

}

void HomePage::connFail()
{

}

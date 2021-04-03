#include "HomePage.h"
#include "ui_HomePage.h"

//
const int BoradSize = 15;

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

// 鼠标点击的模糊距离上限
const int PosDelta = 20;


HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    //固定游戏界面大小
    setFixedSize(BoardMarginLeft * 2 + BlockSize * BoradSize,
                 BoardMarginTop * 2 + BlockSize *BoradSize);
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

    for(int i = 0; i <= BoradSize; ++i)
    {
        painter.drawLine(BoardMarginLeft + BlockSize * i,BoardMarginTop,
                         BoardMarginLeft + BlockSize * i, size().height() - BoardMarginTop);

        painter.drawLine(BoardMarginLeft, BoardMarginTop + BlockSize * i,
                         size().width() - BoardMarginLeft, BoardMarginTop + BlockSize * i);
    }

    //绘制落子标记
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

}
void HomePage::mouseMoveEvent(QMouseEvent *event)
{

}

void HomePage::mouseReleaseEvent(QMouseEvent *event)
{

}

void HomePage::initGameInfo()
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

    player_status = READY;

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

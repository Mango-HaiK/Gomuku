#include "HomePage.h"
#include "ui_HomePage.h"
#include <QMouseEvent>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <QDateTime>

//
const int BoardSize = 15;

// 棋盘右边缘空隙
const int BoardMarginRight = 250;

// 棋盘上下边缘空隙
const int BoardMargin = 30;

// 棋子半径
const int Radius = 15;

// 落子标记边长
const int MarkSize = 6;

// 格子的大小
const int BlockSize = 40;
const int BlockHalfSize = 20;

// 鼠标点击的模糊距离上限
const int PosDelta = 20;

//AI思考时间
const int AIDelay = 500;


HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    //固定游戏界面大小
    setFixedSize(BoardMargin * 2 + BoardMarginRight + BlockSize * BoardSize,
                 BoardMargin * 2 + BlockSize *BoardSize);

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
        painter.drawLine(BoardMargin + BlockSize * i,BoardMargin,
                         BoardMargin + BlockSize * i, size().height() - BoardMargin);

        painter.drawLine(BoardMargin, BoardMargin + BlockSize * i,
                         size().width() - BoardMargin - BoardMarginRight, BoardMargin + BlockSize * i);
    }

    //绘制落子标记
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow > 0 && clickPosRow < BoardSize &&
            clickPosCol > 0 && clickPosCol < BoardSize &&
            game->boardStatusVec[clickPosRow][clickPosCol] == 0)
    {
        if(game_type == PERSON && game->turnFlag == GUEST)
            game->playerFlag ? brush.setColor(Qt::black) : brush.setColor(Qt::white);
        else
            game->playerFlag ? brush.setColor(Qt::white) : brush.setColor(Qt::black);

        painter.setBrush(brush);
        painter.drawRect(BoardMargin + clickPosCol * BlockSize - (MarkSize / 2),
                         BoardMargin + clickPosRow * BlockSize - (MarkSize / 2 ),
                         MarkSize,MarkSize);
    }

    //绘制棋子
    for (int i = 0; i < BoardSize; ++i)
    {
        for (int j = 0; j < BoardSize; ++j)
        {
            if(game->boardStatusVec[i][j] == 1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(BoardMargin + j * BlockSize - Radius,BoardMargin + i * BlockSize - Radius,
                                    Radius * 2,Radius * 2);

            }else if(game->boardStatusVec[i][j] == -1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(BoardMargin + j * BlockSize - Radius,BoardMargin + i * BlockSize - Radius,
                                    Radius * 2,Radius * 2);

            }
        }
    }
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
            if(game->isWin(clickPosRow,clickPosCol))
            {
                qDebug() << str ;
            }
            //？？？？
        }
    }

    //判断死局
    if(game->isDead())
    {
        //TODO 对局结束
    }
}
void HomePage::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() <<__FUNCTION__<<game->gameStatus <<" "<<game->playerFlag;
    if(!game->playerFlag || game->gameStatus == READ)
        return;

    int x = event->x();
    int y = event->y();

    //只能在棋盘落子
    if(x >= BoardMargin + BlockHalfSize &&
            x < size().width() - BoardMargin &&
            y >= BoardMargin + BlockHalfSize &&
            y < size().height() - BoardMargin)
    {
        //获取最近的最上角点的行和列
        int col = x / BlockSize;
        int row = y / BlockSize;

        int leftTopPosX = BoardMargin + col * BlockSize;
        int leftTopPosY = BoardMargin + row * BlockSize;
        //qDebug() << event->x() << " - "<<event->y();
        //qDebug() << __FUNCTION__ << leftTopPosX << leftTopPosY;
        clickPosCol = clickPosRow = -1;
        int len = 0;

        //一个方框分成四个区域，根据区域内算出离哪个点最近，从而确定点击位置
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if(len < PosDelta)
        {
            //qDebug("he");
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
    qDebug() <<__FUNCTION__<<game->gameStatus <<" "<<game->playerFlag;
    if(!game->playerFlag || game->gameStatus == READ)
        return;

    if(game_type == PERSON)
    {
        chessOneByPerson();
    }else if(!(game_type == BOT && !game->playerFlag))
    {
        //人下棋
        chessOneByPerson();
        if(game->gameType == BOT && !game->playerFlag)
            QTimer::singleShot(AIDelay,this,&HomePage::chessOneByAI);
    }
}

void HomePage::closeEvent(QCloseEvent *event)
{
    if(!DataClass::checkMsgBox("退出","确认退出游戏？"))
    {
        event->ignore();
        return ;
    }
    game->~GameMode();
}

void HomePage::initGameInfo()
{
    game = new GameMode();

    startForm->exec();

    ui->btn_undo->setDisabled(false);
    ui->btn_send_char_msg->setDisabled(false);
    //initPVPGame();
}

void HomePage::recvMsgGameReady(PlayerRole role)
{
    ui->btn_send_char_msg->setDisabled(true);
    ui->btn_undo->setDisabled(true);
    qDebug() << __FUNCTION__ << role;
    if(role == HOST)
    {
        ui->btn_ready->setDisabled(true);
        ui->edit_playerA_name->setText("白棋");
        ui->edit_playerB_name->setText("黑棋");
        ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                        " 系统提示: 等待玩家加入...");
    }else
    {
        ui->btn_send_char_msg->setDisabled(false);
        ui->btn_ready->setDisabled(false);
        ui->edit_playerA_name->setText("黑棋");
        ui->edit_playerB_name->setText("白棋");
        ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                        " 系统提示: 已加入游戏,赶紧准备开始游戏吧！");
    }
}

void HomePage::recvMsgGameStart()
{
    ui->btn_undo->setDisabled(false);
    ui->btn_currender->setDisabled(false);
    ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                    " 系统提示: 对局已开始！");

}

void HomePage::recvPlayerJoin()
{
    if(game->getCurrRole() == HOST)
    {
        ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                        " 系统提示: 已有玩家加入，等待对手准备开始游戏！");
        ui->btn_send_char_msg->setDisabled(false);
    }
}

void HomePage::recvMsgChat(QString msg)
{
    ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                    " 旗鼓相当的对手:"+ msg);
}

void HomePage::listenErrorDispos()
{

}

void HomePage::initPVEGame()
{
    game_type = BOT;
    //人机模式直接开始不需要准备
    game->gameStatus = PLAYING;

    game->readyGame(game_type);

    update();
}

void HomePage::initPVPGame()
{
    game_type = PERSON;
    game->gameStatus = READ;

    game->readyGame(game_type);

    connect(game,&GameMode::listenError,this,&HomePage::listenErrorDispos);

    //游戏开始
    connect(game,&GameMode::gameStart,
            this,&HomePage::recvMsgGameStart);

    //游戏准备
    connect(game,SIGNAL(MsgGameReady(PlayerRole)),
            this,SLOT(recvMsgGameReady(PlayerRole)));

    //接受消息
    connect(game,SIGNAL(MsgChat(QString)),
            this,SLOT(recvMsgChat(QString)));

    //玩家加入
    connect(game,&GameMode::PlayerJoin,
            this,&HomePage::recvPlayerJoin);
    ui->text_edit_chat_info->clear();

    update();
}

void HomePage::chessOneByPerson()
{
    //坐标有效且无子
    if(clickPosRow != -1 && clickPosCol != -1 &&
            game->boardStatusVec[clickPosRow][clickPosCol] == 0)
    {
        game->actionByPerson(clickPosRow,clickPosCol);
        update();
    }
}

void HomePage::chessOneByAI()
{
    qDebug() << __FUNCTION__;
    game->actionByAI(clickPosRow,clickPosCol);
    update();
}

void HomePage::on_btn_send_char_msg_clicked()
{
    if(ui->edit_send_msg->text() == "")
        return;
    QString msg = ui->edit_send_msg->text();
    ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                    " 自己:"+ msg);
    DataClass::sendMsg(COMM_CLIENT_SENCHAT,msg,game->player_socket);

    ui->edit_send_msg->setText("");
}

void HomePage::on_btn_ready_clicked()
{
    game->recvMsgGameStart();
    recvMsgGameStart();
}

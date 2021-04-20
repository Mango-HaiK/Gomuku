#include "HomePage.h"
#include "ui_HomePage.h"
#include "helpdiglog.h"
#include <QMouseEvent>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <QDateTime>
#include <QSound>
#define CHESS_ONE_SOUND ":/sound/chessone.wav"
#define WIN_SOUND ":/sound/win.wav"
#define LOSE_SOUND ":/sound/lose.wav"
#define BLACK_ICO ":/image/black.ico"
#define WHITE_ICO ":/image/white.ico"

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

    ui->lab_firstA->hide();
    ui->lab_fristB->hide();
    startForm = new StartGame();
    on_chess_time = new QTimer();
    on_chess_time->setInterval(1000);
    connect(on_chess_time,&QTimer::timeout,this,&HomePage::timeOut);
    //绑定信号
    connect(startForm,&StartGame::actionPVEMode,this,&HomePage::initPVEGame);
    connect(startForm,&StartGame::actionPVPMode,this,&HomePage::initPVPGame);

    ui->btn_undo->setDisabled(true);


    initGameInfo();
}

void HomePage::timeOut()
{
    //QMessageBox::information(this,"提示","你的对手等的花都谢了");
}
HomePage::~HomePage()
{
    if(startForm)  delete startForm;
    if(game) delete game;
    if(on_chess_time) delete on_chess_time;
    delete ui;
}


void HomePage::setTextInfo(QString msg)
{
    ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                    " "+msg);

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
    QBrush brush1;
    brush1.setStyle(Qt::SolidPattern);
    brush1.setColor(Qt::black);
    painter.setBrush(brush1);
    painter.drawRect(BoardMargin + 8 * BlockSize - (MarkSize / 2),
                     BoardMargin + 7 * BlockSize - (MarkSize / 2 ),
                     MarkSize,MarkSize);
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
    //输赢
        if(game->isWin(clickPosRow,clickPosCol))
        {
            checkWin();
            return;
        }
    }

    //判断死局
    if(game->isDead())
    {
        QMessageBox::information(this,"提示","死局，请开启下一局游戏！");
    }

    //场上棋子数大于8且不小于6时才能悔棋
    if(game->gameStatus == PLAYING &&
            game->playerFlag &&
            game->actionNum >= 8)
    {
        ui->btn_undo->setDisabled(false);   //关闭悔棋按钮
    }
    if(game->actionNum <= 6 || !game->playerFlag)
        ui->btn_undo->setDisabled(true);    //开启悔棋按钮
}
void HomePage::mouseMoveEvent(QMouseEvent *event)
{
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
        if(len < PosDelta)
        {
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
void HomePage::mousePressEvent(QMouseEvent *event)
{

}
void HomePage::closeEvent(QCloseEvent *event)
{
    if(!DataClass::checkMsgBox("退出","确认退出游戏？"))
    {
        event->ignore();
        return ;
    }else
        event->accept();
}

void HomePage::checkWin()
{
    //判断对局是否结束
    if(clickPosRow > 0 && clickPosRow < BoardSize &&
            clickPosCol > 0 && clickPosCol < BoardSize &&
            (game->boardStatusVec[clickPosRow][clickPosCol] == 1 ||
             game->boardStatusVec[clickPosRow][clickPosCol] == -1))
    {
        if(game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            QString str = NULL;
            if(game->boardStatusVec[clickPosRow][clickPosCol] == 1)
            {
                str = "游戏结束白棋获胜，再来一局吧！";
            }
            else if(game->boardStatusVec[clickPosRow][clickPosCol] == -1)
            {
                str = "游戏结束黑棋获胜，再来一局吧！";
            }
            QMessageBox::StandardButton btnValue = QMessageBox::information(this,"",str);
            //TODO 对局结束
            qDebug() << str ;
            if(btnValue == QMessageBox::Ok && game_type == PERSON)
            {

            }else
            {
                game->readyGame(game_type);
                game->gameStatus = PLAYING;
            }
            game->initBoard();
        }
    }
}
void HomePage::initGameInfo()
{
    game = new GameMode();

    startForm->exec();

    //initPVPGame();
}

void HomePage::recvMsgGameReady(PlayerRole role)
{
    ui->btn_send_char_msg->setDisabled(true);
    //ui->btn_undo->setDisabled(true);
    if(role == HOST)
    {
        ui->btn_ready->setDisabled(true);
        QPixmap pix1(WHITE_ICO);

        ui->lab_playerA_chess->setPixmap(pix1);

        QPixmap pix2(BLACK_ICO);
        ui->lab_playerB_chess->setPixmap(pix2);

        ui->lab_firstA->show();
        ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                        "系统提示: 等待玩家加入...");
    }else
    {
        ui->btn_send_char_msg->setDisabled(false);
        ui->btn_ready->setDisabled(false);

        QPixmap pix1(WHITE_ICO);
        ui->lab_playerB_chess->setPixmap(pix1);

        QPixmap pix2(BLACK_ICO);
        ui->lab_playerA_chess->setPixmap(pix2);
        ui->lab_fristB->show();
        setTextInfo("系统提示: 已加入游戏,赶紧准备开始游戏吧！");
    }
}

void HomePage::recvMsgGameStart()
{
    on_chess_time->start();
    //ui->btn_undo->setDisabled(false);
    ui->btn_currender->setDisabled(false);
    ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                    " 系统提示: 对局已开始！");
}

void HomePage::recvPlayerJoin()
{
    if(game->getCurrRole() == HOST)
    {
        setTextInfo("系统提示: 已有玩家加入，等待对手准备开始游戏！");
        ui->btn_send_char_msg->setDisabled(false);
    }
}

void HomePage::recvMsgChat(QString msg)
{
    setTextInfo("你的对手:"+ msg);
}

void HomePage::listenErrorDispos()
{

}

void HomePage::initPVEGame()
{
    game_type = BOT;
    //人机模式直接开始不需要准备
    game->gameStatus = PLAYING;
    QPixmap pix1(WHITE_ICO);
    ui->lab_playerA_chess->setPixmap(pix1);

    QPixmap pix2(BLACK_ICO);
    ui->lab_playerB_chess->setPixmap(pix2);

    ui->lab_firstA->show();

    ui->btn_ready->setDisabled(true);
    ui->btn_undo->setDisabled(true);
    ui->btn_send_char_msg->setDisabled(true);

    game->readyGame(game_type);
    setTextInfo("系统提示：游戏开始啦！");
    update();
}

void HomePage::initPVPGame()
{
    game_type = PERSON;
    game->gameStatus = READ;

    game->readyGame(game_type);

    server_status = game->getServerStatus();

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

    connect(game,&GameMode::MsgUndo,
            this,&HomePage::undo);

    connect(game,SIGNAL(isAgreeUndo(bool)),
            this,SLOT(isAgreeUndo(bool)));

    connect(game,&GameMode::MsgPlayerQuit,
            this,&HomePage::PlayerQuit);

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
        QSound::play(CHESS_ONE_SOUND);
        update();
    }
}

void HomePage::chessOneByAI()
{
    qDebug() << __FUNCTION__;
    game->actionByAI(clickPosRow,clickPosCol);
    QSound::play(CHESS_ONE_SOUND);
    update();
}

void HomePage::on_btn_send_char_msg_clicked()
{
    if(ui->edit_send_msg->text() == "")
        return;
    QString msg = ui->edit_send_msg->text();
    ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                    " 自己:"+ msg);
    DataClass::sendMsg(MSG_CLIENT_SENCHAT,msg,game->player_socket);

    ui->edit_send_msg->setText("");
}

void HomePage::on_btn_ready_clicked()
{
    game->recvMsgGameStart();
    recvMsgGameStart();
}

void HomePage::on_btn_undo_clicked()
{
    if(game_type == PERSON)
    {
        ui->btn_undo->setDisabled(true);
        ui->text_edit_chat_info->append(QDateTime::currentDateTime().toString("hh:mm") +
                                        " 系统提示:正在请求对方悔棋...");
    }
    game->undo();
    update();
}

void HomePage::undo()
{
    if(DataClass::checkMsgBox("提示","对方请求悔棋是否同意？"))
    {
        game->actionUndo();
        game->sendUndoInfo(true);
    }else
    {
        game->sendUndoInfo(false);
    }
}

void HomePage::isAgreeUndo(bool flag)
{
    if(flag)
        QMessageBox::information(this,"提示","对方同意悔棋！");
    else
        QMessageBox::information(this,"提示","对方拒绝悔棋！");

    ui->btn_undo->setDisabled(false);
}

void HomePage::on_btn_currender_clicked()
{

    //玩家退出当前游戏
    if(DataClass::checkMsgBox("提示","是否退出当前对局？"))
    {
        if(game_type == BOT)
        {
            startForm->show();
            return;
        }

        game->playerQuit();
    }
    else
        return;
}

void HomePage::PlayerQuit()
{
    QMessageBox::information(this,"提示","玩家已退出游戏");
}


void HomePage::on_btn_help_clicked()
{
    helpDiglog *hd = new helpDiglog();
    hd->exec();
}

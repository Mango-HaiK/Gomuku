#include "gameMode.h"
#include <QMessageBox>

const int BoardSize = 15;


GameMode::GameMode()
{

}
void GameMode::initBoard()
{
    //初始化棋盘
    boardStatusVec.clear();
    for(int i = 0; i < BoardSize; ++i)
    {
        vector<int> lineBoard;
        for(int j = 0; j < BoardSize; ++j)
            lineBoard.push_back(0);
        boardStatusVec.push_back(lineBoard);
    }
}

void GameMode::startGame(GameType type)
{
    gameType = type;

    initBoard();

    // PVP 初始化网络对战信息
    if(gameType == PERSON)
    {
        server_status = new ServerStatus();
        server_status->exec();

        player_role = server_status->getNetPlayerInfo()->role;

        setPlayerRole(player_role);

    }
    //PVE 需要初始化评分--？？？
    if(gameType == BOT)
    {
        scoreVec.clear();
        for(int i = 0; i < BoardSize; ++i)
        {
            vector<int> lineBoard;
            for(int j = 0; j < BoardSize; ++j)
                lineBoard.push_back(0);
            scoreVec.push_back(lineBoard);
        }
    }


    playerFlag = true;
}

bool GameMode::isWin(int row, int col)
{

    bool win_flag = 0 ,flag = false;
    int count = 1;
    int temp_row = row;
    int temp_col = col;

    //记录四个方向的移动，横、竖、左斜、右斜
    int dir[4][2][2] = {
        { {-1, 0}, {1, 0} },
        { {0, -1}, {0, 1} },
        { {-1, -1}, {1, 1} },
        { {1,-1}, {-1, 1} }
    };

    for(int i = 0; i < 4; ++i)
    {
        count = 1;
        //j等于 0，1时分别表示该棋子两边当方向
        for(int j = 0; j < 2 ;++j)
        {
            flag = true;

            //循环向其中一个方向遍历，如果有棋子颜色相同则count++，否则结束在该方向的遍历
            while(flag)
            {
                temp_col = temp_col + dir[i][j][0];
                temp_row = temp_row + dir[i][j][1];

                //棋盘边界判断
                if(temp_col >= 0 && temp_col <= BoardSize &&
                        temp_row >= 0 && temp_row <= BoardSize)
                    boardStatusVec[temp_row][temp_col] == boardStatusVec[row][col] ? ++count : flag = false;
                else
                    flag = false;
            }
            temp_col = col;
            temp_row = row;
        }
        //在一个方向上存在5连
        if(count >= 5)
        {
            win_flag = true;
            break;
        }
    }
    return win_flag;
}

bool GameMode::isDead()
{
    //棋盘已经全部填满
    for(int i = 1 ;i < BoardSize ; ++i)
        for(int j = 1 ;j < BoardSize; ++j)
        {
            if(!(boardStatusVec[i][j] == -1 || boardStatusVec[i][j] == 1))
                return false;
        }
    return true;
}

void GameMode::actionByPerson(int row, int col)
{
    updateBoardVec(row,col);
}

void GameMode::actionByAI(int row, int col)
{

}

void GameMode::updateBoardVec(int row, int col)
{
    playerFlag ? boardStatusVec[row][col] = 1 : boardStatusVec[row][col] = -1;

    playerFlag = !playerFlag;
}

void GameMode::setSocket(QTcpSocket *socket)
{
    conn_server_socket = socket;
}

void GameMode::getNewDataFromServer()
{
    QDataStream in;
    mrt = new MsgRequestType();
    in.setDevice(conn_server_socket);
    in>>mrt->request>>mrt->data;
}

void GameMode::setPlayerRole(PlayerRole role)
{
    //role == Host => 建立连接等待Guest加入
    //role == Guest => 发起向主机的连接

    player_role = role;

    //TODO 初始化游戏信息
    //initGameInfo();

    gameStatus = READ;

    if(role == HOST)
    {
        host_server = new QTcpServer(this);
        //TODO 在这里设置随机随机端口值
        int rec = host_server->listen(QHostAddress::Any,DataClass::port + 1);
        if(!rec)
        {
            //发送监听失败信号
            emit listenError();
            server_status->show();
            return;
        }
        connect(host_server,&QTcpServer::newConnection,this,&GameMode::getNewConn);
    }
    else
    {
        player_socket = new QTcpSocket(this);
        //TODO 获取主机信息
        //guest_socket->connectToHost("127.0.0.1",DataClass::port + 1);

        //连接成功
        connect(player_socket,&QTcpSocket::connected,this,&GameMode::connSucceed);

        //连接失败
        typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
        connect(player_socket,static_cast<QAbstractSocketErrorSignal>(&QTcpSocket::error),
                this,&GameMode::connFail);

    }
}

void GameMode::setPlayerInfo(QString)
{
    //设置玩家信息
}

void GameMode::getNewConn()
{
    //接收到新的连接 - 处理
    player_socket = host_server->nextPendingConnection();
    //connect(player_socket,&QTcpSocket::readyRead,this,&GameMode::getNewDataFromClinet);
}

void GameMode::connSucceed()
{

}

void GameMode::connFail()
{

}

void GameMode::getNewDataFromClient()
{
    QDataStream in;
    mrt = new MsgRequestType();

    in.setDevice(player_socket);
    in >> mrt->request >> mrt->data;
    switch (mrt->request) {
    case COMM_CLIENT_GAMESTART:
        recvMsgGameStart();
        break;

    }
}

void GameMode::recvMsgGameStart()
{
    initBoard();

}

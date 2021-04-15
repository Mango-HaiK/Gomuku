#include "gameMode.h"
#include <QMessageBox>

const int BoardSize = 15;


GameMode::GameMode()
{
    mrt = new MsgRequestType();
}

GameMode::~GameMode()
{
    if(player_role == GUEST)
    {
        //关闭和HOST方的连接
        //然后向HOST方发送退出信息
        DataClass::sendMsg(COMM_CLIENT_CONNLOSE,"GUEST",conn_server_socket);
        if(player_socket)
        {
            DataClass::sendMsg(COMM_CLIENT_OFFCONN,"",player_socket);
            player_socket->close();
            player_socket = NULL;
        }
    }
    else if(player_role == HOST)
    {
        //关闭和GUEST方的连接
        //然后向GUEST方发送退出信息
        DataClass::sendMsg(COMM_CLIENT_CONNLOSE,"HOST",conn_server_socket);
        if(player_socket)
        {
            DataClass::sendMsg(COMM_CLIENT_OFFCONN,"",player_socket);
            player_socket->close();
            player_socket = NULL;
        }
        if(host_server)
        {
            host_server->close();
            host_server = NULL;
        }
    }
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

void GameMode::readyGame(GameType type)
{
    gameType = type;
    initBoard();
    // PVP 初始化网络对战信息
    if(gameType == PERSON)
    {
        server_status = new ServerStatus();
        //这里需要对 Host or Guest 分别处理

        server_status->show();
        //进入房间 -
        connect(server_status,SIGNAL(createRoom(PlayerRole)),
                this,SLOT(setPlayerRole(PlayerRole)));
        playerFlag = false;
    }
    //PVE 需要初始化评分--
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
    playerFlag = true;
    }
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
                    boardStatusVec[temp_row][temp_col] == boardStatusVec[row][col] ?
                                ++count : flag = false;
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
    if(gameType == PERSON)
        DataClass::sendMsg(COMM_CLIENT_ONCHESS,
                           QString::number(row)+"_"+QString::number(col),player_socket);
}

void GameMode::actionByAI(int row, int col)
{
    // 计算评分
    calculateScore();

    // 从评分中找出最大分数的位置
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;

    for (int row = 1; row < BoardSize; row++)
        for (int col = 1; col < BoardSize; col++)
        {
            // 前提是这个坐标是空的
            if (boardStatusVec[row][col] == 0)
            {
                if (scoreVec[row][col] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = scoreVec[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreVec[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }

    // 随机落子，如果有多个点的话
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();

    std::pair<int, int> pointPair = maxPoints.at(index);
    row = pointPair.first; // 记录落子点
    col = pointPair.second;
    updateBoardVec(row, col);
}

void GameMode::calculateScore()
{
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数

    // 清空评分数组
    scoreVec.clear();
    for (int i = 0; i < BoardSize; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j < BoardSize; j++)
            lineScores.push_back(0);
        scoreVec.push_back(lineScores);
    }

    // 计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI智能程度以及攻守风格）
    for (int row = 0; row < BoardSize; row++)
        for (int col = 0; col < BoardSize; col++)
        {
            // 空白点就算
            if (row > 0 && col > 0 &&
                boardStatusVec[row][col] == 0)
            {
                // 遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // 原坐标不算
                        if (!(y == 0 && x == 0))
                        {
                            // 每个方向延伸4个子

                            // 对玩家白子评分（正反两个方向）
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < BoardSize &&
                                    col + i * x > 0 && col + i * x < BoardSize &&
                                    boardStatusVec[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BoardSize &&
                                         col + i * x > 0 && col + i * x < BoardSize &&
                                         boardStatusVec[row + i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < BoardSize &&
                                    col - i * x > 0 && col - i * x < BoardSize &&
                                    boardStatusVec[row - i * y][col - i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < BoardSize &&
                                         col - i * x > 0 && col - i * x < BoardSize &&
                                         boardStatusVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (personNum == 1)                      // 杀二
                                scoreVec[row][col] += 10;
                            else if (personNum == 2)                 // 杀三
                            {
                                if (emptyNum == 1)
                                    scoreVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreVec[row][col] += 40;
                            }
                            else if (personNum == 3)                 // 杀四
                            {
                                // 量变空位不一样，优先级不一样
                                if (emptyNum == 1)
                                    scoreVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreVec[row][col] += 110;
                            }
                            else if (personNum == 4)                 // 杀五
                                scoreVec[row][col] += 10100;

                            // 进行一次清空
                            emptyNum = 0;

                            // 对AI黑子评分
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < BoardSize &&
                                    col + i * x > 0 && col + i * x < BoardSize &&
                                    boardStatusVec[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < BoardSize &&
                                         col + i * x > 0 && col + i * x < BoardSize &&
                                         boardStatusVec[row +i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < BoardSize &&
                                    col - i * x > 0 && col - i * x < BoardSize &&
                                    boardStatusVec[row - i * y][col - i * x] == -1) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < BoardSize &&
                                         col - i * x > 0 && col - i * x < BoardSize &&
                                         boardStatusVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (botNum == 0)                      // 普通下子
                                scoreVec[row][col] += 5;
                            else if (botNum == 1)                 // 活二
                                scoreVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)                // 死三
                                    scoreVec[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreVec[row][col] += 50;  // 活三
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)                // 死四
                                    scoreVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreVec[row][col] += 100; // 活四
                            }
                            else if (botNum >= 4)
                                scoreVec[row][col] += 10000;   // 活五

                        }
                    }
            }
        }
}

void GameMode::updateBoardVec(int row, int col)
{
    if(player_role != GUEST)
        playerFlag ? boardStatusVec[row][col] = 1 : boardStatusVec[row][col] = -1 ;
    else
        playerFlag ? boardStatusVec[row][col] = -1 : boardStatusVec[row][col] = 1 ;

    playerFlag = !playerFlag;

    if(gameType == PERSON)
        turnFlag = (turnFlag == HOST ? GUEST : HOST);

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

    if(mrt->request == COMM_CLIENT_JOIN)
        //TODO 对手加入游戏
        return;
}

void GameMode::setPlayerRole(PlayerRole role)
{
    //role == Host => 建立连接等待Guest加入
    //role == Guest => 发起向主机的连接
    qDebug()<<__FUNCTION__<<role;
    player_role = role;

    gameStatus = READ;

    if(player_role == HOST)
    {
        host_server = new QTcpServer(this);

        int rec = host_server->listen(QHostAddress::Any,DataClass::port + 1);
        if(!rec)
        {
            //发送监听失败信号
            emit listenError();
            server_status->show();
            return;
        }
        //准备
        emit MsgGameReady(HOST);

        connect(host_server,&QTcpServer::newConnection,this,&GameMode::getNewConn);
    }
    else
    {
        player_socket = new QTcpSocket();
        //TODO 获取主机信息
        player_socket->connectToHost(server_status->getHostSocket(),DataClass::port + 1);

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
    connect(player_socket,&QTcpSocket::readyRead,this,&GameMode::getNewDataFromClient);
    emit PlayerJoin();
}

void GameMode::connSucceed()
{
    qDebug()<<__FUNCTION__;
    connect(player_socket,&QTcpSocket::readyRead,this,&GameMode::getNewDataFromClient);
    emit MsgGameReady(GUEST);
}

void GameMode::connFail()
{
    qDebug()<<__FUNCTION__;
    server_status->show();
    QMessageBox::information(server_status,"错误","连接失败！");
}

PlayerRole GameMode::getCurrRole()
{
    return  player_role;
}

void GameMode::getNewDataFromClient()
{
    QDataStream in;

    in.setDevice(player_socket);
    in>>mrt->request>>mrt->data;
    qDebug() << __FUNCTION__ <<mrt->request <<" "<<mrt->data;

    switch (mrt->request) {
    case COMM_CLIENT_GAMESTART:
        recvMsgGameStart();
        break;
    case COMM_CLIENT_SENCHAT:
        emit MsgChat(mrt->data);
        break;
    case COMM_CLIENT_ONCHESS:
        recvMsgOnchess();
        break;
    case COMM_CLIENT_LOSE:
        break;
    case COMM_CLIENT_UNDO:
        break;
    case COMM_CLIENT_UNDOYES:
        break;
    case COMM_CLIENT_UNDONO:
        break;
    }
}

void GameMode::recvMsgOnchess()
{
    int pos = mrt->data.indexOf("_");
    int row = mrt->data.mid(0,pos).toInt();
    int col = mrt->data.mid(pos + 1, mrt->data.size() - pos).toInt();

    updateBoardVec(row,col);
}

void GameMode::recvMsgGameStart()
{
    turnFlag = HOST;
    gameStatus = PLAYING;
    if(player_role == GUEST)
    {
        DataClass::sendMsg(COMM_CLIENT_GAMESTART,"",player_socket);
        playerFlag = false;
    }else
    {
        emit gameStart();
        playerFlag = true;
    }
}

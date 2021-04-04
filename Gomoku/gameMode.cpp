#include "gameMode.h"

const int BoardSize = 15;


GameMode::GameMode()
{

}

void GameMode::startGame(GameType type)
{
    gameType = type;

    //初始化棋盘
    boardStatusVec.clear();
    for(int i = 0; i < BoardSize; ++i)
    {
        vector<int> lineBoard;
        for(int j = 0; j < BoardSize; ++j)
            lineBoard.push_back(0);
        boardStatusVec.push_back(lineBoard);
    }

    //PVE 需要初始化评分
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
    for(int i = 1 ;i < BoardSize ; ++i)
        for(int j = 1 ;j < BoardSize; ++j)
        {
            if(!(boardStatusVec[i][j] == -1 || boardStatusVec[i][j] == 1))
                return false;
        }
    return  true;
}

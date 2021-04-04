#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <vector>

using std::vector;

//游戏类型
enum GameType
{
    PERSON,
    BOT
};

//游戏状态
enum GameStatus
{
    PLAYING,
    WIN,
    READ
};

class GameMode
{
public:
    GameMode();


    //存储当前棋盘棋子的情况 - 无棋子 -> 0 | 黑棋 -> -1 | 白棋 -> 1
    vector<vector<int>> boardStatusVec;

    //存储各点位评分情况，用于AI下棋
    vector<vector<int>> scoreVec;

    //当前下棋的一方
    bool playerFlag;

    //游戏模式
    GameType gameType;

    //游戏状态
    GameStatus gameStatus;

    //开始游戏
    void startGame(GameType type);

    //判断输赢
    bool isWin(int row, int col);

    //判断死局
    bool isDead();


};

#endif // GAMEMODE_H

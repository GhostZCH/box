#pragma once
#include "Tool.h"



enum GAME_STATE
{
	HALT,//暂停
	GO,  //工作
	STOP //停止
};


class Game
{
private:
	int *BigNet;
	int *SmallNet;
	GAME_STATE state;

	Tool *tool;
	Tool *nextTool;

	// 积木位置，以4x4方格的左上角定义位置
	int Loc_X;
	int Loc_Y;

public:
	static const int TIME_STEP = 500;
	static const int NET_WIDTH = 9;
	static const int NET_HEIGHT = 20;

private:
    void AddTool(int *net); // 将方块添加到堆中
	void NextTool();// 引出下一个积木

	bool IsDead();

	bool CanMoveDown();
	bool CanMoveLeft();
	bool CanMoveRight();
	bool CanRoll();
	void Roll();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void RemoveLines();
	void RemoveLine(int index);
	bool CanRemoveLine(int index);

public:
	int  Height;
    int  Width; 

public:
	Game(int height,int width);
	~Game(void);

	int* GetBigNet(); 
	int* GetSmallNet() ;
    GAME_STATE GetState();

	void Start();    // 开始
	bool Go();       // 运行一步,游戏结束时返回false
    void HaltOrContinue();  // 暂停或继续
	void Input(UINT nChar);
};

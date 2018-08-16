#include "StdAfx.h"
#include "Game.h"


Game::Game(int height,int width)
{
	Width    = width;
	Height   = height;

	BigNet   = NULL;
	SmallNet = NULL;

	tool     = NULL;
	nextTool = NULL;

	state = STOP;
}

Game::~Game(void)
{
	delete(BigNet);
	delete(SmallNet);
}

int*  Game::GetBigNet()
{
	int *result = (int *)malloc(sizeof(int)*Height*Width);
	AddTool(result);
	return result;
}

int* Game::GetSmallNet()
{
	return this->SmallNet;
}

GAME_STATE Game::GetState()
{
	return state;
}


// 开始
void Game::Start()
{
	tool     = NULL;
	nextTool = NULL;
	state = GO;

	BigNet   = (int *)malloc(sizeof(int)*Height*Width);
	SmallNet = (int *)malloc(sizeof(int)*4*4);

	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			BigNet[i*Width+j] =0;

	NextTool();
	NextTool();
}


// 暂停或继续
void Game::HaltOrContinue()
{
	if(state == HALT)
		state = GO;
	else
		if(state == GO)
			state = HALT;
}

// 运行一步
bool Game::Go()
{
	if(CanMoveDown())
	{
		MoveDown();
		return true;
	}
	else
	{
		AddTool(this->BigNet);
		RemoveLines();
		if(IsDead())
		{
			state = STOP;
			return false;
		}
		else
		{
			NextTool();
			return true;
		}
	}	
}

void Game::Input(UINT nChar)
{
	switch(nChar)
	{
	case VK_UP   : if(CanRoll())     Roll()     ;break;	
	case VK_DOWN : if(CanMoveDown()) MoveDown() ;break;
	case VK_LEFT : if(CanMoveLeft()) MoveLeft() ;break;
	case VK_RIGHT: if(CanMoveRight()) MoveRight();break;
	}

}

void Game::AddTool(int *net)
{
	if(net!=NULL && tool!=NULL && BigNet!=NULL)
	{
		if(net!=BigNet)
			for(int i=0;i<Height;i++)
				for(int j=0;j<Width;j++)
					net[i*Width+j] = BigNet[i*Width+j];

		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				if( i+Loc_Y>=0      &&
					i+Loc_Y<Height  &&
					j+Loc_X>=0      &&
					j+Loc_X<Width   &&
					tool->GetData()[i*4+j])
					net[(i+Loc_Y)*Width+j+Loc_X] = 1;
			}
	}
}

void Game::NextTool()
{
	// 小窗口积木出场，更换新的积木，设置出场位置
	Loc_Y = 0;
	Loc_X = (Width-4)/2;
	tool = nextTool;

	//更新小窗口信息
	srand((unsigned int) time(0));
	nextTool = new Tool(rand()%6+1);
	SmallNet = nextTool->GetData();
}

bool Game::IsDead()
{
	int cnt1 = 4,cnt2=0,x = (Width-4)/2,y = 0;
	int *tmp = (int *)malloc(sizeof(int)*Height*Width);

	// 复制一个副本，统计原有方块数+tool中的块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
		{
			tmp[i*Width+j] = BigNet[i*Width+j];
			cnt1 += tmp[i*Width+j];
		}

	// 假设发生变换
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if( i+y>=0      &&
				i+y<Height  &&
				j+x>=0      &&
				j+x<Width   &&
				nextTool->GetData()[i*4+j])
				tmp[(i+y)*Width+j+x] = 1;
		}

	// 统计变换后方块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			cnt2 += tmp[i*Width+j];

	delete(tmp);

	return cnt2!=cnt1;
}


bool Game::CanMoveDown()
{
	int cnt1 = 4,cnt2=0,x = Loc_X,y = Loc_Y+1;
	int *tmp = (int *)malloc(sizeof(int)*Height*Width);

	// 复制一个副本，统计原有方块数+tool中的块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
		{
			tmp[i*Width+j] = BigNet[i*Width+j];
			cnt1 += tmp[i*Width+j];
		}

	// 假设发生变换
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if( i+y>=0      &&
				i+y<Height  &&
				j+x>=0      &&
				j+x<Width   &&
				tool->GetData()[i*4+j])
				tmp[(i+y)*Width+j+x] = 1;
		}

	// 统计变换后方块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			cnt2 += tmp[i*Width+j];

	delete(tmp);

	return cnt2==cnt1;
}


bool Game::CanMoveLeft()
{
	int cnt1 = 4,cnt2=0,x = Loc_X-1,y = Loc_Y;
	int *tmp = (int *)malloc(sizeof(int)*Height*Width);

	// 复制一个副本，统计原有方块数+tool中的块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
		{
			tmp[i*Width+j] = BigNet[i*Width+j];
			cnt1 += tmp[i*Width+j];
		}

	// 假设发生变换
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if( i+y>=0      &&
				i+y<Height  &&
				j+x>=0      &&
				j+x<Width   &&
				tool->GetData()[i*4+j])
				tmp[(i+y)*Width+j+x] = 1;
		}

	// 统计变换后方块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			cnt2 += tmp[i*Width+j];

	delete(tmp);

	return cnt2==cnt1;
}

bool Game::CanMoveRight()
{
	int cnt1 = 4,cnt2=0,x = Loc_X+1,y = Loc_Y;
	int *tmp = (int *)malloc(sizeof(int)*Height*Width);

	// 复制一个副本，统计原有方块数+tool中的块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
		{
			tmp[i*Width+j] = BigNet[i*Width+j];
			cnt1 += tmp[i*Width+j];
		}

	// 假设发生变换
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if( i+y>=0      &&
				i+y<Height  &&
				j+x>=0      &&
				j+x<Width   &&
				tool->GetData()[i*4+j])
				tmp[(i+y)*Width+j+x] = 1;
		}

	// 统计变换后方块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			cnt2 += tmp[i*Width+j];

	delete(tmp);

	return cnt2==cnt1;
}

bool Game::CanRoll()
{
	int cnt1 = 4,cnt2=0,x = Loc_X,y = Loc_Y;
	int *tmp = (int *)malloc(sizeof(int)*Height*Width);
	int *rollData = tool->GetRollData();

	// 复制一个副本，统计原有方块数+tool中的块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
		{
			tmp[i*Width+j] = BigNet[i*Width+j];
			cnt1 += tmp[i*Width+j];
		}

	// 假设发生变换
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if( i+y>=0      &&
				i+y<Height  &&
				j+x>=0      &&
				j+x<Width   &&
				rollData[i*4+j])
				tmp[(i+y)*Width+j+x] = 1;
		}

	// 统计变换后方块数
	for(int i=0;i<Height;i++)
		for(int j=0;j<Width;j++)
			cnt2 += tmp[i*Width+j];

	delete(tmp);

	return cnt2==cnt1;
}

void Game::RemoveLines()
{
	for(int i=Height-1;i>0;i--)
		while(CanRemoveLine(i))
			RemoveLine(i);
}

void Game::RemoveLine(int index)
{
	for(int i=index;i>0;i--)
		for(int j=0;j<Width;j++)
			BigNet[i*Width+j] = BigNet[(i-1)*Width+j];

	for(int j=0;j<Width;j++)
		BigNet[j] = 0;
}

bool Game::CanRemoveLine(int index)
{
	int count = 0;

	for(int i=0;i<Width;i++)
		if(BigNet[index*Width+i]==1)
			count ++;

	return count==Width;
}

void Game::Roll()
{
	if(tool)
		tool->Roll();
}

void Game::MoveDown()
{
	Loc_Y++;
}

void Game::MoveLeft()
{
	Loc_X--;
}

void Game::MoveRight()
{
	Loc_X++;
}


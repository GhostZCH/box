#include "StdAfx.h"
#include "Tool.h"
#include <Windows.h>
#include <tchar.h>


Tool::Tool(int type)
{
	_type  = type;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			_data[i][j]=0;

	switch(_type)
	{
	case 1:Make_1();break;
	case 2:Make_2();break;
	case 3:Make_3();break;
	case 4:Make_4();break;
	case 5:Make_5();break;
	case 6:Make_6();break;
	}
}


Tool::~Tool(void)
{
	delete(_data);
}

//---
void Tool::Make_1()
{
	_data[1][0]=_data[1][1]=_data[1][2]=_data[1][3] = 1;
}

//[ ]
void Tool::Make_2()
{
	_data[1][1]=_data[1][2]=_data[2][1]=_data[2][2] = 1;
}
// p
void Tool::Make_3()
{
	_data[1][1]=_data[1][2]=_data[2][1]=_data[3][1] = 1;
}
// q
void Tool::Make_4()
{
	_data[1][0]=_data[1][1]=_data[2][1]=_data[3][1] = 1;
}
// Z
void Tool::Make_5()
{
	_data[1][0]=_data[1][1]=_data[2][1]=_data[2][2] = 1;
}
// op_z
void Tool::Make_6()
{
	_data[2][0]=_data[2][1]=_data[1][1]=_data[1][2] = 1;
}


int Tool::GetType()
{
	return _type;
}

int* Tool::GetData()
{
	return &_data[0][0];
}

void Tool::Roll()
{
	int tmp[4][4];

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			tmp[i][j] = _data[i][j];

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			_data[i][j] = tmp[j][3-i];
}

int* Tool::GetRollData()
{
	int tmp[4][4];
	int *result = (int *)malloc(sizeof(int)*16);

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			tmp[i][j] = _data[i][j];

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			result[i*4+j] = tmp[j][3-i];

	return result;
}

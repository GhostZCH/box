#pragma once

class Tool
{
protected:
	int _data[4][4];
	int _type;

private:
	void Make_1();//---
	void Make_2();//[ ]
	void Make_3();// p
	void Make_4();// q
	void Make_5();// Z
	void Make_6();// op_z

public:
	Tool(int type);
	~Tool(void);

	int GetType();
	int *GetData();

	void Roll();
	int* GetRollData();
};


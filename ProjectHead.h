//��Ϣ�๲������
#pragma once
#include<string>
#include<sstream>
#include<Windows.h>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using namespace std;
//�ṹ�嶨��

typedef struct//��Ϣ������Ϣ
{
	int type;//����
}strObjectInfo;//������������Ϣ�йص���Ϣ��������Ϣ�Ľ����뷢�ͣ�

wstring s2ws(const string& s);

typedef struct 
{
	int up;
	int down;
	int right;
	int left;
	int space;
	int ctrl;
} controlInfo;
#pragma once
#include "EngineMessage.h"
class gameObject
{
public:
	int iNum;
	char cName[10] ;
	int iType;

	int getMessage(EngineMessage M);
	int getMessageAll();
	gameObject(void);
	~gameObject(void);
};


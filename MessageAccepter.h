#pragma once

#include"EngineMessage.h"
#include"ProjectHead.h"
class MessageAccepter
{
public:
//	 strObjectInfo getObjectInfo();//�õ�������Ϣ
	// int accepteMessage(EngineMessage *pM,int iAmount);//������Ϣ
	virtual void getControl(controlInfo CI);
	MessageAccepter(void);
	~MessageAccepter(void);
};


#pragma once

#include"EngineMessage.h"
#include"ProjectHead.h"
class MessageAccepter
{
public:
//	 strObjectInfo getObjectInfo();//得到对象信息
	// int accepteMessage(EngineMessage *pM,int iAmount);//接受消息
	virtual void getControl(controlInfo CI);
	MessageAccepter(void);
	~MessageAccepter(void);
};


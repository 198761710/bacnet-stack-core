#ifndef __MSTP_H__
#define __MSTP_H__
#include <map>
#include <list>
#include "comport.h"
#include "recvframe.h"
#include "sendframe.h"
#include "timeoperator.h"
using namespace std;

class Token
{
public:
	bool ismaster;
	TimeOperator t;
	unsigned char sudomaster;
public:
	Token(void):ismaster(0){}
};

typedef enum
{
	FrameToken = 0x00,
	FramePollMaster = 0x01,
	FramePollMasterAck = 0x02,
	FrameTestRequest = 0x03,
	FrameTestRequestAck = 0x04,
	FrameDataRequest = 0x05,
	FrameDataRequestAck = 0x06,
}FrameType;

class Mstp
{
protected:
	Token token;
	ComPort com;
	RecvFrame frame;
	list<SendFrame> rqlist;
	map<unsigned char, TimeOperator> mastermap;

public:
	Mstp(void);
	Mstp(const string&, int ,int, int, int);

public:
	bool Run(void);
	bool Open(const string&);
	bool Set(int, int, int, int);
	bool ProcFrame(RecvFrame& f);
	bool ProcToken(RecvFrame& f);
	bool ProcPollMaster(RecvFrame& f);
	bool ProcPollMasterAck(RecvFrame& f);
	bool ProcTestRequest(RecvFrame& f);
	bool ProcTestRequestAck(RecvFrame& f);
	bool ProcDataRequest(RecvFrame& f);
	bool ProcDataRequestAck(RecvFrame& f);
};

#endif//__MSTP_H__

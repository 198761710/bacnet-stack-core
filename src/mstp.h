#ifndef __MSTP_H__
#define __MSTP_H__
#include <map>
#include <list>
#include "comport.h"
#include "recvframe.h"
#include "sendframe.h"
#include "timeoperator.h"
using namespace std;

typedef signed   char s8;
typedef unsigned char u8;
typedef signed   int  s32;
typedef unsigned int  u32;
typedef signed   int  int32_t;
typedef unsigned int  uint32_t;

typedef enum
{
	StateIdle = 0,
	StateToken = 1,
	StateRequest = 2,
}StateType;

typedef enum
{
	CmdGetValue = 0,
	CmdSetValue = 1,
}CmdType;

typedef enum
{
	FrameToken			= 0x00,
	FramePollMaster		= 0x01,
	FramePollMasterAck	= 0x02,
	FrameTestRequest	= 0x03,
	FrameTestRequestAck	= 0x04,
	FrameDataRequest	= 0x05,
	FrameDataRequestAck	= 0x06,
}FrameType;

class RecvRate
{
public:
	u8 mac;
	u32 rcount;
	u32 scount;
	float rate;
public:
	RecvRate(void):mac(0),rcount(0),scount(0),rate(0.0){};
	RecvRate(u8 m, u32 r, u32 s, float R):mac(m),rcount(r),scount(s),rate(R){}
};
class Offline
{
public:
	TimeOperator retry;
	TimeOperator recvtime;
};
class Master
{
public:
	StateType state;
	TimeOperator t;
	unsigned char master;
	unsigned char sudomaster;
	unsigned char invokeid;
public:
	Master(void):state(StateIdle),master(0),sudomaster(0),invokeid(0){}
	Master(u8 m, u8 s, u8 i):state(StateIdle),master(s),sudomaster(s),invokeid(i){}
};
class Instance
{
public:
	unsigned char cmd;
	unsigned char dst;
	unsigned int type;
	unsigned int instance;
	union{
		float f;
		int32_t i;
		uint32_t u;
	}value;
	string name;

public:
	Instance(void)
		:cmd(0),dst(0),type(0),instance(0){value.i=0;}
	Instance(u8 c, u8 d, u32 t, u32 i)
		:cmd(c),dst(d),type(t),instance(i){value.i=0;}
	Instance(u8 c, u8 d, u32 t, u32 i, u32 v)
		:cmd(c),dst(d),type(t),instance(i){value.u=v;}
	Instance(u8 c, u8 d, u32 t, u32 i, float v)
		:cmd(c),dst(d),type(t),instance(i){value.f=v;}
public:
};
#define InstanceU(c, d, t, i, v) Instance(c, d, (u32)t, (u32)i, (u32)v)
#define InstanceF(c, d, t, i, v) Instance(c, d, (u32)t, (u32)i, (float)v)

class Mstp
{
protected:
	int retry;
	bool recvok;
	ComPort com;
	Master master;
	RecvFrame frame;
	map<u8,u32> recvcount;
	map<u8,u32> sendcount;
	TimeOperator sendtime;
	TimeOperator recvtime;
	TimeOperator recving;
	list<Instance> instancelist;
	list<Instance> valuelist;
	map<char, Offline> offline;
	map<char, TimeOperator> mastermap;
	bool(*getvalue)(const string&,double,const string&);

public:
	Mstp(void);
	Mstp(const string&, int ,int, int, int);

public:
	bool Run(void);
	void StateMachine(void);
	void DoRequest(void);
	void DoReadRequest(void);
	void DoWriteRequest(void);
	void Add(const Instance&);
	void PushBack(const Instance&);
	void PushFront(const Instance&);
	void PushBack(list<Instance>&);
	void PushFront(list<Instance>&);
	bool Empty(void);
	bool Open(const string&);
	bool Set(int, int, int, int);
	bool Close(void);

public:
	bool ProcFrame(RecvFrame& f);
	bool ProcToken(RecvFrame& f);
	bool ProcPollMaster(RecvFrame& f);
	bool ProcPollMasterAck(RecvFrame& f);
	bool ProcTestRequest(RecvFrame& f);
	bool ProcTestRequestAck(RecvFrame& f);
	bool ProcDataRequest(RecvFrame& f);
	bool ProcDataRequestAck(RecvFrame& f);

public:
	void InitRecvRate(void);
	void GetRecvRate(list<RecvRate>& rlist);
	string GetName(Instance&);
	void GetValue( bool(*f)(const string&, double, const string&) );
	list<Instance>& ValueList(void);
};
#endif//__MSTP_H__

#ifndef __RECVFRAME_H__
#define __RECVFRAME_H__
#include <deque>
#include "config.h"
#include "timeoperator.h"
using namespace std;

class DataNode
{
public:
	TimeOperator t;
	unsigned char c;
public:
	DataNode(void):c(0){}
	DataNode(unsigned char x):c(x){}
};

typedef enum
{
	HeadIndex0	= 0,
	HeadIndex1	= 1,
	TypeIndex	= 2,
	DstIndex	= 3,
	SrcIndex	= 4,
	DlenIndex0	= 5,
	DlenIndex1	= 6,
	HcrcIndex	= 7,
	NpduIndex0	= 8,
	NpduIndex1	= 9,
	ApduIndex	= 10,
	MaxIndex	= MAX_APDU - ApduIndex - 2,
}RecvIndex;
class RecvFrame
{
protected:
	unsigned int m_length;
	unsigned char m_buffer[1024];
	deque<DataNode> m_queue;

public:
	RecvFrame(void);

public:
	bool check(void);
	void clear(void);
	void push(unsigned char c);

public:
	unsigned char  type(void);
	unsigned char  dst(void);
	unsigned char  src(void);
	unsigned short dlen(void);
	unsigned char  hcrc(void);
	unsigned int   length(void);
	unsigned char* data(void);
	unsigned char* npdu(void);
	unsigned char* apdu(void);
	unsigned short dcrc(void);
	unsigned char  chcrc(void);
	unsigned short cdcrc(void);
};

#endif//__RECVFRAME_H__

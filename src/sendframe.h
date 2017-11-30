#ifndef __SENDFRAME_H__
#define __SENDFRAME_H__
#include "recvframe.h"

typedef unsigned char u8;

class SendFrame
{
protected:
	unsigned int  m_length;
	unsigned char m_buffer[MAX_APDU];

public:
	SendFrame(void);

public:
	unsigned char* data(void);
	unsigned int   length(void);

public:
	bool check(void);
	void clear(void);
	void showhex(void);

public:
	unsigned char  type(void);
	unsigned char  dst(void);
	unsigned char  src(void);
	unsigned short dlen(void);
	unsigned char  hcrc(void);
	unsigned short dcrc(void);
	unsigned char  chcrc(void);
	unsigned short cdcrc(void);
	void mstp(u8 type, u8 dst, u8 src);
	void apdu(u8 type, u8 dst, u8 src, u8* buf, int len);
};

#endif//__SENDFRAME_H__

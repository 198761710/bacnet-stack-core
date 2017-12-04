#include <stdio.h>
#include "crc.h"
#include "recvframe.h"

#define debug_recvfailed()  \
do{\
	int len = m_queue.size(); \
	if( len > 0 ) \
	{\
		printf("%s.", __func__); \
		printf("%d:[", __LINE__); \
	}\
	for(int i = 0; i < len; i++) \
	{\
		printf("%02X ", m_queue[i].c); \
	}\
	if( len > 0 )printf("\b]\n");\
}while(0)

RecvFrame::RecvFrame(void)
{
	m_length = 0;
}
void RecvFrame::clear(void)
{
	m_queue.clear();
}
void RecvFrame::showhex(void)
{
	if( m_queue.empty() )
	{
		printf("m_queue.size(0)\n");
		return;
	}
	printf("recvframe.[");
	m_length = m_queue.size();
	for(int i = 0; i < m_length; i++)
	{
		printf("%02X ", m_queue[i].c);
	}
	printf("\b].check(%d)\n", check());
}
void RecvFrame::push(unsigned char c)
{
	m_queue.push_back(DataNode(c));
}
bool RecvFrame::check(void)
{
	if( m_queue.empty() )
	{
		return false;
	}
	if( m_queue[0].t.mdiff() > 500 )
	{
		debug_recvfailed();
		m_queue.pop_front();
		if( m_queue.empty() )
		{
			return false;
		}
		while( m_queue[0].t.mdiff() > 500 )
		{
			m_queue.pop_front();
			if( m_queue.empty() )
			{
				return false;
			}
		}
	}
	if( 0x55 != m_queue[0].c )
	{
		debug_recvfailed();
		m_queue.pop_front();
		if( m_queue.empty() )
		{
			return false;
		}
		while( m_queue[0].c != 0x55 )
		{
			m_queue.pop_front();
			if( m_queue.empty() )
			{
				return false;
			}
		}
	}
	m_length = m_queue.size();
	if(m_length < 8 )
	{
		return false;
	}
	if( 0xff != m_queue[1].c )
	{
		debug_recvfailed();
		m_queue.pop_front();
		return false;
	}
	if( hcrc() != chcrc() )
	{
		debug_recvfailed();
		m_queue.pop_front();
		return false;
	}
	if( dlen() == 0 )
	{
		return true;
	}
	if( (dlen() + NpduIndex0 + 2) > m_length )
	{
		return false;
	}
	if( (dlen() + NpduIndex0 + 2) < m_length )
	{
		debug_recvfailed();
		m_queue.pop_front();
		return false;
	}
	if( dcrc() != cdcrc() )
	{
		debug_recvfailed();
		m_queue.pop_front();
		return false;
	}
	return true;
}
unsigned char  RecvFrame::type(void)
{
	return m_queue[TypeIndex].c;
}
unsigned char  RecvFrame::dst(void)
{
	return m_queue[DstIndex].c;
}
unsigned char  RecvFrame::src(void)
{
	return m_queue[SrcIndex].c;
}
signed int RecvFrame::dlen(void)
{
	signed int l = 0;

	l = (m_queue[DlenIndex0].c << 8) | m_queue[DlenIndex1].c;

	return l %= MAX_APDU;
}
unsigned char  RecvFrame::hcrc(void)
{
	return m_queue[HcrcIndex].c;
}
unsigned int RecvFrame::length(void)
{
	return m_queue.size();
}
unsigned char* RecvFrame::data(void)
{
	m_length = m_queue.size();
	for(int i = 0; i < m_length; i++)
	{
		m_buffer[i] = m_queue[i].c;
	}
	return m_buffer;
}
unsigned char* RecvFrame::npdu(void)
{
	return data() + NpduIndex0;
}
unsigned char* RecvFrame::apdu(void)
{
	return data() + ApduIndex;
}
unsigned short RecvFrame::dcrc(void)
{
	m_length = m_queue.size();
	if( dlen() != m_length - NpduIndex0 - 2 )
	{
		return 0;
	}
	return (m_queue[m_length-1].c << 8) | m_queue[m_length-2].c;
}
unsigned char  RecvFrame::chcrc(void)
{
	unsigned char crc = 0xff;

	for(int i = TypeIndex; i < HcrcIndex; i++)
	{
		crc = CRC_Calc_Header(m_queue[i].c, crc);
	}
	return ~crc;
}
unsigned short RecvFrame::cdcrc(void)
{
	unsigned short crc = 0xffff;

	m_length = m_queue.size() - 2;
	for(int i = NpduIndex0; i < m_length; i++)
	{
		crc = CRC_Calc_Data(m_queue[i].c, crc);
	}
	return ~crc;
}

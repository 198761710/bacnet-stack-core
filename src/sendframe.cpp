#include <stdio.h>
#include <string.h>
#include "sendframe.h"
#include "crc.h"

SendFrame::SendFrame(void)
{
	m_length = 0;
}
unsigned char* SendFrame::data(void)
{
	return m_buffer;
}
unsigned int SendFrame::length(void)
{
	return m_length;
}
bool SendFrame::check(void)
{
	if( 0x55 != m_buffer[HeadIndex0] )
	{
		printf("%s.%d\n", __func__, __LINE__);
		return false;
	}
	if( 0xff != m_buffer[HeadIndex1] )
	{
		printf("%s.%d\n", __func__, __LINE__);
		return false;
	}
	if( chcrc() != m_buffer[HcrcIndex] )
	{
		printf("%s.%d\n", __func__, __LINE__);
		return false;
	}
	if( dlen() != (m_length - NpduIndex0 - 2) )
	{
		printf("%s.%d\n", __func__, __LINE__);
		return false;
	}
	if( dcrc() != cdcrc() )
	{
		printf("%s.%d\n", __func__, __LINE__);
		return false;
	}
	return true;
}
unsigned char  SendFrame::type(void)
{
	return m_buffer[TypeIndex];
}
unsigned char  SendFrame::dst(void)
{
	return m_buffer[DstIndex];
}
unsigned char  SendFrame::src(void)
{
	return m_buffer[SrcIndex];
}
unsigned short SendFrame::dlen(void)
{
	return (m_buffer[DlenIndex0] << 8) | m_buffer[DlenIndex1];
}
unsigned char SendFrame::hcrc(void)
{
	return m_buffer[HcrcIndex];
}
unsigned short SendFrame::dcrc(void)
{
	m_length %= MaxIndex;
	return (m_buffer[m_length-1] << 8) | m_buffer[m_length-2];
}
unsigned char SendFrame::chcrc(void)
{
	unsigned char crc = 0xff;

	for(int i = TypeIndex; i < HcrcIndex; i++)
	{
		crc = CRC_Calc_Header(m_buffer[i], crc);
	}
	return ~crc;
}
unsigned short SendFrame::cdcrc(void)
{
	int len = m_length - 2;
	unsigned short crc = 0xffff;

	for(int i = NpduIndex0; i < len; i++)
	{
		crc = CRC_Calc_Data(m_buffer[i], crc);
	}

	return ~crc;
}
void SendFrame::apdu(u8 type, u8 dst, u8 src, u8* buf, int len)
{
	if( len < 1 )
	{
		return;
	}
	if( NULL == buf )
	{
		return;
	}
	if( len > MaxIndex )
	{
		len = MaxIndex;
	}
	m_buffer[HeadIndex0] = 0x55;
	m_buffer[HeadIndex1] = 0xff;
	m_buffer[TypeIndex]  = type;
	m_buffer[DstIndex]   = dst;
	m_buffer[SrcIndex]   = src;
	m_buffer[DlenIndex0] = (len+2) >> 8;
	m_buffer[DlenIndex1] = (len+2) >> 0;
	m_buffer[HcrcIndex]  = chcrc();
	m_buffer[NpduIndex0] = 0x01;
	m_buffer[NpduIndex1] = 0x04;
	memcpy(m_buffer+ApduIndex, buf, len);
	m_length = ApduIndex + len + 2;
	unsigned short crc = cdcrc();
	m_buffer[m_length-2] = crc >> 0;
	m_buffer[m_length-1] = crc >> 8;
}

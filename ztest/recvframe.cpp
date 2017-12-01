#include <stdio.h>
#include <unistd.h>
#include "recvframe.h"

int main(void)
{
	RecvFrame frame;
	unsigned char buf[] = {
		0x55, 0xff, 0x05, 0x09, 0x01, 0x00, 
		0x0D, 0xCD, 0x01, 0x04, 0x00, 0x03, 
		0x3F, 0x0C, 0x0C, 0x04, 0xC0, 0x00, 
		0x65, 0x19, 0x55, 0xAE, 0x8F
	};
	for(int i = 0; i < sizeof(buf); i++)
	{
		frame.push(buf[i]);
	}

	printf("frame.type  = [%02X]\n", frame.type());
	printf("frame.src   = [%02X]\n", frame.src());
	printf("frame.dst   = [%02X]\n", frame.dst());
	printf("frame.dlen  = [%04X]\n", frame.dlen());
	printf("frame.hcrc  = [%02X]\n", frame.hcrc());
	printf("frame.chcrc = [%02X]\n", frame.chcrc());
	printf("frame.dcrc  = [%04X]\n", frame.dcrc());
	printf("frame.cdcrc = [%04X]\n", frame.cdcrc());

	printf("frame.[");
	for(int i = 0; i < frame.length(); i++)
	{
		printf("%02X ", frame.data()[i]);
	}
	printf("\b].check(%d)\n", frame.check());

	return 0;
}

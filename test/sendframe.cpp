#include <stdio.h>
#include <unistd.h>
#include "sendframe.h"
#include "apdu.h"

int main(void)
{
	Apdu apdu;
	SendFrame frame;
	
	unsigned char buf[] = {
		0x55, 0xff, 0x05, 0x09, 0x01, 0x00, 
		0x0D, 0xCD, 0x01, 0x04, 0x00, 0x03, 
		0x3F, 0x0C, 0x0C, 0x04, 0xC0, 0x00, 
		0x65, 0x19, 0x55, 0xAE, 0x8F
	};
	apdu.BinaryInput(10, 102);
	//apdu.AckBinaryInput(10, 102, 1);
	//frame.apdu(5, 9, 1, buf+ApduIndex, sizeof(buf)-ApduIndex-2);
	frame.apdu(5, 9, 1, apdu.GetApdu(), apdu.Length());
	printf("frame.type=%02X\n", frame.type());
	printf("frame.src=%02X\n", frame.src());
	printf("frame.dst=%02X\n", frame.dst());
	printf("frame.dlen=%04X\n", frame.dlen());
	printf("frame.hcrc=%02X\n", frame.hcrc());
	printf("frame.chcrc=%02X\n", frame.chcrc());
	printf("frame.dcrc=%04X\n", frame.dcrc());
	printf("frame.cdcrc=%04X\n", frame.cdcrc());
	printf("frame.[");
	for(int i = 0; i < frame.length(); i++)
	{
		printf("%02X ", frame.data()[i]);
	}
	printf("\b].check(%d)\n", frame.check());

	return 0;
}

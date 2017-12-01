#include <stdio.h>
#include <unistd.h>
#include "sendframe.h"
#include "apdu.h"

int main(void)
{
	Apdu apdu;
	SendFrame frame;
	
	apdu.BinaryInput(10, 102);
	frame.apdu(5, 9, 1, apdu.GetApdu(), apdu.Length());

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

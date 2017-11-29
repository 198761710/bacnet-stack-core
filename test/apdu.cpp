#include <stdio.h>
#include "apdu.h"

int main(int argc, char **argv)
{
	Apdu apdu;

	apdu.BinaryInput(1, 103);
	apdu.showhex();
	apdu.BinaryOutput(2, 104);
	apdu.showhex();
	apdu.BinaryValue(3, 105);
	apdu.showhex();
	apdu.AnalogInput(4, 1001);
	apdu.showhex();
	apdu.AnalogOutput(5, 1002);
	apdu.showhex();
	apdu.AnalogValue(6, 1003);
	apdu.showhex();

	apdu.AckBinaryInput(1, 103, 1);
	apdu.showhex();
	apdu.AckBinaryOutput(2, 104, 1);
	apdu.showhex();
	apdu.AckBinaryValue(3, 105, 0);
	apdu.showhex();
	apdu.AckAnalogInput(4, 1006, 1.14149);
	apdu.showhex();
	apdu.AckAnalogOutput(5, 1007, 2.14159);
	apdu.showhex();
	apdu.AckAnalogValue(6, 1008, 3.14159);
	apdu.showhex();

	apdu.ParseApdu(apdu.GetApdu(), apdu.Length());

	printf("apdu.IsBool(%d)\n", apdu.IsBool());
	printf("apdu.IsReal(%d)\n", apdu.IsReal());
	printf("apdu.IsSigned(%d)\n", apdu.IsSigned());
	printf("apdu.IsUnsigned(%d)\n", apdu.IsUnsigned());
	if( apdu.IsBool() )printf("Bool(%d)\n", apdu.Unsigned());
	if( apdu.IsReal() )printf("Real(%f)\n", apdu.Real());
	if( apdu.IsSigned() )printf("Signed(%f)\n", apdu.Signed());
	if( apdu.IsUnsigned() )printf("Unsigned(%f)\n", apdu.Unsigned());

	return 0;
}

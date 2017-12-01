#include <stdio.h>
#include "apdu.h"

int main(int argc, char **argv)
{
	Apdu apdu;

	printf("Get:\n");
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
	apdu.MultiStateInput(3, 105);
	apdu.showhex();
	apdu.MultiStateOutput(3, 105);
	apdu.showhex();
	apdu.MultiStateValue(3, 105);
	apdu.showhex();
	apdu.SignedInt(3, 105);
	apdu.showhex();
	apdu.UnsignedInt(3, 105);
	apdu.showhex();

	printf("Set:\n");
	apdu.SetBinaryInput(1, 103, 1);
	apdu.showhex();
	apdu.SetBinaryOutput(2, 104, 0);
	apdu.showhex();
	apdu.SetBinaryValue(3, 105, 1);
	apdu.showhex();
	apdu.SetAnalogInput(4, 1001, 3.14159*1);
	apdu.showhex();
	apdu.SetAnalogOutput(5, 1002, 3.14159*2);
	apdu.showhex();
	apdu.SetAnalogValue(6, 1003, 3.14159*3);
	apdu.showhex();
	apdu.SetMultiStateInput(3, 105, 8);
	apdu.showhex();
	apdu.SetMultiStateOutput(3, 105, 16);
	apdu.showhex();
	apdu.SetMultiStateValue(3, 105, 32);
	apdu.showhex();
	apdu.SetSignedInt(3, 105, 3);
	apdu.showhex();
	apdu.SetUnsignedInt(3, 105, 9);
	apdu.showhex();

	printf("Ack:\n");
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
	apdu.AckMultiStateInput(6, 1008, 3);
	apdu.showhex();
	apdu.AckMultiStateOutput(6, 1008, 4);
	apdu.showhex();
	apdu.AckMultiStateOutput(6, 1008, 5);
	apdu.showhex();
	apdu.AckSignedInt(6, 1008, 7);
	apdu.showhex();
	apdu.AckUnsignedInt(6, 1008, 8);
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

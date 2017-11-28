#include <stdio.h>
#include "bacnet.h"

int main(int argc, char **argv)
{
	Bacnet bacnet;

	bacnet.BinaryInput(1, 0x91);
	bacnet.showhex();
	bacnet.BinaryOutput(2, 0x81);
	bacnet.showhex();
	bacnet.BinaryValue(3, 0x71);
	bacnet.showhex();
	bacnet.AnalogInput(4, 0x22);
	bacnet.showhex();
	bacnet.AnalogOutput(5, 0x33);
	bacnet.showhex();
	bacnet.AnalogValue(6, 0x44);
	bacnet.showhex();

	return 0;
}

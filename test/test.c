#include <stdio.h>
#include "bacdcode.h"
#include "bacapp.h"
#include "rp.h"

typedef unsigned char u8;

int main(int argc, char **argv)
{
	int i = 0;
	int len = 0;
	uint8_t invokeid = 0;
	uint8_t apdu[MAX_APDU] = {0};
	uint8_t data[MAX_APDU] = {0};
	BACNET_READ_PROPERTY_DATA rp;
	BACNET_READ_PROPERTY_DATA rs;
	BACNET_APPLICATION_DATA_VALUE value;
	BACNET_APPLICATION_DATA_VALUE rvalue;

	value.context_specific = 0;
	value.context_tag = 0;
	value.tag = BACNET_APPLICATION_TAG_REAL;
	value.type.Real = 3.141590;

	len = bacapp_encode_data(data, &value);	

	rp.object_type = OBJECT_ANALOG_VALUE;
	rp.object_instance = 0x1234;
	rp.object_property = PROP_PRESENT_VALUE;
	rp.application_data = data;
	rp.application_data_len = len;
	rp.array_index = BACNET_ARRAY_ALL;

	if( argc < 2 )
	{
		len = rp_encode_apdu(apdu, 0xAB, &rp);
	}
	else
	{
		len = rp_ack_encode_apdu(apdu, 0xAB, &rp);

		rp_ack_decode_apdu(apdu, len, &invokeid, &rs);

		printf("rs.object_type = %d\n", rs.object_type);
		printf("rs.object_instance = %d\n", rs.object_instance);
		printf("rs.object_property = %d\n", rs.object_property);
		printf("rs.application_data = %p\n", rs.application_data);
		printf("rs.application_data_len = %d\n", rs.application_data_len);

		bacapp_decode_application_data_safe(rs.application_data,
											rs.application_data_len,
											&rvalue);
		printf("rvalue.context_specific = %d\n", rvalue.context_specific);
		printf("rvalue.context_tag = %d\n", rvalue.context_tag);
		printf("rvalue.tag = %d\n", rvalue.tag);
		switch( rvalue.tag )
		{
			case BACNET_APPLICATION_TAG_REAL:
				printf("rvalue.real = %f\n", rvalue.type.Real);
				break;
			default:
				printf("rvalue.tag = unknow\n");
		}
	}
	

	printf("apdu:[");
	for(i = 0; i < len; i++)
	{
		printf("%02X ", apdu[i]);
	}
	printf("\b]\n");
	return 0;
}

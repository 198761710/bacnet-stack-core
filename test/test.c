#include <stdio.h>
#include "rp.h"
#include "bacdcode.h"

typedef unsigned char u8;

int main(void)
{
	int i = 0;
	u8 id = 0;
	int len = 0;
	u8 tag_number = 0;
	u8 apdu[MAX_APDU] = {0};
	u8 data[MAX_APDU] = {0};
	float value = 123.456789;
	BACNET_READ_PROPERTY_DATA rq;
	BACNET_READ_PROPERTY_DATA rs;

	rq.object_type = OBJECT_ANALOG_VALUE;
	rq.object_instance = 0x31;
	rq.object_property = PROP_PRESENT_VALUE;
	rq.array_index = BACNET_ARRAY_ALL;
	len = encode_application_real(data, value);
	rq.application_data = data;
	rq.application_data_len = len;

	len = rp_ack_encode_apdu(apdu, 1, &rq);	
	for(i = 0; i < len; i++)
	{
		printf("%02X ", apdu[i]);
	}
	printf("\n");
	len = rp_ack_decode_apdu(apdu, len, &id, &rs);
	printf("len=%d\n", len);
	printf("id=%d\n", id);
	printf("rs.object_type=%d\n", rs.object_type);
	printf("rs.object_instance=%d\n", rs.object_instance);
	printf("rs.object_property=%d\n", rs.object_property);
	printf("rs.application_data=%p\n", rs.application_data);
	printf("rs.application_data_len=%d\n", rs.application_data_len);
	len = decode_tag_number(rs.application_data, &tag_number);
	switch(tag_number)
	{
		case BACNET_APPLICATION_TAG_BOOLEAN:
		{
			*(uint32_t*)&value = decode_context_boolean(rs.application_data+len);
			printf("BACNET_APPLICATION_TAG_BOOLEAN(%p)\n", *(uint32_t*)&value);
			break;
		}
		case BACNET_APPLICATION_TAG_UNSIGNED_INT:
		{
			decode_context_unsigned(rs.application_data,
									tag_number,
									(uint32_t*)&value);
			printf("BACNET_APPLICATION_TAG_UNSIGNED_INT(%p)\n", *(uint32_t*)&value);
			break;
		}
		case BACNET_APPLICATION_TAG_SIGNED_INT:
		{
			decode_context_signed(rs.application_data,
									tag_number,
									(int32_t*)&value);
			printf("BACNET_APPLICATION_TAG_SIGNED_INT(%p)\n", *(uint32_t*)&value);
			break;
		}
		case BACNET_APPLICATION_TAG_REAL:
		{
			decode_context_real(rs.application_data,
								tag_number,
								&value);
			printf("BACNET_APPLICATION_TAG_REAL(%f)\n", *(float*)&value);
			break;
		}
		case BACNET_APPLICATION_TAG_ENUMERATED:
		{
			decode_context_enumerated(rs.application_data,
								tag_number,
								(uint32_t*)&value);
			printf("BACNET_APPLICATION_TAG_ENUMERATED(%p)\n", *(uint32_t*)&value);
			break;
		}
	}

	return 0;
}

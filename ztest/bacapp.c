#include <stdio.h>
#include <assert.h>
#include "bacapp.h"
#include "bacdcode.h"

typedef unsigned char u8;

int main(void)
{
    int i = 0;
    int len = 0;
    int apdu_len = 0;
    uint8_t apdu[MAX_APDU] = {0};
    uint32_t len_segment[13] = {0};
    uint32_t single_length_segment = {0};
    BACNET_APPLICATION_DATA_VALUE input_value[13];


    for(i = 1; i < 13; i++)
    {
        input_value[i].tag = (BACNET_APPLICATION_TAG) i;
        input_value[i].context_specific = 0;
        input_value[i].context_tag = 0;
        input_value[i].next = NULL;
        switch (input_value[i].tag)
        {
        case BACNET_APPLICATION_TAG_BOOLEAN:
            input_value[i].type.Boolean = true;
            break;
        case BACNET_APPLICATION_TAG_UNSIGNED_INT:
            input_value[i].type.Unsigned_Int = 0xDEADBEEF;
            break;
        case BACNET_APPLICATION_TAG_SIGNED_INT:
            input_value[i].type.Signed_Int = 0x00C0FFEE;
            break;
        case BACNET_APPLICATION_TAG_REAL:
            input_value[i].type.Real = 3.141592654f;
            break;
        case BACNET_APPLICATION_TAG_DOUBLE:
            input_value[i].type.Double = 2.32323232323;
            break;
        case BACNET_APPLICATION_TAG_OCTET_STRING:
        {
            uint8_t test_octet[5] = { "Karg" };
            octetstring_init(&input_value[i].type.Octet_String,
                             test_octet, 
							 sizeof(test_octet));
        }
        break;

        case BACNET_APPLICATION_TAG_CHARACTER_STRING:
            characterstring_init_ansi(&input_value[i].type.
                                      Character_String, 
									  "Hello There!");
            break;

        case BACNET_APPLICATION_TAG_BIT_STRING:
            bitstring_init(&input_value[i].type.Bit_String);
            bitstring_set_bit(&input_value[i].type.Bit_String, 0, true);
            bitstring_set_bit(&input_value[i].type.Bit_String, 1, false);
            bitstring_set_bit(&input_value[i].type.Bit_String, 2, false);
            bitstring_set_bit(&input_value[i].type.Bit_String, 3, true);
            bitstring_set_bit(&input_value[i].type.Bit_String, 4, false);
            bitstring_set_bit(&input_value[i].type.Bit_String, 5, true);
            bitstring_set_bit(&input_value[i].type.Bit_String, 6, true);
            break;

        case BACNET_APPLICATION_TAG_ENUMERATED:
            input_value[i].type.Enumerated = 0x0BADF00D;
            break;

        case BACNET_APPLICATION_TAG_DATE:
            input_value[i].type.Date.day = 10;
            input_value[i].type.Date.month = 9;
            input_value[i].type.Date.wday = 3;
            input_value[i].type.Date.year = 1998;
            break;

        case BACNET_APPLICATION_TAG_TIME:
            input_value[i].type.Time.hour = 12;
            input_value[i].type.Time.hundredths = 56;
            input_value[i].type.Time.min = 20;
            input_value[i].type.Time.sec = 41;
            break;

        case BACNET_APPLICATION_TAG_OBJECT_ID:
            input_value[i].type.Object_Id.instance = 1234;
            input_value[i].type.Object_Id.type = 12;
            break;

        default:
            break;
        }
        single_length_segment = bacapp_encode_data(&apdu[len], &input_value[i]);
        assert(single_length_segment > 0);
        /* len_segment is accumulated length */
        if( i == 0)
        {
            len_segment[i] = single_length_segment;
        }
        else
        {
            len_segment[i] = single_length_segment + len_segment[i - 1];
        }
        len = len_segment[i];
    }
	printf("apdu:[");
	for(i = 0; i < len; i++)
	{
		printf("%02X ", apdu[i]);
	}
	printf("\b]\n");

	return 0;
}

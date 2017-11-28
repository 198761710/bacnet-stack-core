#include <string.h>
#include "bacnet.h"

Bacnet::Bacnet(void)
{
	length = 0;
	parseok = 0;
	invokeid =  0;
	memset(apdu, 0, sizeof(apdu));
	memset(data, 0, sizeof(data));
	memset(&rpdata, 0, sizeof(rpdata));
	memset(&rsdata, 0, sizeof(rsdata));
	memset(&rsvalue, 0, sizeof(rsvalue));
}
int Bacnet::AnalogInput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_ANALOG_INPUT);
}
int Bacnet::AnalogOutput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_ANALOG_OUTPUT);
}
int Bacnet::AnalogValue(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_ANALOG_VALUE);
}
int Bacnet::BinaryInput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_BINARY_INPUT);
}
int Bacnet::BinaryOutput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_BINARY_OUTPUT);
}
int Bacnet::BinaryValue(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_BINARY_VALUE);
}
int Bacnet::MultiStateInput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_MULTI_STATE_INPUT);
}
int Bacnet::MultiStateOutput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_MULTI_STATE_OUTPUT);
}
int Bacnet::MultiStateValue(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_MULTI_STATE_VALUE);
}
int Bacnet::SignedInt(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_INTEGER_VALUE);
}
int Bacnet::UnsignedInt(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_POSITIVE_INTEGER_VALUE);
}
int Bacnet::MakeRequest(int id, int instance, BACNET_OBJECT_TYPE type)
{
	rpdata.object_type = type;
	rpdata.object_instance = instance;
	rpdata.object_property = PROP_PRESENT_VALUE;
	rpdata.application_data = 0;
	rpdata.application_data_len = 0;
	rpdata.array_index = BACNET_ARRAY_ALL;

	return length = rp_encode_apdu(apdu, id, &rpdata);
}
bool Bacnet::IsBool(void)
{
	if( false == parseok )
	{
		return false;
	}
	if( BACNET_APPLICATION_TAG_BOOLEAN == rsvalue.tag )
	{
		return true;
	}
	return false;
}
bool Bacnet::IsReal(void)
{
	if( false == parseok )
	{
		return false;
	}
	if( BACNET_APPLICATION_TAG_REAL == rsvalue.tag )
	{
		return true;
	}
	return false;
}
bool Bacnet::IsDouble(void)
{
	if( false == parseok )
	{
		return false;
	}
	if( BACNET_APPLICATION_TAG_DOUBLE == rsvalue.tag )
	{
		return true;
	}
	return false;
}
bool Bacnet::IsSigned(void)
{
	if( false == parseok )
	{
		return false;
	}
	if( BACNET_APPLICATION_TAG_SIGNED_INT == rsvalue.tag )
	{
		return true;
	}
	return false;
}
bool Bacnet::IsUnsigned(void)
{
	if( false == parseok )
	{
		return false;
	}
	if( BACNET_APPLICATION_TAG_UNSIGNED_INT == rsvalue.tag )
	{
		return true;
	}
	return false;
}
bool Bacnet::ParseApdu(uint8_t *apdu, int len)
{
	int ret = -1;

	if( 0 == apdu || len < 1 )
	{
		return (parseok = false);
	}
	memset(&rsdata, 0, sizeof(rsdata));
	ret = rp_ack_decode_apdu(apdu, len, &invokeid, &rsdata);
	if( ret < 0 )
	{
		return (parseok = false);
	}
	if( NULL == rsdata.application_data || rsdata.application_data_len < 1 )
	{
		return (parseok = false);
	}
	ret = bacapp_decode_application_data_safe(rsdata.application_data,
											  rsdata.application_data_len,
											  &rsvalue);
	if( ret < 0 )
	{
		return (parseok = false);
	}
	return (parseok = true);
}
void Bacnet::showhex(void)
{
	for(int i = 0; i < length; i++)
	{
		printf("%02X ", apdu[i]);
	}
	printf("\n");
}

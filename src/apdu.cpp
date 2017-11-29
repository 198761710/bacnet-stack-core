#include <string.h>
#include "apdu.h"

Apdu::Apdu(void)
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
int Apdu::AnalogInput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_ANALOG_INPUT);
}
int Apdu::AnalogOutput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_ANALOG_OUTPUT);
}
int Apdu::AnalogValue(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_ANALOG_VALUE);
}
int Apdu::BinaryInput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_BINARY_INPUT);
}
int Apdu::BinaryOutput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_BINARY_OUTPUT);
}
int Apdu::BinaryValue(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_BINARY_VALUE);
}
int Apdu::MultiStateInput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_MULTI_STATE_INPUT);
}
int Apdu::MultiStateOutput(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_MULTI_STATE_OUTPUT);
}
int Apdu::MultiStateValue(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_MULTI_STATE_VALUE);
}
int Apdu::SignedInt(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_INTEGER_VALUE);
}
int Apdu::UnsignedInt(int id, int instance)
{
	return MakeRequest(id, instance, OBJECT_POSITIVE_INTEGER_VALUE);
}
int Apdu::MakeRequest(int id, int instance, BACNET_OBJECT_TYPE type)
{
	rpdata.object_type = type;
	rpdata.object_instance = instance;
	rpdata.object_property = PROP_PRESENT_VALUE;
	rpdata.application_data = 0;
	rpdata.application_data_len = 0;
	rpdata.array_index = BACNET_ARRAY_ALL;

	return length = rp_encode_apdu(apdu, id, &rpdata);
}

int Apdu::AckAnalogInput(int id, int instance, float v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_REAL;
	rpvalue.type.Real		 = v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_ANALOG_INPUT;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckAnalogOutput(int id, int instance, float v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_REAL;
	rpvalue.type.Real		 = v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_ANALOG_OUTPUT;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckAnalogValue(int id, int instance, float v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_REAL;
	rpvalue.type.Real		 = v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_ANALOG_VALUE;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckBinaryInput(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_UNSIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_BINARY_INPUT;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckBinaryOutput(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_UNSIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_BINARY_VALUE;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckBinaryValue(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_UNSIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_BINARY_INPUT;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckMultiStateInput(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_UNSIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_MULTI_STATE_INPUT;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckMultiStateOutput(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_UNSIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_MULTI_STATE_OUTPUT;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckMultiStateValue(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_UNSIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_MULTI_STATE_VALUE;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckSignedInt(int id, int instance, int32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_SIGNED_INT;
	rpvalue.type.Signed_Int	 = v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_INTEGER_VALUE;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}
int Apdu::AckUnsignedInt(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_UNSIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);	
	if( len < 1 )
	{
		return len;
	}

	rpdata.application_data		= data;
	rpdata.application_data_len = len;
	rpdata.object_type			= OBJECT_POSITIVE_INTEGER_VALUE;
	rpdata.object_instance		= instance;
	rpdata.object_property		= PROP_PRESENT_VALUE;
	rpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = rp_ack_encode_apdu(apdu, id, &rpdata));
}

bool Apdu::IsBool(void)
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
bool Apdu::IsReal(void)
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
bool Apdu::IsDouble(void)
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
bool Apdu::IsSigned(void)
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
bool Apdu::IsUnsigned(void)
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
float Apdu::Real(void)
{
	return rsvalue.type.Real;
}
int32_t Apdu::Signed(void)
{
	return rsvalue.type.Signed_Int;
}
uint32_t Apdu::Unsigned(void)
{
	return rsvalue.type.Unsigned_Int;
}
uint8_t* Apdu::GetApdu(void)
{
	return apdu;
}
int32_t Apdu::Length(void)
{
	return length;
}
bool Apdu::ParseApdu(uint8_t *apdu, int len)
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
void Apdu::showhex(void)
{
	for(int i = 0; i < length; i++)
	{
		printf("%02X ", apdu[i]);
	}
	printf("\n");
}

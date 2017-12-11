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
	memset(&wpdata, 0, sizeof(wpdata));
	memset(&wsdata, 0, sizeof(wsdata));
	memset(&rsvalue, 0, sizeof(rsvalue));
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


int Apdu::SetBinaryInput(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_ENUMERATED;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);
	if( len < 1 )
	{
		return len;
	}

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_BINARY_INPUT;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;
	wpdata.priority				= BACNET_MAX_PRIORITY;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetBinaryOutput(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_ENUMERATED;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);
	if( len < 1 )
	{
		return len;
	}

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_BINARY_OUTPUT;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;
	wpdata.priority				= BACNET_MAX_PRIORITY;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetBinaryValue(int id, int instance, uint32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_ENUMERATED;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);
	if( len < 1 )
	{
		return len;
	}

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_BINARY_VALUE;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;
	wpdata.priority				= BACNET_MAX_PRIORITY;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetAnalogInput(int id, int instance, float v)
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

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_ANALOG_INPUT;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetAnalogOutput(int id, int instance, float v)
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

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_ANALOG_OUTPUT;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetAnalogValue(int id, int instance, float v)
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

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_ANALOG_VALUE;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetMultiStateInput(int id, int instance, uint32_t v)
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

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_MULTI_STATE_INPUT;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;
	wpdata.priority				= BACNET_MAX_PRIORITY;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetMultiStateOutput(int id, int instance, uint32_t v)
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

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_MULTI_STATE_OUTPUT;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;
	wpdata.priority				= BACNET_MAX_PRIORITY;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetMultiStateValue(int id, int instance, uint32_t v)
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

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_MULTI_STATE_VALUE;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;
	wpdata.priority				= BACNET_MAX_PRIORITY;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetSignedInt(int id, int instance, int32_t v)
{
	int len = 0;

	rpvalue.context_specific = 0;
	rpvalue.context_tag		 = 0;
	rpvalue.tag				 = BACNET_APPLICATION_TAG_SIGNED_INT;
	rpvalue.type.Unsigned_Int= v;

	len = bacapp_encode_data(data, &rpvalue);
	if( len < 1 )
	{
		return len;
	}

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_INTEGER_VALUE;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
}
int Apdu::SetUnsignedInt(int id, int instance, uint32_t v)
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

	memcpy(wpdata.application_data, data, len);
	wpdata.application_data_len = len;
	wpdata.object_type			= OBJECT_POSITIVE_INTEGER_VALUE;
	wpdata.object_instance		= instance;
	wpdata.object_property		= PROP_PRESENT_VALUE;
	wpdata.array_index			= BACNET_ARRAY_ALL;

	return (length = wp_encode_apdu(apdu, id, &wpdata));
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
bool Apdu::IsEnumerated(void)
{
	if( false == parseok )
	{
		return false;
	}
	if( BACNET_APPLICATION_TAG_ENUMERATED == rsvalue.tag )
	{
		return true;
	}
	return false;
}
uint32_t Apdu::Tag(void)
{
	return (uint32_t)rsvalue.tag;
}
bool Apdu::Bool(void)
{
	return !!rsvalue.type.Unsigned_Int;
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
uint32_t Apdu::Enumerated(void)
{
	return rsvalue.type.Unsigned_Int;
}
uint32_t Apdu::Instance(void)
{
	return rsdata.object_instance;
}
uint8_t  Apdu::InvokeId(void)
{
	return invokeid;
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

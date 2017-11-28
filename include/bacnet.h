#ifndef __BACNET_H__
#define __BACNET_H__
#include "bacdcode.h"
#include "bacapp.h"
#include "rp.h"

class Bacnet
{
protected:
	int length;
	bool parseok;
	uint8_t invokeid;
	uint8_t apdu[MAX_APDU/1];
	uint8_t data[MAX_APDU/2];
	BACNET_READ_PROPERTY_DATA rpdata;
	BACNET_READ_PROPERTY_DATA rsdata;
	BACNET_APPLICATION_DATA_VALUE rsvalue;

public:
	Bacnet(void);

public:
	int AnalogInput(int id, int instance);
	int AnalogOutput(int id, int instance);
	int AnalogValue(int id, int instance);
	int BinaryInput(int id, int instance);
	int BinaryOutput(int id, int instance);
	int BinaryValue(int id, int instance);
	int MultiStateInput(int id, int instance);
	int MultiStateOutput(int id, int instance);
	int MultiStateValue(int id, int instance);
	int SignedInt(int id, int instance);
	int UnsignedInt(int id, int instance);
	int MakeRequest(int id, int instance, BACNET_OBJECT_TYPE type);

public:
	int AckAnalogInput(int id, int instance);
	int AckAnalogOutput(int id, int instance);
	int AckAnalogValue(int id, int instance);
	int AckBinaryInput(int id, int instance);
	int AckBinaryOutput(int id, int instance);
	int AckBinaryValue(int id, int instance);
	int AckMultiStateInput(int id, int instance);
	int AckMultiStateOutput(int id, int instance);
	int AckMultiStateValue(int id, int instance);
	int AckSignedInt(int id, int instance);
	int AckUnsignedInt(int id, int instance);
	int MakeAck(int id, int instance, BACNET_OBJECT_TYPE type);

public:
	bool IsBool(void);
	bool IsReal(void);
	bool IsDouble(void);
	bool IsSigned(void);
	bool IsUnsigned(void);
	bool ParseApdu(uint8_t *apdu, int len);
	
public:
	void showhex(void);
};

#endif//__BACNET_H__

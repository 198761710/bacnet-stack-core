#include <assert.h>
#include "mstp.h"
#include "apdu.h"


Mstp::Mstp(void)
{
}
Mstp::Mstp(const string& dev, int baud, int parity, int bsize, int stop)
{
	assert( com.Open(dev) );
	assert( com.Set(baud, parity, bsize, stop) );
}
bool Mstp::Run(void)
{
	unsigned char c = 0;

	if( com.Recv(&c, 1) == 1 )
	{
		frame.push(c);
	}
	if( frame.check() )
	{
		//frame.showhex();
		ProcFrame(frame);
		recvtime.init();
		frame.clear();
	}
	StateMachine();
}
void Mstp::StateMachine(void)
{		
	switch( master.state )
	{
		case StateIdle:
			if( recvtime.mdiff() > 300 )
			{
				master.state = StateToken;
				printf("mstp.state = %d\n", master.state);
			}
			break;
		case StateToken:
			DoRequest();
			break;
		case StateRequest:
			break;
	}
}
void Mstp::DoRequest(void)
{
	if( instancelist.empty() )
	{
		return;
	}
	if( sendtime.mdiff() < 70 )
	{
		return;
	}
	Instance &i = instancelist.front();
	switch( i.cmd )
	{
		case GetValue:
			DoReadRequest();
			break;
		case SetValue:
			DoWriteRequest();
			break;
	}
}
void Mstp::DoReadRequest(void)
{
	Apdu apdu;
	SendFrame sendframe;
	Instance &i = instancelist.front();

	switch( i.type )
	{
		case OBJECT_ANALOG_INPUT:
			apdu.AnalogInput(master.invokeid, i.instance);
			break;
		case OBJECT_ANALOG_OUTPUT:
			apdu.AnalogOutput(master.invokeid, i.instance);
			break;
		case OBJECT_ANALOG_VALUE:
			apdu.AnalogValue(master.invokeid, i.instance);
			break;
		case OBJECT_BINARY_INPUT:
			apdu.BinaryInput(master.invokeid, i.instance);
			break;
		case OBJECT_BINARY_OUTPUT:
			apdu.BinaryOutput(master.invokeid, i.instance);
			break;
		case OBJECT_BINARY_VALUE:
			apdu.BinaryValue(master.invokeid, i.instance);
			break;
		case OBJECT_INTEGER_VALUE:
			apdu.SignedInt(master.invokeid, i.instance);
			break;
		case OBJECT_POSITIVE_INTEGER_VALUE:
			apdu.UnsignedInt(master.invokeid, i.instance);
			break;
		case OBJECT_MULTI_STATE_INPUT:
			apdu.MultiStateInput(master.invokeid, i.instance);
			break;
		case OBJECT_MULTI_STATE_OUTPUT:
			apdu.MultiStateOutput(master.invokeid, i.instance);
			break;
		case OBJECT_MULTI_STATE_VALUE:
			apdu.MultiStateValue(master.invokeid, i.instance);
			break;
	}
	sendframe.apdu(FrameDataRequest, i.dst, master.sudomaster, apdu.GetApdu(), apdu.Length());
	if( com.Send(sendframe.data(), sendframe.length()) )
	{
		sendtime.init();
		master.invokeid++;
		//printf("%s=>", __func__);
		//sendframe.showhex();
		instancelist.pop_front();
	}
}
void Mstp::DoWriteRequest(void)
{
	Apdu apdu;
	SendFrame sendframe;
	Instance &i = instancelist.front();

	switch( i.type )
	{
		case OBJECT_ANALOG_INPUT:
			apdu.SetAnalogInput(master.invokeid, i.instance, i.value.f);
			break;
		case OBJECT_ANALOG_OUTPUT:
			apdu.SetAnalogOutput(master.invokeid, i.instance, i.value.f);
			break;
		case OBJECT_ANALOG_VALUE:
			apdu.SetAnalogValue(master.invokeid, i.instance, i.value.f);
			break;
		case OBJECT_BINARY_INPUT:
			apdu.SetBinaryInput(master.invokeid, i.instance, i.value.u);
			break;
		case OBJECT_BINARY_OUTPUT:
			apdu.SetBinaryOutput(master.invokeid, i.instance, i.value.u);
			break;
		case OBJECT_BINARY_VALUE:
			apdu.SetBinaryValue(master.invokeid, i.instance, i.value.u);
			break;
		case OBJECT_INTEGER_VALUE:
			apdu.SetSignedInt(master.invokeid, i.instance, i.value.i);
			break;
		case OBJECT_POSITIVE_INTEGER_VALUE:
			apdu.SetUnsignedInt(master.invokeid, i.instance, i.value.u);
			break;
		case OBJECT_MULTI_STATE_INPUT:
			apdu.SetMultiStateInput(master.invokeid, i.instance, i.value.u);
			break;
		case OBJECT_MULTI_STATE_OUTPUT:
			apdu.SetMultiStateOutput(master.invokeid, i.instance, i.value.u);
			break;
		case OBJECT_MULTI_STATE_VALUE:
			apdu.SetMultiStateValue(master.invokeid, i.instance, i.value.u);
			break;
	}
	sendframe.apdu(FrameDataRequest, i.dst, master.sudomaster, apdu.GetApdu(), apdu.Length());
	if( com.Send(sendframe.data(), sendframe.length()) )
	{
		sendtime.init();
		master.invokeid++;
		//printf("%s=>", __func__);
		//sendframe.showhex();
		instancelist.pop_front();
	}
}
void Mstp::Add(const Instance& instance)
{
	instancelist.push_back(instance);
}
bool Mstp::Empty(void)
{
	return instancelist.empty();
}
bool Mstp::Open(const string& dev)
{
	return com.Open(dev);
}
bool Mstp::Set(int baud, int parity, int bsize, int stop)
{
	return com.Set(baud, parity, bsize, stop);
}
bool Mstp::ProcFrame(RecvFrame& f)
{
	unsigned dst = f.dst();
	unsigned src = f.src();

	mastermap[src].init();
	if( 0xff != dst && mastermap[dst].mdiff() < 1000 )
	{
		return false;
	}
	switch( f.type() )
	{
		case FrameToken:
			return ProcToken(f);

		case FramePollMaster:
			return ProcPollMaster(f);

		case FramePollMasterAck:
			return ProcPollMasterAck(f);

		case FrameTestRequest:
			return ProcTestRequest(f);

		case FrameTestRequestAck:
			return ProcTestRequestAck(f);

		case FrameDataRequest:
			return ProcDataRequest(f);

		case FrameDataRequestAck:
			return ProcDataRequestAck(f);
	}
	return false;
}
bool Mstp::ProcToken(RecvFrame& f)
{
	unsigned char src = f.src();
	unsigned char dst = f.dst();

	master.state = StateToken;
	master.sudomaster = dst;
	master.t.init();

	return true;
}
bool Mstp::ProcPollMaster(RecvFrame& f)
{
	SendFrame s;
	unsigned char src = f.src();
	unsigned char dst = f.dst();
	unsigned char type = FramePollMasterAck;

	s.mstp(type, src, dst);
	if( com.Send(s.data(), s.length()) )
	{
		//s.showhex();
		return true;
	}
	return false;
}
bool Mstp::ProcPollMasterAck(RecvFrame& f)
{
	return true;
}
bool Mstp::ProcTestRequest(RecvFrame& f)
{
	return true;
}
bool Mstp::ProcTestRequestAck(RecvFrame& f)
{
	return true;
}
bool Mstp::ProcDataRequest(RecvFrame& f)
{
	return true;
}
bool Mstp::ProcDataRequestAck(RecvFrame& f)
{
	Apdu apdu;

	if( f.dlen() > 2 )
	{
		apdu.ParseApdu(f.apdu(), f.dlen()-2);
	}
	switch( *f.apdu() )
	{
		case 0x20://APDU_TYPE_SIMPLE_ACK:
		case 0x30://APDU_TYPE_COMPLEX_ACK:
			break;
		case 0x50://APDU_TYPE_ERROR:
			printf("APDU_TYPE_ERROR\n");
			return false;
		case 0x60://APDU_TYPE_REJECT:
			printf("APDU_TYPE_REJECT\n");
			return false;
		case 0x70://APDU_TYPE_ABORT:
			printf("APDU_TYPE_ABORT\n");
			return false;
	}
	static TimeOperator t;

	printf("%d", t.mdiff());
	t.init();
	if( apdu.IsBool() )
	{
		printf("---->Bool:instance(%d).value(%u)\n", apdu.Instance(), apdu.Unsigned());
	}
	else if( apdu.IsReal() )
	{
		printf("---->Real:instance(%d).value(%f)\n", apdu.Instance(), apdu.Real());
	}
	else if( apdu.IsSigned() )
	{
		printf("---->Signed:instance(%d).value(%d)\n", apdu.Instance(), apdu.Signed());
	}
	else if( apdu.IsUnsigned() )
	{
		printf("---->Unsigned:instance(%d).value(%u)\n", apdu.Instance(), apdu.Unsigned());
	}
	else if( apdu.IsEnumerated() )
	{
		printf("---->Enumerated:instance(%d).value(%u)\n", apdu.Instance(), apdu.Enumerated());
	}

	return true;
}

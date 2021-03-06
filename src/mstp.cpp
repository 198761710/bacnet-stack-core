#include <assert.h>
#include "mstp.h"
#include "apdu.h"


Mstp::Mstp(void)
{
	retry = 0;
	recvok = false;
	getvalue = 0;
}
Mstp::Mstp(const string& dev, int baud, int parity, int bsize, int stop)
{
	retry = 0;
	recvok = false;
	getvalue = 0;
	assert( com.Open(dev) );
	assert( com.Set(baud, parity, bsize, stop) );
}
bool Mstp::Run(void)
{
	valuelist.clear();
	for(int i = 0; i < 8; i++)
	{
		unsigned char c = 0;

		if( com.Recv(&c, 1) == 1 )
		{
			recving.init();
			frame.push(c);
		}
		if( frame.check() )
		{
			offlineread[frame.src()].recvtime.init();
			offlinewrite[frame.src()].recvtime.init();
			ProcFrame(frame);
			recvtime.init();
			frame.clear();
		}
		StateMachine();
	}
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
	if( recving.mdiff() < 10 )
	{
		return;
	}
	if( recvok == false && sendtime.mdiff() < 120 )
	{
		return;
	}
	Instance &i = instancelist.front();
	switch( i.cmd )
	{
		case CmdGetValue:
			DoReadRequest();
			break;
		case CmdSetValue:
			DoWriteRequest();
			break;
	}
}
void Mstp::DoReadRequest(void)
{
	Apdu apdu;
	SendFrame sendframe;
	Instance &i = instancelist.front();
	Offline &o = offlineread[i.dst];

	if( o.recvtime.sdiff() > 20 )
	{
		if( o.retry.sdiff() < 10 )
		{
			instancelist.pop_front();
			return;
		}
		o.retry.init();
	}

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
		recvok = false;
		sendcount[sendframe.dst()]++;
		sendtime.init();
		master.invokeid++;
		if( retry++ > 0 )
		{
			retry = 0;
			instancelist.pop_front();
		}
	}
	else
	{
		printf("send failed!\n");
	}
}
void Mstp::DoWriteRequest(void)
{
	Apdu apdu;
	SendFrame sendframe;
	Instance &i = instancelist.front();
	Offline &o = offlinewrite[i.dst];

	if( o.recvtime.sdiff() > 10 )
	{
		if( o.retry.sdiff() < 5 )
		{
			instancelist.pop_front();
			return;
		}
		o.retry.init();
	}


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
		sendframe.showhex();
		recvok = false;
		sendcount[sendframe.dst()]++;
		sendtime.init();
		master.invokeid++;
		if( retry++ > 0 )
		{
			retry = 0;
			instancelist.pop_front();
		}
	}
	else
	{
		printf("send failed!\n");
	}
}
void Mstp::Add(const Instance& instance)
{
	instancelist.push_back(instance);
}
void Mstp::PushBack(const Instance& instance)
{
	instancelist.push_back(instance);
}
void Mstp::PushFront(const Instance& instance)
{
	if( instancelist.empty() )
	{
		instancelist.push_front(instance);
	}
	else
	{
		instancelist.insert(++instancelist.begin(), instance);
	}
}
void Mstp::PushBack(list<Instance>& ilist)
{
	for(list<Instance>::iterator i = ilist.begin(); i != ilist.end(); i++)
	{
		instancelist.push_back(*i);
	}
}
void Mstp::PushFront(list<Instance>& ilist)
{
	for(list<Instance>::iterator i = ilist.begin(); i != ilist.end(); i++)
	{
		instancelist.push_front(*i);
	}
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
bool Mstp::Close(void)
{
	return com.Close();
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

	recvok = true;
	recvcount[f.src()]++;
	if( f.dlen() > 2 )
	{
		apdu.ParseApdu(f.apdu(), f.dlen()-2);
	}
	switch( *f.apdu() )
	{
		case 0x20://APDU_TYPE_SIMPLE_ACK:
		case 0x30://APDU_TYPE_COMPLEX_ACK:
			retry = 0;
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
		default:
			printf("APDU_TYPE_UNKONW(%d)\n", *f.apdu());
			return false;
	}

	Instance &i = instancelist.front();

	if( apdu.IsBool() )
	{
		i.value.f = apdu.Unsigned();
	}
	else if( apdu.IsReal() )
	{
		i.value.f = apdu.Real();
	}
	else if( apdu.IsSigned() )
	{
		i.value.f = apdu.Signed();
	}
	else if( apdu.IsUnsigned() )
	{
		i.value.f = apdu.Unsigned();
	}
	else if( apdu.IsEnumerated() )
	{
		i.value.f = apdu.Unsigned();
	}
	if( instancelist.front().instance == apdu.Instance() )
	{
		if( getvalue )
		{
			double v = i.value.f;
			getvalue(i.name, v, "mstp");
		}
		static TimeOperator t;
		printf("[%d][%-6s]=%-3.1f\n", t.mdiff(), GetName(i).data(), i.value.f);
		t.init();
	}
	valuelist.push_back(i);
	instancelist.pop_front();
	return true;
}
void Mstp::InitRecvRate(void)
{
	for(map<u8,u32>::iterator i = recvcount.begin(); i != recvcount.end(); i++)
	{
		i->second = 0;
	}
	for(map<u8,u32>::iterator i = sendcount.begin(); i != sendcount.end(); i++)
	{
		i->second = 0;
	}
}
void Mstp::GetRecvRate(list<RecvRate>& rlist)
{
	for(map<u8,u32>::iterator i = sendcount.begin(); i != sendcount.end(); i++)
	{
		u8 d = i->first;
		u32 s = i->second;
		u32 r = recvcount[d];

		if( 0 == s )
		{
			rlist.push_back( RecvRate(d, 0, 0, float(0.0)) );
		}
		else
		{
			rlist.push_back( RecvRate(d, r, s, float(r)/float(s) * 100.0) );
		}
	}

}
string Mstp::GetName(Instance& i)
{
	char buf[32] = {0};

	snprintf(buf, sizeof(buf), "%d", i.instance);
	switch(i.type)
	{
		case OBJECT_ANALOG_INPUT:
			return string("AI") + buf;
		case OBJECT_ANALOG_OUTPUT:
			return string("AO") + buf;
		case OBJECT_ANALOG_VALUE:
			return string("AV") + buf;
		case OBJECT_BINARY_INPUT:
			return string("BI") + buf;
		case OBJECT_BINARY_OUTPUT:
			return string("BO") + buf;
		case OBJECT_BINARY_VALUE:
			return string("BV") + buf;
		case OBJECT_MULTI_STATE_INPUT:
			return string("MI") + buf;
		case OBJECT_MULTI_STATE_OUTPUT:
			return string("MO") + buf;
		case OBJECT_MULTI_STATE_VALUE:
			return string("MV") + buf;
	}
	return string("XU") + buf;
}
void Mstp::GetValue( bool(*f)(const string&, double, const string&) )
{
	getvalue = f;
}
list<Instance>& Mstp::ValueList(void)
{
	return valuelist;
}

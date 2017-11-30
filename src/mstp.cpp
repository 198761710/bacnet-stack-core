#include <assert.h>
#include "mstp.h"


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
		ProcFrame(frame);
		frame.clear();
	}
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

	token.ismaster = true;
	token.sudomaster = dst;
	token.t.init();

	return true;
}
bool Mstp::ProcPollMaster(RecvFrame& f)
{
	return true;
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
	return true;
}

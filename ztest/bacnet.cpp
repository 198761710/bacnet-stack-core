#include <list>
#include <stdio.h>
#include <assert.h>
#include "recvframe.h"
#include "sendframe.h"
#include "comport.h"
#include "apdu.h"
using namespace std;


void MakeList(list<SendFrame>& l)
{
	Apdu apdu;
	SendFrame frame;

	apdu.AnalogValue(1, 103);
	frame.apdu(5, 1, 0x7f, apdu.GetApdu(), apdu.Length());
	l.push_back(frame);
}
int main(int argc, char **argv)
{
	ComPort com;
	RecvFrame frame;
	list<SendFrame> rqlist;
	time_t t = time(0);
	
	assert( com.Open("/dev/ttySX0") );
	assert( com.Set(9600, 0, 8, 1) );

	while(1)
	{
		unsigned char c = 0;
		if( com.Recv(&c, 1) == 1 )
		{
			frame.push(c);
		}
		if( frame.check() )
		{
			frame.showhex();
			frame.clear();
		}
		if( time(0) - t != 0 )
		{
			t = time(0);
			if( rqlist.empty() )
			{
				MakeList(rqlist);
			}
			SendFrame &s = rqlist.front();
			//com.Send(s.data(), s.length());
			s.showhex();
			rqlist.pop_front();
		}
		usleep(100);
	}

	return 0;
}

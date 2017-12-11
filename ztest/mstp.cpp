#include <unistd.h>
#include <stdio.h>
#include "bacenum.h"
#include "mstp.h"

void GetRecvRate(Mstp& mstp)
{
	list<RecvRate> rlist;
	mstp.GetRecvRate(rlist);
	for(list<RecvRate>::iterator i = rlist.begin(); i != rlist.end(); i++)
	{
		printf("mac(%d):[recvcount(%d)/sendcount(%d)]=%.0f%%\n", 
				i->mac, i->rcount, i->scount, i->rate);
	}
	mstp.InitRecvRate();
}
void Get(Mstp& mstp)
{
	{
		mstp.Add( Instance(0, 1, OBJECT_ANALOG_VALUE, 102) );
		mstp.Add( Instance(0, 1, OBJECT_ANALOG_VALUE, 103) );
		mstp.Add( Instance(0, 1, OBJECT_ANALOG_VALUE, 104) );
		mstp.Add( Instance(0, 1, OBJECT_ANALOG_VALUE, 1013) );
		mstp.Add( Instance(0, 1, OBJECT_ANALOG_VALUE, 1015) );
		mstp.Add( Instance(0, 1, OBJECT_ANALOG_VALUE, 202) );
		mstp.Add( Instance(0, 1, OBJECT_ANALOG_VALUE, 1020) );
		mstp.Add( Instance(0, 1, OBJECT_BINARY_VALUE, 203) );
		mstp.Add( Instance(0, 1, OBJECT_MULTI_STATE_VALUE, 101) );
		mstp.Add( Instance(0, 1, OBJECT_MULTI_STATE_VALUE, 103) );
	}
}
void Set(Mstp& mstp)
{
	if( mstp.Empty() )
	{
		Get(mstp);
		GetRecvRate(mstp);
	}
}
int main(void)
{
	Mstp mstp("/dev/ttySX0", 9600, 0, 8, 1);

	while(1)
	{
		Set(mstp);
		mstp.Run();
		usleep(100);
	}
	mstp.Close();

	return 0;
}

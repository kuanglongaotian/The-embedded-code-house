#include <REGX52.H>
#include "Nixie_tube.H"

void main()
{
  while(1)
	{
		Nixie_tube(1,4);
		Nixie_tube(2,3);
		Nixie_tube(3,2);
		Nixie_tube(4,1);
	}
}
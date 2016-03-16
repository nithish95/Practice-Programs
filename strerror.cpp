#include <iostream>
#include <string.h>

using namespace std;
int main(void)
{
	int eno;
	
	for (eno = 0; eno <= 40; eno++)
		cout<<eno<<" :"<<strerror(eno)<<"\n";

   return 0;
}

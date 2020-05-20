#include "gassp72.h"

extern short TabSig[];
extern int CalculDFT(short *,int );

int tab[64];

int main(void)
{	

	for(int i=0;i<64;i++){
		tab[i]=CalculDFT(TabSig,i);
	}
	
	
	while	(1)
		{
		}
}

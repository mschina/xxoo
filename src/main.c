#include <stdio.h>

#define SWAP(A,B)	{A=A^B^A;B=A^B^B}


int main(void)
{
	int a=100,b=290;
	SWAP(a,b);
	printf(a,b);
	return 0;
}

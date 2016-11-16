#include "Bignum.h"
#include "BignumTl.h"
#include <stdio.h>
#include <stdlib.h>

#define fname "C:\\Users\\NeutrinoAnt\\Documents\\file.txt"

int fibo(void)
{
	int i, N;
	Bigint f1, f2, f3;

	BInit(&f1);
	BInit(&f2);
	BInit(&f3);

	printf("Print 1~Nth Fibonacci. Input N : ");
	scanf("%d", &N);

	BIn(&f1, "1");
	BIn(&f2, "1");

	if (N == 1) {
		BPrint(&f1);
		return 0;
	} else if (N == 2) {
		BPrint(&f2);
		return 0;
	}

	for (i=3; i<=N; i++)
	{
		BAdd(&f3, &f1, &f2);
		BCopy(&f1, &f2);
		BCopy(&f2, &f3);
	}
	BFilePrint(&f3, fname);
		
	return 0;
}


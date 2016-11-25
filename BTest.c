#include "Bignum.h"
#include "BignumTl.h"
#include <stdio.h>
#include <stdlib.h>

#define fname "C:\\Users\\NeutrinoAnt\\Documents\\file.txt"

int BTest(void)
{
	int errnum = 0;
	Bigint B, B1, B2;
	char *out = NULL;

	BInit(&B);
	BInit(&B1);
	BInit(&B2);

	// BignumIO.c Test //
	printf("-----------------------------------\n");
	printf("--------[ BignumIO.c Test ]--------\n");
	printf("-----------------------------------\n");
	printf("[1-1] BIn(), BOut() Test           \n");
	BIn(&B, "0");                     out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "1");                     out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "2");                     out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "3");                     out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "10");                    out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "20");                    out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "30");                    out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "1000000000000000000000");out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "2000000000000000000000");out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "3000000000000000000000");out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "-123456789123456789123");out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "+987654321987654321987");out = BOut(&B, out); printf("%s\n", out); free(out);
	BIn(&B, "1561568946313128648676");out = BOut(&B, out); printf("%s\n", out); free(out);

	printf("-----------------------------------\n");
	printf("[1-2] BCompare() Test              \n");
	BPrint(BIn(&B1, "10"));
	BPrint(BIn(&B2, "10"));
	printf("cvalue: %d\n", BCompare(&B1, &B2));

	BPrint(BIn(&B1, "-9930"));
	BPrint(BIn(&B2, "-9930"));
	printf("cvalue: %d\n", BCompare(&B1, &B2));

	BPrint(BIn(&B1, "0"));
	BPrint(BIn(&B2, "0"));
	printf("cvalue: %d\n", BCompare(&B1, &B2));

	BPrint(BIn(&B1, "0"));
	BPrint(BIn(&B2, "-1"));
	printf("cvalue: %d\n", BCompare(&B1, &B2));

	BPrint(BIn(&B1, "75"));
	BPrint(BIn(&B2, "76"));
	printf("cvalue: %d\n", BCompare(&B1, &B2));

	BPrint(BIn(&B1, "1"));
	BPrint(BIn(&B2, "0"));
	printf("cvalue: %d\n", BCompare(&B1, &B2));

	printf("-----------------------------------\n");
	printf("[1-3] BPrint() Test                \n");
	BPrint(BIn(&B1, "0"));
	BPrint(BIn(&B1, "1"));
	BPrint(BIn(&B1, "34"));
	BPrint(BIn(&B1, "129"));
	BPrint(BIn(&B1, "4050"));
	BPrint(BIn(&B1, "30000"));
	BPrint(BIn(&B1, "654321"));
	BPrint(BIn(&B1, "9673958"));
	BPrint(BIn(&B1, "93729472957285438563857628"));

	printf("-----------------------------------\n");
	printf("[1-4] BFilePrint() Test            \n");
	BPrint(BIn(&B1, "93729472957285438563857628"));
	BFilePrint(&B1, fname);
	
	printf("-----------------------------------\n");
	printf("[2-1] BAdd() Test                  \n");
	BPrint(BIn(&B1, "12300000000000000000000000000000000000000000000001"));
	BPrint(BIn(&B2, "98700000000000000000000000999999999999999999999999"));
	BPrint(BAdd(&B, &B1, &B2));


	printf("-----------------------------------\n");
	printf("[2-2] BSub() Test                  \n");
	BPrint(BIn(&B1, "13000000000000000000000000000000"));
	BPrint(BIn(&B2, "12000000000000000000000000000001"));
	BPrint(BSub(&B, &B1, &B2));

	printf("-----------------------------------\n");
	printf("[2-3] BMul() Test                  \n");
	BPrint(BIn(&B1, "1"));
	BPrint(BIn(&B2, "2"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "3"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "4"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "5"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "6"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "7"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "8"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "9"));
	BPrint(BMul(&B1, &B1, &B2));
	BPrint(BIn(&B2, "10"));
	BPrint(BMul(&B1, &B1, &B2));
	
	printf("-----------------------------------\n");
	printf("[3-1] BCopy() Test                 \n");
	BPrint(BIn(&B1, "123456987654"));
	BPrint(BCopy(&B2, &B1));

	BPrint(BIn(&B1, "0"));
	BPrint(BCopy(&B2, &B1));

	BPrint(BIn(&B1, "-1234"));
	BPrint(BCopy(&B2, &B1));
	BbinPrint(&B2);

	return errnum;
}


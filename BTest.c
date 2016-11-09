#include "Bignum.h"
#include <stdio.h>
#include <stdlib.h>

int BTest(void)
{
	int errnum = 0;
	Bigint *B;
	char *out = NULL;

	// BignumIO.c Test //
	B = (Bigint *) malloc(sizeof(Bigint));
	B = BIn(B, "0");                     out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "1");                     out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "2");                     out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "3");                     out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "10");                    out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "20");                    out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "30");                    out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "1000000000000000000000");out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "2000000000000000000000");out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "3000000000000000000000");out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "-123456789123456789123");out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "+987654321987654321987");out = BOut(B, out); printf("%s\n", out); free(out);
	B = BIn(B, "1561568946313128648676");out = BOut(B, out); printf("%s\n", out); free(out);

	return errnum;
}

#include "Bignum.h"
#include <stdio.h>

int main(void)
{
	char *str = "65536";
	Bigint * bnum = BIn(str);
	int i;

	printf("%s\n\n", str);

	for (i=bnum->len-1; i>=0; i--)
		printf("%d ", bnum->num[i]);

	printf("\n\n");
	for (i=bnum->len-1; i>=0; i--, printf("\n"))
	{
		printf("%d", !!(bnum->num[i] & 0x00008000));
		printf("%d", !!(bnum->num[i] & 0x00004000));
		printf("%d", !!(bnum->num[i] & 0x00002000));
		printf("%d ", !!(bnum->num[i] & 0x00001000));
		printf("%d", !!(bnum->num[i] & 0x00000800));
		printf("%d", !!(bnum->num[i] & 0x00000400));
		printf("%d", !!(bnum->num[i] & 0x00000200));
		printf("%d ", !!(bnum->num[i] & 0x00000100));
		printf("%d", !!(bnum->num[i] & 0x00000080));
		printf("%d", !!(bnum->num[i] & 0x00000040));
		printf("%d", !!(bnum->num[i] & 0x00000020));
		printf("%d ", !!(bnum->num[i] & 0x00000010));
		printf("%d", !!(bnum->num[i] & 0x00000008));
		printf("%d", !!(bnum->num[i] & 0x00000004));
		printf("%d", !!(bnum->num[i] & 0x00000002));
		printf("%d ", !!(bnum->num[i] & 0x00000001));
	}

	printf("\n\n");

	return 0;
}


#include "Bignum.h"
#include <stdio.h>
#include <stdlib.h>

Bigint * BCopy(Bigint *dest, Bigint *src)
{
	int i;
	int len = src->len;
	unsigned int *src_t = src->num;
	unsigned int *temp = (unsigned int *)malloc(len * sizeof(unsigned int));
	
	for (i=0; i<len; i++)
		temp[i] = src_t[i];

	if (dest->num != NULL)
		free(dest->num);
	dest->len = len;
	dest->num = temp;
	dest->sign = src->sign;

	return dest;
}

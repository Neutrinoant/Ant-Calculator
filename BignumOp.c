#include "Bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Bigint * BAdd(Bigint *result, Bigint *B1, Bigint *B2)
{
	int len;
	int i, j;

	len = (B1->len < B2->len)? B1->len : B2->len;

	if (result == NULL)
		result = (Bigint *)malloc(sizeof(Bigint));

	// 덧셈 //
	for (i=0; i<len; i++);
}

// 미완성 //

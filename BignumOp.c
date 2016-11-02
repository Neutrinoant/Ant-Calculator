#include "Bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ★len의 보정이 필요한지 나중에 확인 바람.
// ex) 000 000 001이면 len=1이지 len=3이 아님.
void BcarryIn(Bigint *B)
{
	int i;
	int n = B->len;
	unsigned int *num = B->num;

	num[n] = 0;

	// carry in 수행(각 자리마다 최대 한 자리 carry 가능) //
	for (i=0; i<n; i++)
	{
		num[i+1] += num[i] >> 16;
		num[i] = num[i] & 0x0000FFFF;
	}

	// 가장 높은 자리수에서 carry발생시 len 업뎃 //
	if (num[n] > 0)
		B->len = n+1;
}

Bigint * BAdd(Bigint *result, Bigint *B1, Bigint *B2)
{
	unsigned int *n;
	unsigned int *n1 = B1->num;
	unsigned int *n2 = B2->num;
	int len1 = B1->len;
	int len2 = B2->len;
	int i;
	
	int flag = (B1->len < B2->len)? 1 : 0;

	if (flag == 1)
		n = (unsigned int *)calloc(len2+1, sizeof(unsigned int));
	else
		n = (unsigned int *)calloc(len1+1, sizeof(unsigned int));

	// 덧셈 //
	if (flag == 1)
	{
		for (i=0; i<len1; i++)
			n[i] = n1[i] + n2[i];
		for (i=len1; i<len2; i++)
			n[i] = n2[i];
	}
	else
	{
		for (i=0; i<len2; i++)
			n[i] = n1[i] + n2[i];
		for (i=len2; i<len1; i++)
			n[i] = n1[i];
	}
		
	if (result->num != NULL)
	{
		free(result->num);
		result->num = n;
	}
	result->len = (flag)? B2->len : B1->len;
	result->sign = POS;
	BcarryIn(result);

	return result;
}

// 미완성 //

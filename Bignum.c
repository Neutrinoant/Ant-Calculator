#include "Bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Bigint * BIn(char * snum)
{
	Bigint *Bnum = (Bigint *)malloc(sizeof(Bigint));
	char *snum_t;  // 십진수를 역순으로 저장
	char *bnum_t;  // 이진수를 역순으로 저장
	int slen, blen, blen_t, Blen;
	int i, j;
	int n;  // snum의 실시간 최대길이
	int temp;

	if (snum[0] == '-' || snum[0] == '+')  // 부호가 붙음
		slen = strlen(snum) - 1;
	else                                   // 부호 안붙음
		slen = strlen(snum);
	snum_t = (char *)malloc((slen+1) * sizeof(char));
	n = slen;
	
	blen_t = (int)floor(slen / log10(2.0)) + 1 + 16;   // blen, slen에 관한 특수한 부등식 사용
	bnum_t = (char *)malloc(blen_t * sizeof(char));

	// snum을 idx[1]부터 역순으로 저장 //
	snum_t[0] = 0;
	if (snum[0] == '-' || snum[0] == '+')  // 부호가 붙음
		for (i=1; i<=slen; i++)
			snum_t[i] = snum[slen-i+1] - '0';
	else                                   // 부호 안붙음
		for (i=1; i<=slen; i++)
			snum_t[i] = snum[slen-i] - '0';

	// 10진수 -> 2진수 변환 //
	blen = 0;
	while (n > 0)
	{
		snum_t[0] = 0;
		for (i=0; i<=n-1; i++)
		{
			snum_t[i] = (snum_t[i+1] % 2) * 5 + snum_t[i];
			snum_t[i+1] = snum_t[i+1] / 2;
		}
		bnum_t[blen++] = snum_t[0] & 1;
		n = n - !(snum_t[n]);
	}
	while (blen%16 != 0)
		bnum_t[blen++] = 0;   // 

	// 2진수를 Bnum에 저장 //
	Blen = (int)ceil(blen / 16.0);
	Bnum->len = Blen;
	Bnum->num = (unsigned int *)calloc(Blen, sizeof(unsigned int));
	
	for (i=0; i<Blen; i++)
	{
		for (j=0, temp=0; j<16; j++)
			temp += bnum_t[16*i+j] << j;
		Bnum->num[i] = temp;
	}

	if (snum[0] == '-')
		Bnum->sign = NEG;
	else if (snum[0] == '+')
		Bnum->sign = POS;
	else      // 부호 안붙음
		Bnum->sign = POS;

	// 동적 메모리 해제 //
	free(snum_t);
	free(bnum_t);
	
	return Bnum;
}


char * BOut(Bigint *bnum)
{
}

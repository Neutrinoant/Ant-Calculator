#include "Bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void BInit(Bigint *Bnum)
{
	Bnum->num = NULL;
	Bnum->len = 0;
	Bnum->sign = ZRO;
}

Bigint * BIn(Bigint *Bnum, char *snum)
{
	char *snum_t;  // 십진수를 역순으로 저장
	char *bnum_t;  // 이진수를 역순으로 저장
	int slen, blen, blen_t, Blen;
	int i, j;
	int n;  // snum의 실시간 최대길이
	int temp;

	// 초기화된 값이 있으면 덮어씌움 (기존값 제거) //
	if (Bnum->num != NULL)
		free(Bnum->num);

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
		if (strcmp(snum, "0") == 0)
			Bnum->sign = ZRO;
		else
			Bnum->sign = POS;

	// 동적 메모리 해제 //
	free(snum_t);
	free(bnum_t);
	
	return Bnum;
}


void carryIn(unsigned char *snum, int *len)
{
	int i;
	int n = *len;

	snum[n] = 0;

	// carry in 수행(각 자리마다 두 자리 carry 가능) //
	for (i=0; i<n; i++)
	{
		snum[i+1] += snum[i] / 10;
		snum[i] = snum[i] % 10;
	}
	// 가장 높은 자리수에서 carry발생시 len 업뎃 //
	if ((int)(snum[n]) >= 10)               // 두 자리 업뎃
	{
		snum[n+1] = snum[n] / 10;
		snum[n] = snum[n] % 10;
		*len = n+2;
	}
	else if (snum[n] > 0)  // 한자리 업뎃
		*len = n+1;
}

char * BOut(Bigint *Bnum, char *snum)
{
	unsigned char *snum_t;  // 임시 snum
	int slen, blen, Blen;
	int i, j, k;
	int n;   // snum의 실시간 최대길이

	Blen = Bnum->len;
	blen = 16 * Blen;
	slen = (int)floor(blen * log10(2.0)) + 1;  // blen, slen에 관한 특수한 부등식 사용
	snum_t = (unsigned char *)calloc(slen, sizeof(unsigned char));
	n = 1;

	snum = (char *)malloc((slen+1) * sizeof(char));

	// 16자리씩 십진수로 변환 //
	for (i=0; i<Blen; i++)
	{
		for (j=0; j<4; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
		
		for (j=4; j<8; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
		
		for (j=8; j<12; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
		
		for (j=12; j<16; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
	}

	// 아스키코드 숫자로 변경 //
	for (i=0; i<n; i++)
		snum_t[i] += '0';

	snum = (char *)malloc((slen+1) * sizeof(char));
	strcpy(snum, (char *)snum_t);
	free(snum_t);

	snum[n] = '\0';
	snum = strrev(snum);
	return snum;
}

void BbinPrint(Bigint *Bnum)
{
	int Blen = Bnum->len;
	int i, j;

	if (Bnum->sign == NEG)
		printf("(-)");

	// 16자리씩 출력 (4+4+4+4) //
	for (i=0; i<Blen; i++)
	{
		for (j=0; j<4; j++)
			printf("%d",(Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		putchar(' ');
		for (j=4; j<8; j++)
			printf("%d",(Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		putchar(' ');
		for (j=8; j<12; j++)
			printf("%d",(Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		putchar(' ');
		for (j=12; j<16; j++)
			printf("%d",(Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		putchar(' ');
		putchar('\n');
	}
}

void BPrint(Bigint *Bnum)
{
	unsigned char *snum_t;  // DN을 저장할 변수
	int slen, blen, Blen;
	int i, j, k;
	int n;   // snum의 실시간 최대길이

	Blen = Bnum->len;
	blen = 16 * Blen;
	slen = (int)floor(blen * log10(2.0)) + 1;  // blen, slen에 관한 특수한 부등식 사용
	snum_t = (unsigned char *)calloc(slen, sizeof(unsigned char));
	n = 1;

	// 16자리씩 십진수로 변환 //
	for (i=0; i<Blen; i++)
	{
		for (j=0; j<4; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
		
		for (j=4; j<8; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
		
		for (j=8; j<12; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
		
		for (j=12; j<16; j++)
		{
			for (k=0; k<n; k++)
				snum_t[k] = snum_t[k] * 2;
			snum_t[0] = snum_t[0] + ((Bnum->num[Blen-1-i] & (0x1 << (15-j))) >> (15-j));
		}
		carryIn(snum_t, &n); // 자리올림후 n 업뎃
	}

	// DN 출력 //
	if (Bnum->sign == NEG)
		printf("-");

	for (i=n-1; i>=0; i--)
		printf("%d", snum_t[i]);
	putchar('\n');

	free(snum_t);
}

#include "Bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX(x,y) ((x)>(y))?(x):(y)

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



// 큰 두 양수의 덧셈 //
Bigint * BAdd(Bigint *result, Bigint *B1, Bigint *B2)
{
	unsigned int *n;
	unsigned int *n1 = B1->num;
	unsigned int *n2 = B2->num;
	int len1 = B1->len;
	int len2 = B2->len;
	int i;
	
	int flag = (B1->len < B2->len)? 1 : 0;

	if (flag == 1)  // B2가 더 길면
		n = (unsigned int *)calloc(len2+1, sizeof(unsigned int));
	else
		n = (unsigned int *)calloc(len1+1, sizeof(unsigned int));

	// 덧셈 //
	if (flag == 1)  // B2가 더 길면
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
	
	// 초기화된 값이 있으면 덮어씌움 (기존값 제거) //
	if (result->num != NULL)
		free(result->num);
	result->num = n;
	result->len = (flag)? B2->len : B1->len;
	result->sign = POS;
	BcarryIn(result);  // 자리올림

	return result;
}

// 두 큰 정수의 비교 //
int BCompare(Bigint *B1, Bigint *B2)
{
	unsigned int *p1, *p2;
	int s1 = B1->sign;
	int s2 = B2->sign;
	int len;
	int i;

	// 두 수에 0이 포함될 때
	if (s1 == ZRO && s2 == ZRO)
		return ZRO;
	else if (s1 == ZRO)
		return -s2;
	else if (s2 == ZRO)
		return s1;

	// 두 수의 부호가 다를때
	if (s1*s2 == NEG)
	{
		if (s1 == NEG) return NEG;
		else           return POS;
	}
	
	// 두 수의 부호가 같을때
	if (B1->len > B2->len)
		return s1;  // (+):B1>B2, (-):B1<B2
	else if (B1->len < B2->len)
		return -s1; // (+):B1>B2, (-):B1<B2

	// 두 수의 길이가 같을때
	len = B1->len; p1 = B1->num, p2 = B2->num;
	for (i=len-1; i>=0; i--)
		if (p1[i] != p2[i])
			break;

	if (i < 0)
		return ZRO;   // 같음
	else if (p1[i] > p2[i])
		return s1;  // (+):B1>B2, (-):B1<B2
	else // (p1[i] < p2[i])
		return -s1; // (+):B1>B2, (-):B1<B2
}

// 큰 두 양수의 뺄셈 //
Bigint * BSub(Bigint *result, Bigint *B1, Bigint *B2)
{
	unsigned int *n, *nb, *ns;
	int lenb, lens, lenr;
	int i;
	
	int flag;
	
	// 두 수의 크기 비교
	flag = BCompare(B1, B2);

	if (flag == 1) {  // 결과가 양수
		nb = B1->num, ns = B2->num;
		lenb = B1->len, lens = B2->len;
	} else if (flag == -1) {  // 결과가 음수
		nb = B2->num, ns = B1->num;
		lenb = B2->len, lens = B1->len;
	} else {     // 결과가 0 (같음)
		// 초기화된 값이 있으면 덮어씌움 //
		if (result->num != NULL)
			free(result->num);
		result->num = (unsigned int *)calloc(1, sizeof(unsigned int));
		result->len = 1;
		result->sign = ZRO;
		return result;
	}

	n = (unsigned int *)calloc(lenb, sizeof(unsigned int));

	// 뺄셈 //
	n[0] = nb[0] - ns[0] + ((lenb <= 1)? 0 : 0x00010000);     // 큰수-작은수+carry(받음)

	for (i=1; i<lens-1; i++)
		n[i] = nb[i] - ns[i] + 0x00010000 - 1; // 큰수-작은수+carry(받음)-carry(줌)
	
	if (lenb > lens)
		n[lens-1] = nb[lens-1] - ns[lens-1] + 0x00010000 - 1; // 큰수+carry(받음)-carry(줌)
	else // (lenb == lens)
		n[lens-1] = nb[lens-1] - ns[lens-1] - 1;              // 큰수-carry(줌)

	for (i=lens; i<lenb-1; i++)
		n[i] = nb[i] + 0x00010000 - 1;         // 큰수+carry(받음)-carry(줌)

	if (lenb > lens)
		n[lenb-1] = nb[lenb-1] - 1;            // 큰수-carry(줌)

	// carry in 수행(각 자리마다 최대 한 자리 carry 가능) //
	for (i=0; i<lenb-1; i++)
	{
		n[i+1] += n[i] >> 16;
		n[i] = n[i] & 0x0000FFFF;
	}
	
	// 수의 길이 조정 //
	lenr = lenb;
	for (i=lenb-1; i>=1; i--)
	{
		if (n[i] != 0)
			break;
		lenr--;
	}

	if (result->num != NULL)
		free(result->num);
	result->num = n;
	result->len = lenr;
	result->sign = flag;

	return result;
}

// 큰 두 양수의 곱셈 //
Bigint * BMul(Bigint *result, Bigint *B1, Bigint *B2)
{
	unsigned int *n, *nb, *ns;
	int lenb, lens, lenr;
	int i, j, k;
	unsigned int temp;
	
	int flag = (B1->len > B2->len)? 1 : 0;

	// 곱셈의 순서 결정 //
	if (flag == 1) {  // B1이 더 길때
		nb = B1->num, ns = B2->num;
		lenb = B1->len, lens = B2->len;
	} else { // (flag == -1)  // B2가 더 길때
		nb = B2->num, ns = B1->num;
		lenb = B2->len, lens = B1->len;
	}

	n = (unsigned int *)calloc(lenb+lens+1, sizeof(unsigned int));

	// 자릿수별로 곱셈하면서 누적 //
	for (i=0; i<lens; i++)
	{
		for (j=0; j<lenb; j++)
			n[i+j] += nb[j] * ns[i];

		// 누적 후 자리 올림 //
		for (k=i; k<i+lenb; k++)
		{
			temp = n[k];
			if (temp > 0x00010000)
			{
				n[k+1] += temp >> 16;
				n[k] = temp & 0x0000FFFF;
			}
		}
	}

	// 계산 결과의 길이 계산 //
	if (n[lenb+lens-1] > 0)
		lenr = lenb+lens;
	else
		lenr = lenb+lens-1;
	
	// 초기화된 값이 있으면 덮어씌움 (기존값 제거) //
	if (result->num != NULL);
		free(result->num);
	result->num = n;
	result->len = lenr;
	result->sign = POS;

	return result;
}

/*
Bigint * BDiv(Bigint *result, Bigint *B1, Bigint *B2)
{
	int i;
	unsigned int *rem;
	unsigned int *div;
	unsigned int *divt;
	unsigned int *num;
	unsigned int *quo;
	
	int temp1, temp2;  // 부호가 필요해서

	int Blen = B1->len;
	int dlen = B2->len;
	int rlen;
	int qlen;
	int nBlock;

	int sign = B1->sign ^ B2->sign;
	int curpos = Blen-1;  // 데이터를 가져올 위치
	int flag;
	int udist, ldist, dist;
	int dlowbound;
	
	rem = (unsigned int *)calloc(dlen+1, sizeof(unsigned int));
	
	div = B2->num;

	divt = (unsigned int *)calloc(dlen+1, sizeof(unsigned int));
	for (i=0; i<dlen; i++)
		divt[i] = div[i];   // div를 divt에 복사
	dlowbound = 0; // divt의 위치를 옮길 때 하한의 위치

	num = B1->num;
	quo = (unsigned int *)calloc(Blen-dlen+1, sizeof(unsigned int));



	// rem와 div의 길이를 같게 세팅  //

	// 1-1. num의 최고자리에서 dlen만큼 가져옴 (필요)
	for (i=0; i<dlen; i++)
		rem[dlen-1-i] = num[curpos--];
	rlen = dlen;

	// 나눗셈 //
	while (1)
	{
		ldist = (int)floor(log(1.*divt[dlen-1])/log(2.)) + 1;

		if (rlen == 0) {
			udist = 0;
			dist = udist - ldist + (1 - dlen) * 16;
		} else {
			udist = (int)(log(1.*rem[rlen-1])/log(2.)) + 1;
			dist = udist - ldist + (rlen - dlen) * 16;  // assumption : dist < 16
		}

		// 2-1. 길이 맞추기 1 //

		// 필요할때 divt를 이동시킬 수 없거나, rem이 부족하면, 블록을 추가 //
		if ((dist < 0 && dlowbound+dist < 0) || (dist==0 && dlowbound==0)) 
		{
			if (dist == 0)
				nBlock = 1;
			else if (dist%16 == 0)
				nBlock = dist / 16;
			else
				nBlock = dist / 16 + 1;

			if (curpos + 1 - nBlock < 0)
				break;  // 블록을 추가할 수 없어, 더이상 나눌 수 없으므로 중단

			rlen = rlen + nBlock;
			for (i=rlen-1; i>=nBlock; i--)
				rem[i] = rem[i-nBlock];

			for (i=nBlock-1; i>=0; i--)
				rem[i] = num[curpos--];

			// update dist 
			udist = (int)(log(1.*rem[rlen-1])/log(2.)) + 1;
			ldist = (int)(log(1.*divt[dlen-1])/log(2.)) + 1;
			dist = udist - ldist + (rlen - dlen) * 16; 
		}
		else if (dist < 0 && dlowbound+dist >= 0)  // 거리가 음수지만 div를 right shift가능하다면 해줌
		{
			temp1 = divt[0] >> (-dist);
			for (i=1; i<dlen; i++)
			{
				temp2 = divt[i];
				divt[i-1] = temp1 | ((temp2 << (16 - (-dist))) & 0x0000FFFF);
				temp1 = temp2 >> (-dist);
			}
			divt[dlen-1] = temp1;
			dlowbound = dlowbound - (-dist);

			if (divt[dlen-1] == 0)
				dlen--;  // shift 후 길이 재 조정 //
		}
		
		if (dist > 0)  // 거리가 양수이면 div를 left shift
		{
			temp1 = divt[rlen-1] << ((udist - ldist + 16) % 16);  // for positive modulo
			for (i=rlen-2; i>=0; i--)
			{
				temp2 = divt[i];
				divt[i+1] = temp1 | (temp2 >> ((ldist - udist + 16) % 16));  // for positive modulo
				temp1 = (temp2 << dist) & 0x0000FFFF;
			}
			divt[0] = temp1;
			dlowbound = dlowbound + dist;

			if (ldist + dist > 16)
				dlen++;  // shift 후 길이 재 조정 //
		}
		
		// 2-2. 길이 맞추기 2 (세부길이) //

		flag = 0;
		for (i=rlen-1; i>=0; i--)   // 크기 비교 후 결정
		{
			flag = rem[i] - divt[i];
			if (flag > 0 || flag < 0)
				break;
		}

		if (flag < 0)  // rem이 div보다 작으면, div를 1회 shift
		{
			if (dlowbound == 0)
				continue;   // div를 shift할 수 없으면 (2-1)로.

			temp1 = divt[0] >> 1;
			for (i=1; i<dlen; i++)
			{
				temp2 = divt[i];
				divt[i-1] = temp1 | ((temp2 & 0x1) << 15);
				temp1 = temp2 >> 1;
			}
			divt[dlen-1] = temp1;
			dlowbound = dlowbound - 1;

			// shift 후 길이 재 조정 //
			if (divt[dlen-1] == 0)
				dlen--;
		}

		// 2-3. 뺄셈 //

		flag = 0;  // carryin 역할
		for (i=0; i<rlen; i++)
		{
			temp1 = rem[i] - divt[i] - flag;
			if (temp1 < 0)
			{
				flag = 1;
				rem[i] = temp1 + 0x00010000;  // carry in
			}
			else
			{
				flag = 0;
				rem[i] = temp1;
			}
		}

		// 뺄셈 후 길이 재 조정 (0은 길이 0으로 취급) //
		i = rlen-1;
		for (i=rlen-1; i>=0; i--)
			if (rem[i] == 0) {
				rlen--;
				break;
			}
		
		// 2-4. 몫을 계산 //
		quo[curpos+1] = quo[curpos+1] | (1 << dlowbound);
		printf("quo[%d] = %d\n", curpos+1, quo[curpos+1]);
	}

	if (quo[Blen-dlen] > 0)
		qlen = Blen-dlen+1;
	else
		qlen = Blen-dlen;

	// 초기화된 값이 있으면 덮어씌움 (기존값 제거) //
	if (result->num != NULL);
		free(result->num);
	result->num = quo;
	result->len = qlen;
	result->sign = sign;
	
	return result;
}

*/

Bigint * BDiv(Bigint *result, Bigint *B1, Bigint *B2)
{
	int i;
	unsigned int *rem;
	unsigned int *div;
	unsigned int *divt;
	unsigned int *num;
	unsigned int *quo;
	
	int temp1, temp2;  // 부호가 필요해서

	int Blen = B1->len;
	int Dlen = B2->len;
	int dlen = Dlen;  // 임시 divisor length
	int rlen;
	int qlen;
	int nBlock;

	int sign = B1->sign ^ B2->sign;
	int curpos;   // 데이터를 가져올 위치
	int flag;
	int udist, ldist, dist;
	int dlow;    // div의 하한 index

	int Bf = (Blen > Dlen);
	int Sf;
	int Cf;
	
	rem = (unsigned int *)calloc(dlen+1, sizeof(unsigned int));
	
	div = B2->num;

	divt = (unsigned int *)calloc(dlen+1, sizeof(unsigned int));
	for (i=0; i<dlen; i++)
		divt[i] = div[i];   // div를 divt에 복사
	dlow = 0; // divt의 위치를 옮길 때 하한의 위치

	num = B1->num;
	quo = (unsigned int *)calloc(Blen-dlen+1, sizeof(unsigned int));

	// 0으로 나누기는 금지 //
	if (dlen == 1 && div[0] == 0)
		return BIn(result, "NAN");

	// Divisor의 길이가 더 길때 //
	if (Bf == -1)
		return BIn(result, "0");

	// Default : num에서 div의 크기만큼 자르기 //
	curpos = Blen-1;
	for (i=0; i<dlen; i++)
		rem[dlen-1-i] = num[curpos--];
	rlen = dlen;

	// Bignum의 길이가 더 길때 //
	while (Bf != 0)
	{
		// 세부길이 계산 //
		ldist = (int)floor(log(1.*divt[dlen-1])/log(2.)) + 1;
		if (rlen == 1 && rem[0] == 0)  // rem이 0이면
		{
			udist = 0;
			dist = udist - ldist + (rlen - dlen) * 16;
			Sf = dist;
		}
		else
		{
			udist = (int)(log(1.*rem[rlen-1])/log(2.)) + 1;
			dist = udist - ldist + (rlen - dlen) * 16;
			Sf = dist;
		}

		if (Sf < 0 && dlow >= abs(dist))
		{
			// dist 만큼 div를 rshift //
			temp1 = divt[0] >> (-dist);
			for (i=1; i<dlen; i++)
			{
				temp2 = divt[i];
				divt[i-1] = temp1 | ((temp2 << (16 - (-dist))) & 0x0000FFFF);
				temp1 = temp2 >> (-dist);
			}
			divt[dlen-1] = temp1;
			dlow = dlow - (-dist);

			// shift 후 길이 재 조정 //
			if (divt[dlen-1] == 0)
				dlen--;
		}
		else if (Sf < 0 && dlow < abs(dist))
		{
			if (curpos < 0)
				break;  //////////////조심!!

			// 블록 1개 추가 //
			nBlock = 1;
			rlen = rlen + nBlock;
			for (i=rlen-1; i>=nBlock; i--)
				rem[rlen-1] = rem[rlen-1-nBlock];
			rem[0] = num[curpos--];

			Bf = (Blen > Dlen);

			if (rlen > dlen)
			{
				// dist만큼 div를 lshift (여기 다시한번 주의깊게 보자) //
				temp1 = divt[rlen-1] << ((udist - ldist + 16) % 16);  // for positive modulo
				for (i=rlen-2; i>=0; i--)
				{
					temp2 = divt[i];
					divt[i+1] = temp1 | (temp2 >> ((ldist - udist + 16) % 16));  // for positive modulo
					temp1 = (temp2 << abs(dist)) & 0x0000FFFF;
				}
				divt[0] = temp1;
				dlow = dlow + abs(dist);

				if (ldist + abs(dist) > 16)
					dlen++;  // shift 후 길이 재 조정 //
			}
		}


		/// 덜했음

	}



	while (1)
	{
		




	}


}

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

Bigint * RLShift(Bigint *B, int dist)
{
	unsigned int *num = B->num;
	unsigned int *result;
	int temp1, temp2;
	int len = B->len;
	int tdist;
	int i, q;

	// dist==0 이면 수행X //
	if (dist == 0)
		return B;

	tdist = (int)(log(1.* num[len-1])/log(2.)) + 1;

	// dist>0 이면 lshift //
	if (dist > 0)
	{
		q = (dist + tdist - 1) / 16;  // 블록 사이 이동 횟수

		result = (unsigned int *)calloc(len+q, sizeof(unsigned int));
		for (i=0; i<len; i++)
			result[i] = num[i];

		// Case 1 //
		if ((dist%16) <= (16-tdist))
		{
			// dist만큼 div를 lshift //
			temp1 = result[len-1] << (dist % 16);
			for (i=len-2; i>=0; i--)
			{
				temp2 = result[i];
				result[i+1+q] = temp1 | (temp2 >> (16 - (dist % 16)));
				temp1 = (temp2 << (dist % 16)) & 0x0000FFFF;
			}
			result[q] = temp1;
			for (i=q-1; i>=0; i--)
				result[i] = 0;  // shift된 값들은 0으로
		}
		// Case 2 //
		else if ((dist%16) > (16-tdist))
		{
			// dist만큼 div를 lshift //
			result[len-1+q] = result[len-1] >> (16 - (dist % 16));
			temp1 = result[len-1] << (dist % 16);
			for (i=len-2; i>=0; i--)
			{
				temp2 = result[i];
				result[i+q] = temp1 | (temp2 >> (16 - (dist % 16)));
				temp1 = (temp2 << (dist % 16)) & 0x0000FFFF;
			}
			result[q-1] = temp1;
			for (i=q-2; i>=0; i--)
				result[i] = 0;  // shift된 값들은 0으로
		}

		len = len + q;  // 길이 조정
	}
	else // (dist < 0)
	{
		q = (-dist) / 16;  // 블록 사이 이동 횟수

		// |dist| 만큼 div를 rshift //
		temp1 = num[0] >> ((-dist) % 16);
		for (i=1; i<len; i++)
		{
			temp2 = num[i];
			if ((i-1-q) >= 0)
				num[i-1-q] = temp1 | ((temp2 << (16 - (-dist) % 16)) & 0x0000FFFF);
			temp1 = temp2 >> ((-dist) % 16);
		}
		num[len-1-q] = temp1;
		for (i=len-q; i<len; i++)
			num[i] = 0;  // shift된 값들은 0으로

		len = len + (dist + tdist - 16) / 16;  // 길이 조정

		result = (unsigned int *)calloc(len, sizeof(unsigned int));
		for (i=0; i<len; i++)
			result[i] = num[i];
	}

	free(num);
	
	B->num = result;
	B->len = len;
	return B;
}

Bigint * BDiv(Bigint *result, Bigint *B1, Bigint *B2)
{
	unsigned int *num = B1->num;
	unsigned int *div = B2->num;
	unsigned int *divt;

	int nlen = B1->len;
	int dlen = B2->len;
	
	int udist, ldist, dist;
	int dlow;    // div의 하한 index
	int i;

	// 0으로 나누기는 금지 //
	if (dlen == 1 && div[0] == 0)
		return BIn(result, "NAN");

	// Divisor의 길이가 더 길때 //
	if (BCompare(B1, B2) == -1)
		return BIn(result, "0");

	divt = (unsigned int *)calloc(nlen, sizeof(unsigned int));
	for (i=0; i<dlen; i++)
		divt[i] = div[i];   // div를 divt에 복사
	dlow = 0; // divt의 위치를 옮길 때 하한의 위치

	ldist = (int)floor(log(1.*divt[dlen-1])/log(2.)) + 1;
	udist = (int)(log(1.*num[nlen-1])/log(2.)) + 1;
	dist = udist - ldist + (nlen - dlen) * 16;

	// 미완성 //
}

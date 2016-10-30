#ifndef __BIGNUM_H__
#define __BIGNUM_H__

// 부호 정의 //
#define POS 1
#define NEG -1
#define ZRO 0

typedef struct {
	/*
	 * 수를 저장할 배열.
	 * 각 인덱스에 2^15-1 까지 저장 가능하며 (2^16 진법)
	 * Little endian으로 저장.
	 */
	unsigned int *num;

	/*
	 * 수의 길이를 저장 (배열의 인덱스 n - 1)
	 */
	int len;

	/*
	 * 부호를 저장 (POS, NEG, ZRO)
	 */
	int sign;
} Bigint;

// 10진수(string) -> 2진수(bigint) 변환하여 저장 //
Bigint * BIn(char * snum);

// 2진수 -> 10진수 변환하여 반환 //
char * BOut(Bigint *bnum);

// 두 양수 덧셈 //
Bigint * BAdd(Bigint *result, Bigint *bnum1, Bigint *bnum2);

// 두 양수 뺄셈 //
Bigint * BSub(Bigint *result, Bigint *bnum1, Bigint *bnum2);

// 두 수 곱셈 //
Bigint * BMul(Bigint *result, Bigint *bnum1, Bigint *bnum2);

// 두 수 나눗셈(몫 반환) //
Bigint * BDiv(Bigint *result, Bigint *bnum1, Bigint *bnum2);

// 두 수 모듈러 //
Bigint * BMod(Bigint *result, Bigint *bnum1, Bigint *bnum2);

#endif
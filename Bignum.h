#ifndef __BIGNUM_H__
#define __BIGNUM_H__

// 부호 정의 //
#define POS 1
#define NEG -1
#define ZRO 0

typedef struct {
	/*
	 * 수를 저장할 배열.
	 * 1. 각 인덱스에 2^15-1 까지 저장 가능하며 (2^16 진법)
	 *    Little endian으로 저장.
	 * 2. 0은 num[0]=0 으로 저장 (데이터가 존재해야 연산 가능)
	 * 3. 초기화시 NULL로 초기화
	 * 4. NAN이 저장될 시 num[0]=0xFFFF0000 으로 저장 (초기화된 데이터와 혼동 방지)
	 */
	unsigned int *num;

	/*
	 * 수의 길이를 저장 (배열의 인덱스 n + 1)
	 * 1. 초기화시 길이를 0으로 정의
	 * 2. 0의 길이는 1로 정의 (0~9, 10~99, 등으로 구분 합리적)
	 * 3. NAN의 길이는 -1로 정의.
	 */
	int len;

	/*
	 * 부호를 저장 (POS, NEG, ZRO)
	 * 1. 초기화시 부호는 ZRO로 정의
	 * 2. 0의 부호는 ZRO로 정의 (부호 연산에 유리함)
	 * 3. NAN의 부호는 ZRO로 정의
	 */
	int sign;
} Bigint;

// 아래 모든 함수를 테스트 //
int BTest(void);

// 큰 수의 초기화작업 (변수 선언 후 필수 사용) //
void BInit(Bigint *Bnum);

// 10진수(string) -> 2진수(bigint) 변환하여 저장 //
Bigint * BIn(Bigint *Bnum, char * snum);

// 2진수 -> 10진수 변환하여 반환 //
char * BOut(Bigint *Bnum, char *snum);

// 큰 수를 2진수로 한 line에 16자리씩 출력 //
void BbinPrint(Bigint *Bnum);

// 큰 수를 10진수로 출력 //
void BPrint(Bigint *Bnum);

// 큰 수를 10진수로 파일에 출력 //
void BFilePrint(Bigint *Bnum, const char *fname);


// 두 큰 정수의 비교
//   POS : B1이 더 큼
//   ZRO : 두 수가 같음
//   NEG : B2이 더 큼
int BCompare(Bigint *B1, Bigint *B2);

// 두 양수 덧셈 //
Bigint * BAdd(Bigint *result, Bigint *B1, Bigint *B2);

// 두 양수 뺄셈 //
Bigint * BSub(Bigint *result, Bigint *B1, Bigint *B2);

// 두 수 곱셈 //
Bigint * BMul(Bigint *result, Bigint *B1, Bigint *B2);

// 두 수 나눗셈(몫 반환) //
Bigint * BDiv(Bigint *result, Bigint *B1, Bigint *B2);

// 두 수 모듈러 //
Bigint * BMod(Bigint *result, Bigint *B1, Bigint *B2);

#endif

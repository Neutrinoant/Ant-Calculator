#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Bignum.h"
#include "BignumTl.h"

Bigint * BPow(Bigint *result, Bigint *Bnum, int n)
{
	Bigint R;
	int i;
	int len;

	if (n < 0)
		return BIn(result, "NAN"); // 음수제곱은 허용 X
	else if (n == 0)
		return BIn(result, "1");   // 0 제곱은 1

	len = (int)floor(log(1.* n)/log(2.)) + 1;

	BInit(&R);
	BIn(&R, "1");
	
	// 지수 계산 알고리즘 //
	for (i=len-1; i>=0; i--)
	{
		BMul(&R, &R, &R);
		if ( ((n & (1<<i))>>i) == 1 )
			BMul(&R, &R, Bnum);
	}

	result->num = R.num;
	result->len = R.len;
	result->sign = (n%2 == 0)? (POS) : (R.sign);
	return result;
}

Bigint * BNthRoot(Bigint *result, Bigint *Bnum, int n)
{
	Bigint Curr;
	Bigint Prev;
	Bigint Prevt;
	Bigint temp;
	Bigint N;    // n
	Bigint N_1;  // n-1

	unsigned int *num = Bnum->num;

	int len = Bnum->len;
	int sign = Bnum->sign;

	BInit(&Curr);
	BInit(&Prev);
	BInit(&Prevt);
	BInit(&temp);
	BInit(&N);
	BInit(&N_1);
	
	if (sign == NEG && n%2 == 1)
		return BIn(result, "NAN");
	
	else if (len == 1 && num[0] == 0)
		return BIn(result, "0");

	else if (len == 1 && num[0] == 1)
		return BIn(result, "1");

		/* x_(n+1) 계산 */
		////////////////////////////////////////////////////
		//            < Newton's Method >                 //
		//   x_(n+1) = x_n - f(x_n)/f'(x_n) 을 이용하면   //
		//     f(x) = x^2 - k 일때                        //
		//     x_(n+1) = {(n-1)(x_n)^n + k}/n(x_n)^(n-1)  //
		//     이때, lim(x_n) = n_root_of(k) 가 성립한다. //
		//                                                //
		//   x_n - x_(n+1) < 1 이면                       //
		//     x_(n+1) == n_root_of(k) 로 간주한다.       //
		////////////////////////////////////////////////////
	
	BInt(&N, n);      // n값을 갖는 Bigint 변수 생성
	BInt(&N_1, n-1);  // n-1값을 갖는 Bigint 변수 생성
	
	// 초기값은 x_n = Bnum 으로 함 //
	BCopy(&Curr, Bnum);

	// Newton's Method //
	while (1)
	{
		BPrint(BCopy(&Prev, &Curr));
		BPrint(BPow(&temp, &Prev, n-1));
		BPrint(BMul(&Curr, &temp, &Prev));
		BPrint(BMul(&Curr, &Curr, &N_1));
		BPrint(BAdd(&Curr, &Curr, Bnum));
		BPrint(BMul(&temp, &temp, &N));
		BPrint(BDiv(&Curr, &Curr, &temp));

		if (BCompare(&Prev, &Curr) == 0)
			break;
	}

	free(Prev.num);
	free(temp.num);
	free(N.num);
	free(N_1.num);

	result->num = Curr.num;
	result->len = Curr.len;
	result->sign = Curr.sign;
	return result;
}

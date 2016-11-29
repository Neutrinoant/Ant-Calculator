#ifndef __BIGNUM_TL_H__
#define __BIGNUM_TL_H__

#include "Bignum.h"

// 큰 수를 n제곱 (n>=0) //
Bigint * BPow(Bigint *result, Bigint *Bnum, int n);

// 큰 수를 n제곱근 //
Bigint * BNthRoot(Bigint *result, Bigint *Bnum, int n);

#endif

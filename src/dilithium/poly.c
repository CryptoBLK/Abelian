#include <stdint.h>
#include <immintrin.h>
#include "test/cpucycles.h"
#include "fips202.h"
#include "fips202x4.h"
#include "params.h"
#include "reduce.h"
#include "rounding.h"
#include "ntt.h"
#include "poly.h"

#ifdef DBENCH
extern const unsigned long long timing_overhead;
extern unsigned long long *tred, *tadd, *tmul, *tround, *tsample, *tpack;
#endif

/*************************************************
* Name:        poly_reduce
*
* Description: Reduce all coefficients of input polynomial to representative
*              in [0,2*Q[.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_reduce(poly *a) {
  DBENCH_START();

  reduce32(a->coeffs, a->coeffs);

  DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_csubq
*
* Description: For all coefficients of input polynomial subtract Q if
*              coefficient is bigger than Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_csubq(poly *a) {
  DBENCH_START();

  csubq(a->coeffs, a->coeffs);

  DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_freeze
*
* Description: Reduce all coefficients of the polynomial to standard
*              representatives.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_freeze(poly *a) {
  DBENCH_START();

  reduce32(a->coeffs, a->coeffs); //FIXME
  csubq(a->coeffs, a->coeffs);

  DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_add
*
* Description: Add polynomials. No modular reduction is performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first summand
*              - const poly *b: pointer to second summand
**************************************************/
void poly_add(poly *c, const poly *a, const poly *b)  {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N; ++i)
    c->coeffs[i] = a->coeffs[i] + b->coeffs[i];

  DBENCH_STOP(*tadd);
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract polynomials. Assumes coefficients of second input
*              polynomial to be less than 2*Q. No modular reduction is
*              performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial to be
*                               subtraced from first input polynomial
**************************************************/
void poly_sub(poly *c, const poly *a, const poly *b) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N; ++i)
    c->coeffs[i] = a->coeffs[i] + 2*_Q - b->coeffs[i];

  DBENCH_STOP(*tadd);
}

/*************************************************
* Name:        poly_shiftl
*
* Description: Multiply polynomial by 2^k without modular reduction. Assumes
*              input coefficients to be less than 2^{32-k}.
*
* Arguments:   - poly *a: pointer to input/output polynomial
*              - unsigned int k: exponent
**************************************************/
void poly_shiftl(poly *a, unsigned int k) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N; ++i)
    a->coeffs[i] <<= k;

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_ntt
*
* Description: Forward NTT. Output coefficients can be up to 16*Q larger than
*              input coefficients.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_ntt(poly *a) {
  DBENCH_START();

  ntt(a->coeffs, a->coeffs, zetas);

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_invntt_montgomery
*
* Description: Inverse NTT and multiplication with 2^{32}. Input coefficients
*              need to be less than 2*Q. Output coefficients are less than 2*Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_invntt_montgomery(poly *a) {
  DBENCH_START();

  invntt(a->coeffs, a->coeffs, zetas_inv);

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_pointwise_invmontgomery
*
* Description: Pointwise multiplication of polynomials in NTT domain
*              representation and multiplication of resulting polynomial
*              with 2^{-32}. Output coefficients are less than 2*Q if input
*              coefficient are less than 22*Q.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_pointwise_invmontgomery(poly *c, const poly *a, const poly *b) {
  DBENCH_START();

  pointwise_mul(c->coeffs, a->coeffs, b->coeffs);

  DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_power2round
*
* Description: For all coefficients c of the input polynomial,
*              compute c0, c1 such that c mod Q = c1*2^D + c0
*              with -2^{D-1} < c0 <= 2^{D-1}. Assumes coefficients to be
*              standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients Q + a0
*              - const poly *v: pointer to input polynomial
**************************************************/
void poly_power2round(poly *a1, poly *a0, const poly *a) {
  unsigned int i;
  DBENCH_START();
  
  for(i = 0; i < _N; ++i)
    a1->coeffs[i] = power2round(a->coeffs[i], a0->coeffs+i);

  DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_decompose
*
* Description: For all coefficients c of the input polynomial,
*              compute high and low bits c0, c1 such c mod Q = c1*ALPHA + c0
*              with -ALPHA/2 < c0 <= ALPHA/2 except c1 = (Q-1)/ALPHA where we
*              set c1 = 0 and -ALPHA/2 <= c0 = c mod Q - Q < 0.
*              Assumes coefficients to be standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients Q + a0
*              - const poly *c: pointer to input polynomial
**************************************************/
void poly_decompose(poly *a1, poly *a0, const poly *a) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N; ++i)
    a1->coeffs[i] = decompose(a->coeffs[i], a0->coeffs+i);

  DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_make_hint
*
* Description: Compute hint polynomial. The coefficients of which indicate
*              whether the low bits of the corresponding coefficient of
*              the input polynomial overflow into the high bits.
*
* Arguments:   - poly *h: pointer to output hint polynomial
*              - const poly *a0: pointer to low part of input polynomial
*              - const poly *a1: pointer to high part of input polynomial
*
* Returns number of 1 bits.
**************************************************/
unsigned int poly_make_hint(poly *h, const poly *a0, const poly *a1) {
  unsigned int i, s = 0;
  DBENCH_START();

  for(i = 0; i < _N; ++i) {
    h->coeffs[i] = make_hint(a0->coeffs[i], a1->coeffs[i]);
    s += h->coeffs[i];
  }

  DBENCH_STOP(*tround);
  return s;
}

/*************************************************
* Name:        poly_use_hint
*
* Description: Use hint polynomial to correct the high bits of a polynomial.
*
* Arguments:   - poly *a: pointer to output polynomial with corrected high bits
*              - const poly *b: pointer to input polynomial
*              - const poly *h: pointer to input hint polynomial
**************************************************/
void poly_use_hint(poly *a, const poly *b, const poly *h) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N; ++i)
    a->coeffs[i] = use_hint(b->coeffs[i], h->coeffs[i]);

  DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_chknorm
*
* Description: Check infinity norm of polynomial against given bound.
*              Assumes input coefficients to be standard representatives.
*
* Arguments:   - const poly *a: pointer to polynomial
*              - uint32_t B: norm bound
*
* Returns 0 if norm is strictly smaller than B and 1 otherwise.
**************************************************/
int poly_chknorm(const poly *a, uint32_t B) {
  unsigned int i;
  int32_t t;
  DBENCH_START();

  /* It is ok to leak which coefficient violates the bound since
     the probability for each coefficient is independent of secret
     data but we must not leak the sign of the centralized representative. */
  for(i = 0; i < _N; ++i) {
    /* Absolute value of centralized representative */
    t = (_Q-1)/2 - a->coeffs[i];
    t ^= (t >> 31);
    t = (_Q-1)/2 - t;

    if((uint32_t)t >= B) {
      DBENCH_STOP(*tsample);
      return 1;
    }
  }

  DBENCH_STOP(*tsample);
  return 0;
}

/*************************************************
* Name:        poly_uniform
*
* Description: Sample uniformly random polynomial using stream of random bytes.
*              Assumes that enough random bytes are given (e.g.
*              5*SHAKE128_RATE bytes).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const unsigned char *buf: array of random bytes
**************************************************/
void poly_uniform(poly *a, const unsigned char *buf) {
  unsigned int ctr, pos;
  uint32_t t;
  DBENCH_START();

  ctr = pos = 0;
  while(ctr < _N) {
    t  = buf[pos++];
    t |= (uint32_t)buf[pos++] << 8;
    t |= (uint32_t)buf[pos++] << 16;
    t &= 0x7FFFFF;

    if(t < _Q)
      a->coeffs[ctr++] = t;
  }

  DBENCH_STOP(*tsample);
}

/*************************************************
* Name:        rej_eta
*
* Description: Sample uniformly random coefficients in [-ETA, ETA] by
*              performing rejection sampling using array of random bytes.
*
* Arguments:   - uint32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const unsigned char *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_eta(uint32_t *a,
                            unsigned int len,
                            const unsigned char *buf,
                            unsigned int buflen)
{
#if ETA > 7
#error "rej_eta() assumes ETA <= 7"
#endif
  unsigned int ctr, pos;
  unsigned char t0, t1;
  DBENCH_START();

  ctr = pos = 0;
  while(ctr < len && pos < buflen) {
#if ETA <= 3
    t0 = buf[pos] & 0x07;
    t1 = buf[pos++] >> 5;
#else
    t0 = buf[pos] & 0x0F;
    t1 = buf[pos++] >> 4;
#endif

    if(t0 <= 2*ETA)
      a[ctr++] = _Q + ETA - t0;
    if(t1 <= 2*ETA && ctr < len)
      a[ctr++] = _Q + ETA - t1;
  }

  DBENCH_STOP(*tsample);
  return ctr;
}

/*************************************************
* Name:        poly_uniform_eta
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-ETA,ETA] by performing rejection sampling using the
*              output stream from SHAKE256(seed|nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const unsigned char seed[]: byte array with seed of length
*                                            SEEDBYTES
*              - unsigned char nonce: nonce byte
**************************************************/
void poly_uniform_eta(poly *a,
                      const unsigned char seed[SEEDBYTES],
                      unsigned char nonce)
{
  unsigned int i, ctr;
  unsigned char inbuf[SEEDBYTES + 1];
  /* Probability that we need more than 2 blocks: < 2^{-84}
     Probability that we need more than 3 blocks: < 2^{-352} */
  unsigned char outbuf[2*SHAKE256_RATE];
  uint64_t state[25];

  for(i= 0; i < SEEDBYTES; ++i)
    inbuf[i] = seed[i];
  inbuf[SEEDBYTES] = nonce;

  shake256_absorb(state, inbuf, SEEDBYTES + 1);
  shake256_squeezeblocks(outbuf, 2, state);

  ctr = rej_eta(a->coeffs, _N, outbuf, 2*SHAKE256_RATE);
  if(ctr < _N) {
    shake256_squeezeblocks(outbuf, 1, state);
    rej_eta(a->coeffs + ctr, _N - ctr, outbuf, SHAKE256_RATE);
  }
}

void poly_uniform_eta_4x(poly *a0,
                         poly *a1,
                         poly *a2,
                         poly *a3,
                         const unsigned char seed[SEEDBYTES],
                         unsigned char nonce0,
                         unsigned char nonce1,
                         unsigned char nonce2,
                         unsigned char nonce3)
{
  unsigned int i;
  unsigned char inbuf[4][SEEDBYTES + 1];
  unsigned char outbuf[4][2*SHAKE256_RATE];
  __m256i state[25];

  for(i= 0; i < SEEDBYTES; ++i) {
    inbuf[0][i] = seed[i];
    inbuf[1][i] = seed[i];
    inbuf[2][i] = seed[i];
    inbuf[3][i] = seed[i];
  }
  inbuf[0][SEEDBYTES] = nonce0;
  inbuf[1][SEEDBYTES] = nonce1;
  inbuf[2][SEEDBYTES] = nonce2;
  inbuf[3][SEEDBYTES] = nonce3;

  shake256_absorb4x(state, inbuf[0], inbuf[1], inbuf[2], inbuf[3],
                    SEEDBYTES + 1);
  shake256_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 2,
                           state);

  if(rej_eta(a0->coeffs, _N, outbuf[0], 2*SHAKE256_RATE) < _N)
    poly_uniform_eta(a0, seed, nonce0);
  if(rej_eta(a1->coeffs, _N, outbuf[1], 2*SHAKE256_RATE) < _N)
    poly_uniform_eta(a1, seed, nonce1);
  if(rej_eta(a2->coeffs, _N, outbuf[2], 2*SHAKE256_RATE) < _N)
    poly_uniform_eta(a2, seed, nonce2);
  if(rej_eta(a3->coeffs, _N, outbuf[3], 2*SHAKE256_RATE) < _N)
    poly_uniform_eta(a3, seed, nonce3);
}

/*************************************************
* Name:        rej_gamma1m1
*
* Description: Sample uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1] by performing rejection sampling
*              using array of random bytes.
*
* Arguments:   - uint32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const unsigned char *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_gamma1m1(uint32_t *a,
                                 unsigned int len,
                                 const unsigned char *buf,
                                 unsigned int buflen)
{
#if GAMMA1 > (1 << 19)
#error "rej_gamma1m1() assumes GAMMA1 - 1 fits in 19 bits"
#endif
  unsigned int ctr, pos;
  uint32_t t0, t1;
  DBENCH_START();

  ctr = pos = 0;
  while(ctr < len && pos + 5 <= buflen) {
    t0  = buf[pos];
    t0 |= (uint32_t)buf[pos + 1] << 8;
    t0 |= (uint32_t)buf[pos + 2] << 16;
    t0 &= 0xFFFFF;

    t1  = buf[pos + 2] >> 4;
    t1 |= (uint32_t)buf[pos + 3] << 4;
    t1 |= (uint32_t)buf[pos + 4] << 12;

    pos += 5;

    if(t0 <= 2*GAMMA1 - 2)
      a[ctr++] = _Q + GAMMA1 - 1 - t0;
    if(t1 <= 2*GAMMA1 - 2 && ctr < len)
      a[ctr++] = _Q + GAMMA1 - 1 - t1;
  }

  DBENCH_STOP(*tsample);
  return ctr;
}

/*************************************************
* Name:        poly_uniform_gamma1m1
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1] by performing rejection
*              sampling on output stream of SHAKE256(seed|nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const unsigned char seed[]: byte array with seed of length
*                                            SEEDBYTES + CRHBYTES
*              - uint16_t nonce: 16-bit nonce
**************************************************/
void poly_uniform_gamma1m1(poly *a,
                           const unsigned char seed[SEEDBYTES + CRHBYTES],
                           uint16_t nonce)
{
  unsigned int i, ctr;
  unsigned char inbuf[SEEDBYTES + CRHBYTES + 2];
  /* Probability that we need more than 5 blocks: < 2^{-81}
     Probability that we need more than 6 blocks: < 2^{-467} */
  unsigned char outbuf[5*SHAKE256_RATE];
  uint64_t state[25];

  for(i = 0; i < SEEDBYTES + CRHBYTES; ++i)
    inbuf[i] = seed[i];
  inbuf[SEEDBYTES + CRHBYTES] = nonce & 0xFF;
  inbuf[SEEDBYTES + CRHBYTES + 1] = nonce >> 8;

  shake256_absorb(state, inbuf, SEEDBYTES + CRHBYTES + 2);
  shake256_squeezeblocks(outbuf, 5, state);

  ctr = rej_gamma1m1(a->coeffs, _N, outbuf, 5*SHAKE256_RATE);
  if(ctr < _N) {
    /* There are no bytes left in outbuf
       since 5*SHAKE256_RATE is divisible by 5 */
    shake256_squeezeblocks(outbuf, 1, state);
    rej_gamma1m1(a->coeffs + ctr, _N - ctr, outbuf, SHAKE256_RATE);
  }
}

void poly_uniform_gamma1m1_4x(poly *a0,
                              poly *a1,
                              poly *a2,
                              poly *a3,
                              const unsigned char seed[SEEDBYTES + CRHBYTES],
                              uint16_t nonce0,
                              uint16_t nonce1,
                              uint16_t nonce2,
                              uint16_t nonce3)
{
  unsigned int i;
  unsigned char inbuf[4][SEEDBYTES + CRHBYTES + 2];
  unsigned char outbuf[4][5*SHAKE256_RATE];
  __m256i state[25];

  for(i = 0; i < SEEDBYTES + CRHBYTES; ++i) {
    inbuf[0][i] = seed[i];
    inbuf[1][i] = seed[i];
    inbuf[2][i] = seed[i];
    inbuf[3][i] = seed[i];
  }
  inbuf[0][SEEDBYTES + CRHBYTES] = nonce0 & 0xFF;
  inbuf[0][SEEDBYTES + CRHBYTES + 1] = nonce0 >> 8;
  inbuf[1][SEEDBYTES + CRHBYTES] = nonce1 & 0xFF;
  inbuf[1][SEEDBYTES + CRHBYTES + 1] = nonce1 >> 8;
  inbuf[2][SEEDBYTES + CRHBYTES] = nonce2 & 0xFF;
  inbuf[2][SEEDBYTES + CRHBYTES + 1] = nonce2 >> 8;
  inbuf[3][SEEDBYTES + CRHBYTES] = nonce3 & 0xFF;
  inbuf[3][SEEDBYTES + CRHBYTES + 1] = nonce3 >> 8;

  shake256_absorb4x(state, inbuf[0], inbuf[1], inbuf[2], inbuf[3],
                    SEEDBYTES + CRHBYTES + 2);
  shake256_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5, state);

  if(rej_gamma1m1(a0->coeffs, _N, outbuf[0], 5*SHAKE256_RATE) < _N)
    poly_uniform_gamma1m1(a0, seed, nonce0);
  if(rej_gamma1m1(a1->coeffs, _N, outbuf[1], 5*SHAKE256_RATE) < _N)
    poly_uniform_gamma1m1(a1, seed, nonce1);
  if(rej_gamma1m1(a2->coeffs, _N, outbuf[2], 5*SHAKE256_RATE) < _N)
    poly_uniform_gamma1m1(a2, seed, nonce2);
  if(rej_gamma1m1(a3->coeffs, _N, outbuf[3], 5*SHAKE256_RATE) < _N)
    poly_uniform_gamma1m1(a3, seed, nonce3);
}

/*************************************************
* Name:        polyeta_pack
*
* Description: Bit-pack polynomial with coefficients in [-ETA,ETA].
*              Input coefficients are assumed to lie in [Q-ETA,Q+ETA].
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLETA_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyeta_pack(unsigned char *r, const poly *a) {
#if ETA > 7
#error "polyeta_pack() assumes ETA <= 7"
#endif
  unsigned int i;
  unsigned char t[8];
  DBENCH_START();

#if ETA <= 3
  for(i = 0; i < N/8; ++i) {
    t[0] = Q + ETA - a->coeffs[8*i+0];
    t[1] = Q + ETA - a->coeffs[8*i+1];
    t[2] = Q + ETA - a->coeffs[8*i+2];
    t[3] = Q + ETA - a->coeffs[8*i+3];
    t[4] = Q + ETA - a->coeffs[8*i+4];
    t[5] = Q + ETA - a->coeffs[8*i+5];
    t[6] = Q + ETA - a->coeffs[8*i+6];
    t[7] = Q + ETA - a->coeffs[8*i+7];

    r[3*i+0]  = t[0];
    r[3*i+0] |= t[1] << 3;
    r[3*i+0] |= t[2] << 6;
    r[3*i+1]  = t[2] >> 2;
    r[3*i+1] |= t[3] << 1;
    r[3*i+1] |= t[4] << 4;
    r[3*i+1] |= t[5] << 7;
    r[3*i+2]  = t[5] >> 1;
    r[3*i+2] |= t[6] << 2;
    r[3*i+2] |= t[7] << 5;
  }
#else
  for(i = 0; i < _N/2; ++i) {
    t[0] = _Q + ETA - a->coeffs[2*i+0];
    t[1] = _Q + ETA - a->coeffs[2*i+1];
    r[i] = t[0] | (t[1] << 4);
  }
#endif

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyeta_unpack
*
* Description: Unpack polynomial with coefficients in [-ETA,ETA].
*              Output coefficients lie in [Q-ETA,Q+ETA].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyeta_unpack(poly *r, const unsigned char *a) {
  unsigned int i;
  DBENCH_START();

#if ETA <= 3
  for(i = 0; i < N/8; ++i) {
    r->coeffs[8*i+0] = a[3*i+0] & 0x07;
    r->coeffs[8*i+1] = (a[3*i+0] >> 3) & 0x07;
    r->coeffs[8*i+2] = (a[3*i+0] >> 6) | ((a[3*i+1] & 0x01) << 2);
    r->coeffs[8*i+3] = (a[3*i+1] >> 1) & 0x07;
    r->coeffs[8*i+4] = (a[3*i+1] >> 4) & 0x07;
    r->coeffs[8*i+5] = (a[3*i+1] >> 7) | ((a[3*i+2] & 0x03) << 1);
    r->coeffs[8*i+6] = (a[3*i+2] >> 2) & 0x07;
    r->coeffs[8*i+7] = (a[3*i+2] >> 5);

    r->coeffs[8*i+0] = Q + ETA - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = Q + ETA - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = Q + ETA - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = Q + ETA - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = Q + ETA - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = Q + ETA - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = Q + ETA - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = Q + ETA - r->coeffs[8*i+7];
  }
#else
  for(i = 0; i < _N/2; ++i) {
    r->coeffs[2*i+0] = a[i] & 0x0F;
    r->coeffs[2*i+1] = a[i] >> 4;
    r->coeffs[2*i+0] = _Q + ETA - r->coeffs[2*i+0];
    r->coeffs[2*i+1] = _Q + ETA - r->coeffs[2*i+1];
  }
#endif

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt1_pack
*
* Description: Bit-pack polynomial t1 with coefficients fitting in 9 bits.
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLT1_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt1_pack(unsigned char *r, const poly *a) {
#if _D != 14
#error "polyt1_pack() assumes _D == 14"
#endif
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N/8; ++i) {
    r[9*i+0]  =  a->coeffs[8*i+0] & 0xFF;
    r[9*i+1]  = (a->coeffs[8*i+0] >> 8) | ((a->coeffs[8*i+1] & 0x7F) << 1);
    r[9*i+2]  = (a->coeffs[8*i+1] >> 7) | ((a->coeffs[8*i+2] & 0x3F) << 2);
    r[9*i+3]  = (a->coeffs[8*i+2] >> 6) | ((a->coeffs[8*i+3] & 0x1F) << 3);
    r[9*i+4]  = (a->coeffs[8*i+3] >> 5) | ((a->coeffs[8*i+4] & 0x0F) << 4);
    r[9*i+5]  = (a->coeffs[8*i+4] >> 4) | ((a->coeffs[8*i+5] & 0x07) << 5);
    r[9*i+6]  = (a->coeffs[8*i+5] >> 3) | ((a->coeffs[8*i+6] & 0x03) << 6);
    r[9*i+7]  = (a->coeffs[8*i+6] >> 2) | ((a->coeffs[8*i+7] & 0x01) << 7);
    r[9*i+8]  =  a->coeffs[8*i+7] >> 1;
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt1_unpack
*
* Description: Unpack polynomial t1 with 9-bit coefficients.
*              Output coefficients are standard representatives.
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyt1_unpack(poly *r, const unsigned char *a) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N/8; ++i) {
    r->coeffs[8*i+0] =  a[9*i+0]       | ((uint32_t)(a[9*i+1] & 0x01) << 8);
    r->coeffs[8*i+1] = (a[9*i+1] >> 1) | ((uint32_t)(a[9*i+2] & 0x03) << 7);
    r->coeffs[8*i+2] = (a[9*i+2] >> 2) | ((uint32_t)(a[9*i+3] & 0x07) << 6);
    r->coeffs[8*i+3] = (a[9*i+3] >> 3) | ((uint32_t)(a[9*i+4] & 0x0F) << 5);
    r->coeffs[8*i+4] = (a[9*i+4] >> 4) | ((uint32_t)(a[9*i+5] & 0x1F) << 4);
    r->coeffs[8*i+5] = (a[9*i+5] >> 5) | ((uint32_t)(a[9*i+6] & 0x3F) << 3);
    r->coeffs[8*i+6] = (a[9*i+6] >> 6) | ((uint32_t)(a[9*i+7] & 0x7F) << 2);
    r->coeffs[8*i+7] = (a[9*i+7] >> 7) | ((uint32_t)(a[9*i+8] & 0xFF) << 1);
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt0_pack
*
* Description: Bit-pack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*              Input coefficients are assumed to lie in ]Q-2^{D-1}, Q+2^{D-1}].
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLT0_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt0_pack(unsigned char *r, const poly *a) {
  unsigned int i;
  uint32_t t[4];
  DBENCH_START();

  for(i = 0; i < _N/4; ++i) {
    t[0] = _Q + (1 << (_D-1)) - a->coeffs[4*i+0];
    t[1] = _Q + (1 << (_D-1)) - a->coeffs[4*i+1];
    t[2] = _Q + (1 << (_D-1)) - a->coeffs[4*i+2];
    t[3] = _Q + (1 << (_D-1)) - a->coeffs[4*i+3];

    r[7*i+0]  =  t[0];
    r[7*i+1]  =  t[0] >> 8;
    r[7*i+1] |=  t[1] << 6;
    r[7*i+2]  =  t[1] >> 2;
    r[7*i+3]  =  t[1] >> 10;
    r[7*i+3] |=  t[2] << 4;
    r[7*i+4]  =  t[2] >> 4;
    r[7*i+5]  =  t[2] >> 12;
    r[7*i+5] |=  t[3] << 2;
    r[7*i+6]  =  t[3] >> 6;
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt0_unpack
*
* Description: Unpack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*              Output coefficients lie in ]Q-2^{D-1},Q+2^{D-1}].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyt0_unpack(poly *r, const unsigned char *a) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N/4; ++i) {
    r->coeffs[4*i+0]  = a[7*i+0];
    r->coeffs[4*i+0] |= (uint32_t)(a[7*i+1] & 0x3F) << 8;

    r->coeffs[4*i+1]  = a[7*i+1] >> 6;
    r->coeffs[4*i+1] |= (uint32_t)a[7*i+2] << 2;
    r->coeffs[4*i+1] |= (uint32_t)(a[7*i+3] & 0x0F) << 10;

    r->coeffs[4*i+2]  = a[7*i+3] >> 4;
    r->coeffs[4*i+2] |= (uint32_t)a[7*i+4] << 4;
    r->coeffs[4*i+2] |= (uint32_t)(a[7*i+5] & 0x03) << 12;

    r->coeffs[4*i+3]  = a[7*i+5] >> 2;
    r->coeffs[4*i+3] |= (uint32_t)a[7*i+6] << 6;

    r->coeffs[4*i+0] = _Q + (1 << (_D-1)) - r->coeffs[4*i+0];
    r->coeffs[4*i+1] = _Q + (1 << (_D-1)) - r->coeffs[4*i+1];
    r->coeffs[4*i+2] = _Q + (1 << (_D-1)) - r->coeffs[4*i+2];
    r->coeffs[4*i+3] = _Q + (1 << (_D-1)) - r->coeffs[4*i+3];
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyz_pack
*
* Description: Bit-pack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLZ_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyz_pack(unsigned char *r, const poly *a) {
#if GAMMA1 > (1 << 19)
#error "polyz_pack() assumes GAMMA1 <= 2^{19}"
#endif
  unsigned int i;
  uint32_t t[2];
  DBENCH_START();

  for(i = 0; i < _N/2; ++i) {
    /* Map to {0,...,2*GAMMA1 - 2} */
    t[0] = GAMMA1 - 1 - a->coeffs[2*i+0];
    t[0] += ((int32_t)t[0] >> 31) & _Q;
    t[1] = GAMMA1 - 1 - a->coeffs[2*i+1];
    t[1] += ((int32_t)t[1] >> 31) & _Q;

    r[5*i+0]  = t[0];
    r[5*i+1]  = t[0] >> 8;
    r[5*i+2]  = t[0] >> 16;
    r[5*i+2] |= t[1] << 4;
    r[5*i+3]  = t[1] >> 4;
    r[5*i+4]  = t[1] >> 12;
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyz_unpack
*
* Description: Unpack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1].
*              Output coefficients are standard representatives.
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const unsigned char *a: byte array with bit-packed polynomial
**************************************************/
void polyz_unpack(poly *r, const unsigned char *a) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N/2; ++i) {
    r->coeffs[2*i+0]  = a[5*i+0];
    r->coeffs[2*i+0] |= (uint32_t)a[5*i+1] << 8;
    r->coeffs[2*i+0] |= (uint32_t)(a[5*i+2] & 0x0F) << 16;

    r->coeffs[2*i+1]  = a[5*i+2] >> 4;
    r->coeffs[2*i+1] |= (uint32_t)a[5*i+3] << 4;
    r->coeffs[2*i+1] |= (uint32_t)a[5*i+4] << 12;

    r->coeffs[2*i+0] = GAMMA1 - 1 - r->coeffs[2*i+0];
    r->coeffs[2*i+0] += ((int32_t)r->coeffs[2*i+0] >> 31) & _Q;
    r->coeffs[2*i+1] = GAMMA1 - 1 - r->coeffs[2*i+1];
    r->coeffs[2*i+1] += ((int32_t)r->coeffs[2*i+1] >> 31) & _Q;
  }

  DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyw1_pack
*
* Description: Bit-pack polynomial w1 with coefficients in [0, 15].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - unsigned char *r: pointer to output byte array with at least
*                                  POLW1_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyw1_pack(unsigned char *r, const poly *a) {
  unsigned int i;
  DBENCH_START();

  for(i = 0; i < _N/2; ++i)
    r[i] = a->coeffs[2*i+0] | (a->coeffs[2*i+1] << 4);

  DBENCH_STOP(*tpack);
}
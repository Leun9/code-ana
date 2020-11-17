#include <stdio.h>
#include <gmp.h>

#define NSMALLPRIMES 170

typedef mpz_t BigInt;

char buf[2048];
long long buf_len;
void get_bigint() {
  buf_len = 0;
  char ch = getchar();
  while (ch < '0' || ch > '9') ch = getchar();
  while (ch >= '0' && ch <= '9') {buf[buf_len] = ch; ++buf_len; ch = getchar();}
  buf[buf_len] = 0;
}

int getint() {
  int x = 0;
  char ch = getchar();
  while (ch < '0' || ch > '9') ch = getchar();
  while (ch >= '0' && ch <= '9') {x = x*10 + (ch - '0'); ch = getchar();}
  return x;
}

int n;
BigInt p, q, p_1, q_1, N, phi_N;
BigInt e, d, s, t;
BigInt p2, q2;
BigInt K0, K1, K2;
BigInt prime[NSMALLPRIMES];
BigInt gcd_temp;
int reps_temp = 15;

void init() {
  mpz_init_set_ui(K0, 0);
  mpz_init_set_ui(K1, 1);
  mpz_init_set_ui(K2, 2);
  mpz_set(prime[0], K2);
  for (int i = 1; i < NSMALLPRIMES; ++i) {
    mpz_init(prime[i]);
    mpz_nextprime(prime[i], prime[i - 1]);
    //mpz_out_str(stdout, 10, prime[i]); putchar('\n');
  }
}

bool bu_ping_hua(BigInt &x) {
  mpz_t result; mpz_init(result);
  mpz_set(result, x);
  for (int i = 0; i < NSMALLPRIMES; ++i) {
    mpz_remove(result, result, prime[i]);
    //mpz_out_str(stdout, 10, result); putchar('\n');
  }
  if (mpz_cmp(result, K1) == 0) return true; 
  return false;
}

int main (){
  init();
  n = getint();
  for (int i = 0; i < n; ++i) {
    get_bigint(); mpz_init_set_str(e, buf, 10); long long len_e = buf_len;
    get_bigint(); mpz_init_set_str(p, buf, 10); long long len_p = buf_len;
    get_bigint(); mpz_init_set_str(q, buf, 10); long long len_q = buf_len;
    mpz_mul(p2, p, K2);
    mpz_mul(q2, q, K2);
    if (mpz_cmp(p2, q) > 0 && mpz_cmp(q2, p) > 0 && len_e < (len_p + len_q >> 2)) {printf("ERROR\n"); continue;}; //wiener attack

    mpz_sub(p_1, p, K1);
    mpz_sub(q_1, q, K1);
    mpz_mul(phi_N, p_1, q_1);
    //mpz_invert(d, e, phi_N); // BUG : 没逆，但不返回0
    mpz_gcdext(gcd_temp, d, t, e, phi_N);

    if (mpz_cmp(gcd_temp, K1) != 0) {printf("ERROR\n"); continue;};
    if (mpz_probab_prime_p(p, reps_temp) == 0) {printf("ERROR\n"); continue;};
    if (mpz_probab_prime_p(q, reps_temp) == 0) {printf("ERROR\n"); continue;};
    if (bu_ping_hua(p_1)) {printf("ERROR\n"); continue;};
    if (bu_ping_hua(q_1)) {printf("ERROR\n"); continue;};
    mpz_out_str(stdout, 10, d); putchar('\n');
  }
  return 0;
}
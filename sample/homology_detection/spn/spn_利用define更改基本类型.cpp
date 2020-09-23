#include <cstdio>

#define INTINT int
#define CHARCHAR char

INTINT n;
INTINT pi_s[] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
INTINT pi_p[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}; // symmetry !
INTINT pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
CHARCHAR buf[5];
// spn args
INTINT x, y, raw_key1, raw_key2, k1, k2, k3, k4, k5;
INTINT s[65536], ps[65536], s_r[65536], ps_r[65536];

INTINT hello; /***
Comment for lex testing

/ * /
* /*
***/

inline void preprocess() {
  for (INTINT i = 0; i < 65536; ++i) {
    s[i] = (pi_s[i>>12]<<12) | (pi_s[(i>>8)&0xf]<<8) | (pi_s[(i>>4)&0xf]<<4) | (pi_s[i&0xf]);
    s_r[s[i]] = i;
  }
  for (INTINT i = 0; i < 65536; ++i) {
    ps[i] = 0;
    for (INTINT j = 0; j < 16; ++j) { // FIXME
      if (pow2[j] & s[i]) ps[i] |= pow2[pi_p[j]];
    }
    ps_r[ps[i]] = i;
  }
}

inline INTINT get16bit() {
  for (INTINT i = 0; i < 4; ) {
    buf[i] = getchar();
    if (buf[i] >= '0' && buf[i] <= '9') {
      buf[i] -= '0';
      i++;
    } else if (buf[i] >= 'a' && buf[i] <= 'z') {
      buf[i] += 10 - 'a';
      i++;
    }
  }
  return (buf[0] << 12) | (buf[1] << 8) | (buf[2] << 4) | buf[3];
}

inline void put16bit(INTINT x) {
  for (INTINT i = 0; i < 4; ++i) {
    buf[3 - i] = x & 0xf;
    if (buf[3 - i] <= 9) buf[3 - i] += '0';
    else buf[3 - i] += 'a' - 10;
    x >>= 4;
  }
  putchar(buf[0]);
  putchar(buf[1]);
  putchar(buf[2]);
  putchar(buf[3]);
}

INTINT main () {
  preprocess();
  scanf("%d", &n);
  for (INTINT i = 0; i < n; i++) {
    raw_key1 = get16bit();
    raw_key2 = get16bit();
    x = get16bit();
    k5 = raw_key2;
    k4 = (raw_key2 >> 4) | ((raw_key1 & 0xf) << 12);
    k3 = (raw_key2 >> 8) | ((raw_key1 & 0xff) << 8);
    k2 = (raw_key2 >> 12) | ((raw_key1 & 0xfff) << 4);
    k1 = raw_key1;
    y = s[ps[ps[ps[x^k1]^k2]^k3]^k4];
    put16bit(y^k5);
    putchar(' ');
    put16bit(ps_r[ps_r[ps_r[s_r[y^1]^k4]^k3]^k2]^k1);
    putchar('\n');
  }
  return 0;
}
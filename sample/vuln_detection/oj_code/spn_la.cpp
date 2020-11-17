#include <cstdio>
#include <cstring>

#define GET4BIT(x) {x = getchar(); if (x >= '0' && x <= '9') x -= '0'; else x += 10 - 'a';}
#define PUT4BIT(x) {if (x >= 0 && x <= 9) putchar(x + '0'); else putchar(x - 10 + 'a');}
#define SHR(x, k) (x >> k)
#define BIT(x, k) ((x >> k) & 1)

#define NPAIR1 7500
#define NPAIR2 5000
#define TOPNUM2 2
#define MAGIC 2

// spn
int pi_s[] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
int pi_p[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}; // symmetry !
int pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
// spn args
int k1, k2, k3, k4, k5;
int s[65536], ps[65536];
char buf[5];

inline void preprocess() {
  for (int i = 0; i < 65536; ++i) {
    s[i] = (pi_s[i>>12]<<12) | (pi_s[(i>>8)&0xf]<<8) | (pi_s[(i>>4)&0xf]<<4) | (pi_s[i&0xf]);
  }
  for (int i = 0; i < 65536; ++i) {
    ps[i] = 0;
    for (int j = 0; j < 16; ++j) { // FIXME
      if (pow2[j] & s[i]) ps[i] |= pow2[pi_p[j]];
    }
  }
}

void put16bit(int x) {
  for (int i = 0; i < 4; ++i) {
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

// linear analy
int n;
int s_r[] = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};
char x[8000][4], y[8000][4];
char key1, key2, key3, key4;
unsigned short cnr[16][16];
unsigned short cnt[16][16];
unsigned short cns[16][16];

void get_x_and_y() {
  for (int i = 0; i < 8000; ++i) {
    GET4BIT(x[i][0]);
    GET4BIT(x[i][1]);
    GET4BIT(x[i][2]);
    GET4BIT(x[i][3]);
    getchar();
    GET4BIT(y[i][0]);
    GET4BIT(y[i][1]);
    GET4BIT(y[i][2]);
    GET4BIT(y[i][3]);
    getchar();
  }
}

int main() {
  preprocess();
  scanf("%d", &n);
  while (getchar() != '\n') ;
  for (int i = 0; i < n; ++i) {
    get_x_and_y();
    static int x_[MAGIC]; for (int i = 0; i < MAGIC; ++i) x_[i] = (x[i][0] << 12) | (x[i][1] << 8) | (x[i][2] << 4) | x[i][3];
    static int y_[MAGIC]; for (int i = 0; i < MAGIC; ++i) y_[i] = (y[i][0] << 12) | (y[i][1] << 8) | (y[i][2] << 4) | y[i][3];

    // 8 bit
    memset(cnr, 0, 256 * sizeof(unsigned short));
    for (int j = 0; j < NPAIR1; ++j) 
      for (int k2 = 0; k2 < 16; ++k2) {
          char x2 = x[j][1], x21 = SHR(x2,1), x23 = SHR(x2,3);
          char u2 = s_r[y[j][1]^k2], u22 = SHR(u2,2);
        for (int k4 = 0; k4 < 16; ++k4) {
          char u4 = s_r[y[j][3]^k4];
          if ((x2^x21^x23^u2^u22^u4^SHR(u4,2))&1) cnr[k2][k4] += 1;
        }
      }
    for (int k2 = 0; k2 < 16; ++k2)
      for (int k4 = 0; k4 < 16; ++k4) {
        if (cnr[k2][k4] > (NPAIR1 >> 1)) cnr[k2][k4] -= (NPAIR1 >> 1);
        else cnr[k2][k4] = (NPAIR1 >> 1) - cnr[k2][k4];
      }
    
    for (;;) {
      // other 8 bit
      int max = 0;
      for (int i = 0; i < 16; ++i) 
        for (int j = 0; j < 16; ++j)
          if (cnr[i][j] > max) {
            max = cnr[i][j];
            key2 = i;
            key4 = j;
          }
      cnr[key2][key4] = 0;
      memset(cnt, 0, 256 * sizeof(unsigned short));
      memset(cns, 0, 256 * sizeof(unsigned short));
      for (int j = 0; j < NPAIR2; ++j) {
            char x1 = x[j][0], v1 = x1^SHR(x1,2)^SHR(x1,3);
            char x3 = x[j][2], v3 = x3^SHR(x3,2)^SHR(x3,3);
            char u2 = s_r[y[j][1] ^ key2], v21 = SHR(u2,1), v23 = SHR(u2,3);
            char u4 = s_r[y[j][3] ^ key4], v41 = SHR(u4,1), v43 = SHR(u4,3);
        for (int k1 = 0; k1 < 16; ++k1) {
            char u1 = s_r[y[j][0] ^ k1], u13 = SHR(u1,3), u11 = SHR(u1,1);
          for (int k3 = 0; k3 < 16; ++k3) {
            char u3 = s_r[y[j][2] ^ k3];
            if ((v1^u13^v23^SHR(u3,3)^v43) & 1) cnt[k1][k3] += 1;
            if ((v3^u11^v21^SHR(u3,1)^v41) & 1) cns[k1][k3] += 1;
          }
        }
      }
      for (int k1 = 0; k1 < 16; ++k1)
        for (int k3 = 0; k3 < 16; ++k3) {
          int cnt_ = cnt[k1][k3];
          int cns_ = cns[k1][k3];
          if (cnt_ > (NPAIR2 >> 1)) cnt_ -= (NPAIR2 >> 1); else cnt_ = (NPAIR2 >> 1) - cnt_;
          if (cns_ > (NPAIR2 >> 1)) cns_ -= (NPAIR2 >> 1); else cns_ = (NPAIR2 >> 1) - cns_;
          cnt[k1][k3] = cnt_ + cns_;
        }

        // other 16 bit
        for (int cnt16bit = 0; cnt16bit < TOPNUM2; cnt16bit++) {
          int max = 0;
          for (int i = 0; i < 16; ++i) 
            for (int j = 0; j < 16; ++j)
              if (cnt[i][j] > max) {
                max = cnt[i][j];
                key1 = i;
                key3 = j;
              }
          cnt[key1][key3] = 0;
          k5 = (key1 << 12) | (key2 << 8) | (key3 << 4) | (key4);
          for (k1 = 0; k1 < 65536; ++k1) {
            k4 = (k5 >> 4) | ((k1 & 0xf) << 12);
            k3 = (k5 >> 8) | ((k1 & 0xff) << 8);
            k2 = (k5 >> 12) | ((k1 & 0xfff) << 4);
            int i = 0;
            for (; i < MAGIC; ++i) 
              if ((s[ps[ps[ps[x_[i]^k1]^k2]^k3]^k4]^k5) != y_[i]) break;
            if (i == MAGIC) goto Found;
          }
        }
    }
Found:
    put16bit(k1);
    put16bit(k5);
    putchar('\n');
  }
  return 0;
}
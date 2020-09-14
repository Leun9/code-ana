#include <cstdio>

int n;
int llIlllIl[] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
int llIlIIIl[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
int IIIIIIIl[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
char IIIllIIl[5];
// spn args
int x, y, raw_key1, raw_key2, k1, k2, k3, k4, k5;
int lIIlllII[65536], IIIllIII[65536], IlIlllII[65536], IllllIII[65536];

int hello; /***
Comment for lex testing

/ * /
* /*
***/

inline void O00OO0O(int x) {
  for (int i = 0; i < 4; ++i) {
    IIIllIIl[3 - i] = x & 0xf;
    if (IIIllIIl[3 - i] <= 9) IIIllIIl[3 - i] += '0';
    else IIIllIIl[3 - i] += 'a' - 10;
    x >>= 4;
  }
  putchar(IIIllIIl[0]);
  putchar(IIIllIIl[1]);
  putchar(IIIllIIl[2]);
  putchar(IIIllIIl[3]);
}

inline int O00O00O() {
  for (int i = 0; i < 4; ) {
    IIIllIIl[i] = getchar();
    if (IIIllIIl[i] >= '0' && IIIllIIl[i] <= '9') {
      IIIllIIl[i] -= '0';
      i++;
    } else if (IIIllIIl[i] >= 'a' && IIIllIIl[i] <= 'z') {
      IIIllIIl[i] += 10 - 'a';
      i++;
    }
  }
  return (IIIllIIl[0] << 12) | (IIIllIIl[1] << 8) | (IIIllIIl[2] << 4) | IIIllIIl[3];
}


inline void O00O0O0() {
  for (int i = 0; i < 65536; ++i) {
    lIIlllII[i] = (llIlllIl[i>>12]<<12) | (llIlllIl[(i>>8)&0xf]<<8) | (llIlllIl[(i>>4)&0xf]<<4) | (llIlllIl[i&0xf]);
    IlIlllII[lIIlllII[i]] = i;
  }
  for (int i = 0; i < 65536; ++i) {
    IIIllIII[i] = 0;
    for (int j = 0; j < 16; ++j) {
      if (IIIIIIIl[j] & lIIlllII[i]) IIIllIII[i] |= IIIIIIIl[llIlIIIl[j]];
    }
    IllllIII[IIIllIII[i]] = i;
  }
}

int main () {
  O00O0O0();
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    raw_key1 = O00O00O();
    raw_key2 = O00O00O();
    x = O00O00O();
    k5 = raw_key2;
    k4 = (raw_key2 >> 4) | ((raw_key1 & 0xf) << 12);
    k3 = (raw_key2 >> 8) | ((raw_key1 & 0xff) << 8);
    k2 = (raw_key2 >> 12) | ((raw_key1 & 0xfff) << 4);
    k1 = raw_key1;
    y = lIIlllII[IIIllIII[IIIllIII[IIIllIII[x^k1]^k2]^k3]^k4];
    O00OO0O(y^k5);
    putchar(' ');
    O00OO0O(IllllIII[IllllIII[IllllIII[IlIlllII[y^1]^k4]^k3]^k2]^k1);
    putchar('\n');
  }
  return 0;
}
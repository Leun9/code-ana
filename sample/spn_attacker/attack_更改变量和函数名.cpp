#include <cstdio>
#include <cstdlib>
#include <cstring>

#define AAA 439
#define BBB 396
#define CCC 777

#define CNT2MAX 16

namespace TEST {
/***
 * 1. 更改部分变量名为棍棍（由I、1、l组成）
 * 2. 更改部分函数名为圈圈（由o、O、0组成）
 ***/
}

int n;
int IIIIIIIl[] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
int IIIIlllI[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}; // symmetry !
int IIllIIll[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
// spn args
int IIllIIlI[250], llllIIlI[250], llIlIIlI[250], lIIlIIlI[250];
int u1, u2, u3, u4;
unsigned l;
int maxkey24, maxkey13, max;
unsigned key0, key;
unsigned raw_key;
int k1, k2, k3, k4, k5;
int IIIIIIII[65536];
int llllllll[65536];
int llIlllll[65536], lllIllll[65536], lllIllIl[65536], lllIlllI[65536];
int IIlIlllI[65536];

inline void oooooooo() {
  for (int i = 0; i < 65536; ++i) {
    llIlllll[i] = (IIIIIIIl[i>>12]<<12) | (IIIIIIIl[(i>>8)&0xf]<<8) | (IIIIIIIl[(i>>4)&0xf]<<4) | (IIIIIIIl[i&0xf]);
    lllIllIl[llIlllll[i]] = i;
    lllIllll[i] = 0;
    for (int j = 0; j < 16; ++j) { // FIXME
      if (IIllIIll[j] & i) lllIllll[i] |= IIllIIll[IIIIlllI[j]];
    }
    lllIlllI[lllIllll[i]] = i;
  }
}
inline void OOOOOOOO() {
  int k=0;
  while (k < 80) {
    IIllIIlI[k] = rand()%65536;
    llIlIIlI[k] = IIllIIlI[k]^0x0b00;
    llllIIlI[k] = IIlIlllI[IIllIIlI[k]];
    lIIlIIlI[k] = IIlIlllI[llIlIIlI[k]];
    if(!((llllIIlI[k]^lIIlIIlI[k])&0xf0f0)) ++k;
  }
}
inline void O0O0O0O0() {
  int k=0;
  while (k < 240) {
    IIllIIlI[k] = rand()%65536;
    llIlIIlI[k] = IIllIIlI[k]^0x0f00;
    llllIIlI[k] = IIlIlllI[IIllIIlI[k]];
    lIIlIIlI[k] = IIlIlllI[llIlIIlI[k]];
    if(!((llllIIlI[k]^lIIlIIlI[k])&0x0f00)) ++k;
  }
}
char buf[16];
inline int get16bit() {
  for (int i = 0; i < 4; ) {
    buf[i] = getchar();
    if (buf[i] >= '0' && buf[i] <= '9') {
      buf[i] -= '0';
      i++;
    } else if (buf[i] >= 'a' && buf[i] <= 'z') {
      buf[i] += 10 - 'a';
      i++;
    }
  }
  getchar();
  return (buf[0] << 12) | (buf[1] << 8) | (buf[2] << 4) | buf[3];
}


int main()
{
  oooooooo();
  int i,j,k;
  scanf("%d",&n);
  getchar();
  for (k = 0; k < n; ++k)
  {
  for (i = 0; i < 65536; ++i) IIlIlllI[i] = get16bit();
  int find = 0;

  // first 8 bit
  OOOOOOOO();
  memset(cnt, 0, 256 * sizeof(int));
  for (i = 0; i < 80; ++i) {
    for (l = 0; l <= 0xff; ++l) {
      u2 = lllIllIl[(l>>4)^((llllIIlI[i]&0x0f00)>>8)] ^ lllIllIl[(l>>4)^((lIIlIIlI[i]&0x0f00)>>8)];
      u4 = lllIllIl[(l&0x0f)^(llllIIlI[i]&0x000f)] ^ lllIllIl[(l&0x0f)^(lIIlIIlI[i]&0x000f)];
      if (!(u2^6) && !(u4^6)) ++IIIIIIII[l];
  }
  }

    // second 8 bit
    for (; ;)
    {
    int max = -1;
    for(l = 0; l <= 0xff; ++l) {
      if(IIIIIIII[l] > max) {
        max = IIIIIIII[l];
        maxkey24 = l;
    }
    }
    IIIIIIII[maxkey24] = 0;
    O0O0O0O0();
    memset(cns, 0, 256 * sizeof(int));
    for (i = 0; i < 240; ++i) {
      for (l = 0; l <= 0xff; ++l) {
        u1 = lllIllIl[(l>>4)^((llllIIlI[i]&0xf000)>>12)] ^ lllIllIl[(l>>4)^((lIIlIIlI[i]&0xf000)>>12)];
        u3 = lllIllIl[(l&0x0f)^((llllIIlI[i]&0x00f0)>>4)] ^ lllIllIl[(l&0x0f)^((lIIlIIlI[i]&0x00f0)>>4)];
        u4 = lllIllIl[(maxkey24&0x0f)^(llllIIlI[i]&0x000f)] ^ lllIllIl[(maxkey24&0x0f)^(lIIlIIlI[i]&0x000f)];
        if (!(u1^6) & !(u3^6) && !(u4^6)) ++llllllll[l];
    }
    }

    // other 16 bit
    for (int cnt2 = 0; cnt2 < CNT2MAX; ++cnt2)
    {
      max = -1;
      for(l = 0; l <= 0xff; ++l) {
        if(llllllll[l] > max) {
          max = llllllll[l];
          maxkey13 = l;
      }
      }
      llllllll[maxkey13] = 0;
      for (key0 = 0; key0 <= 0xffff; ++key0) {
        key = (key0<<16) | ((maxkey13&0xf0)<<8) | ((maxkey24<<4)&0x0f00) | ((maxkey13&0xf)<<4) | (maxkey24&0xf);
        k5 = key & 0xffff;
        k4 = (key>>4) & 0xffff;
        k3 = (key>>8) & 0xffff;
        k2 = (key>>12) & 0xffff;
        k1 = (key>>16) & 0xffff;
        if((llIlllll[lllIllll[llIlllll[lllIllll[llIlllll[lllIllll[llIlllll[AAA^k1]]^k2]]^k3]]^k4] ^ k5) == IIlIlllI[AAA])
          if((llIlllll[lllIllll[llIlllll[lllIllll[llIlllll[lllIllll[llIlllll[BBB^k1]]^k2]]^k3]]^k4] ^ k5) == IIlIlllI[BBB])
            if((llIlllll[lllIllll[llIlllll[lllIllll[llIlllll[lllIllll[llIlllll[CCC^k1]]^k2]]^k3]]^k4] ^ k5) == IIlIlllI[CCC])
              {find = 1; break;}
      }
      if (find) break;
    }
    if (find) break;
    }
  // put key
  for (int i = 7; i >= 0; --i) {
    buf[i] = key & 0xf;
    if (buf[i] < 10) buf[i] += '0';
    else buf[i] += 'a' - 10;
    key >>= 4;
  }
  fwrite(buf, 1, 8, stdout);
  putchar('\n');
  }

  return 0;
}

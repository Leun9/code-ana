#include <cstdio>
#include <cstdlib>
#include <cstring>

#define AAA 439
#define BBB 396
#define CCC 777

#define CNT2MAX 16

namespace TEST {
/***
 * TEST /*
 * TEST /* 
 ***/
}

int IIIlllll;
int lllIllll[] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
int IllIllll[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}; // symmetry !
int lIlIllll[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
// spn args
int IIlIllll[250], llIIllll[250], IlIIllll[250], lIIIllll[250];
int IIIIllll, llllIlll, IlllIlll, lIllIlll;
unsigned l;
int IIllIlll, llIlIlll, IlIlIlll;
unsigned key0, key;
unsigned raw_key;
int lIIlIlll, IIIlIlll, lllIIlll, IllIIlll, lIlIIlll;
int IIlIIlll[65536];
int llIIIlll[65536];
int IlIIIlll[65536], lIIIIlll[65536], IIIIIlll[65536], lllllIll[65536];
int IllllIll[65536];

inline void lllllllI() {
  for (int IIlllIll = 0; IIlllIll < 65536; ++IIlllIll) {
    IlIIIlll[IIlllIll] = (lllIllll[IIlllIll>>12]<<12) | (lllIllll[(IIlllIll>>8)&0xf]<<8) | (lllIllll[(IIlllIll>>4)&0xf]<<4) | (lllIllll[IIlllIll&0xf]);
    IIIIIlll[IlIIIlll[IIlllIll]] = IIlllIll;
    lIIIIlll[IIlllIll] = 0;
    for (int llIllIll = 0; llIllIll < 16; ++llIllIll) { // FIXME
      if (lIlIllll[llIllIll] & IIlllIll) lIIIIlll[IIlllIll] |= lIlIllll[IllIllll[llIllIll]];
    }
    lllllIll[lIIIIlll[IIlllIll]] = IIlllIll;
  }
}
inline void IllllllI() {
  int IlIllIll=0;
  while (IlIllIll < 80) {
    IIlIllll[IlIllIll] = rand()%65536;
    IlIIllll[IlIllIll] = IIlIllll[IlIllIll]^0x0b00;
    llIIllll[IlIllIll] = IllllIll[IIlIllll[IlIllIll]];
    lIIIllll[IlIllIll] = IllllIll[IlIIllll[IlIllIll]];
    if(!((llIIllll[IlIllIll]^lIIIllll[IlIllIll])&0xf0f0)) ++IlIllIll;
  }
}
inline void lIlllllI() {
  int IlIllIll=0;
  while (IlIllIll < 240) {
    IIlIllll[IlIllIll] = rand()%65536;
    IlIIllll[IlIllIll] = IIlIllll[IlIllIll]^0x0f00;
    llIIllll[IlIllIll] = IllllIll[IIlIllll[IlIllIll]];
    lIIIllll[IlIllIll] = IllllIll[IlIIllll[IlIllIll]];
    if(!((llIIllll[IlIllIll]^lIIIllll[IlIllIll])&0x0f00)) ++IlIllIll;
  }
}
char lIlllIll[16];
inline int IIlllllI() {
  for (int IIlllIll = 0; IIlllIll < 4; ) {
    lIlllIll[IIlllIll] = getchar();
    if (lIlllIll[IIlllIll] >= '0' && lIlllIll[IIlllIll] <= '9') {
      lIlllIll[IIlllIll] -= '0';
      IIlllIll++;
    } else if (lIlllIll[IIlllIll] >= 'a' && lIlllIll[IIlllIll] <= 'z') {
      lIlllIll[IIlllIll] += 10 - 'a';
      IIlllIll++;
    }
  }
  getchar();
  return (lIlllIll[0] << 12) | (lIlllIll[1] << 8) | (lIlllIll[2] << 4) | lIlllIll[3];
}


int main()
{
  lllllllI();
  int IIlllIll,llIllIll,IlIllIll;
  scanf("%d",&IIIlllll);
  getchar();
  for (IlIllIll = 0; IlIllIll < IIIlllll; ++IlIllIll)
  {
  for (IIlllIll = 0; IIlllIll < 65536; ++IIlllIll) IllllIll[IIlllIll] = IIlllllI();
  int lIIllIll = 0;

  // first 8 bit
  IllllllI();
  memset(IIlIIlll, 0, 256 * sizeof(int));
  for (IIlllIll = 0; IIlllIll < 80; ++IIlllIll) {
    for (l = 0; l <= 0xff; ++l) {
      llllIlll = IIIIIlll[(l>>4)^((llIIllll[IIlllIll]&0x0f00)>>8)] ^ IIIIIlll[(l>>4)^((lIIIllll[IIlllIll]&0x0f00)>>8)];
      lIllIlll = IIIIIlll[(l&0x0f)^(llIIllll[IIlllIll]&0x000f)] ^ IIIIIlll[(l&0x0f)^(lIIIllll[IIlllIll]&0x000f)];
      if (!(llllIlll^6) && !(lIllIlll^6)) ++IIlIIlll[l];
  }
  }

    // second 8 bit
    for (; ;)
    {
    int IlIlIlll = -1;
    for(l = 0; l <= 0xff; ++l) {
      if(IIlIIlll[l] > IlIlIlll) {
        IlIlIlll = IIlIIlll[l];
        IIllIlll = l;
    }
    }
    IIlIIlll[IIllIlll] = 0;
    lIlllllI();
    memset(llIIIlll, 0, 256 * sizeof(int));
    for (IIlllIll = 0; IIlllIll < 240; ++IIlllIll) {
      for (l = 0; l <= 0xff; ++l) {
        IIIIllll = IIIIIlll[(l>>4)^((llIIllll[IIlllIll]&0xf000)>>12)] ^ IIIIIlll[(l>>4)^((lIIIllll[IIlllIll]&0xf000)>>12)];
        IlllIlll = IIIIIlll[(l&0x0f)^((llIIllll[IIlllIll]&0x00f0)>>4)] ^ IIIIIlll[(l&0x0f)^((lIIIllll[IIlllIll]&0x00f0)>>4)];
        lIllIlll = IIIIIlll[(IIllIlll&0x0f)^(llIIllll[IIlllIll]&0x000f)] ^ IIIIIlll[(IIllIlll&0x0f)^(lIIIllll[IIlllIll]&0x000f)];
        if (!(IIIIllll^6) & !(IlllIlll^6) && !(lIllIlll^6)) ++llIIIlll[l];
    }
    }

    // other 16 bit
    for (int lllIlIll = 0; lllIlIll < CNT2MAX; ++lllIlIll)
    {
      IlIlIlll = -1;
      for(l = 0; l <= 0xff; ++l) {
        if(llIIIlll[l] > IlIlIlll) {
          IlIlIlll = llIIIlll[l];
          llIlIlll = l;
      }
      }
      llIIIlll[llIlIlll] = 0;
      for (key0 = 0; key0 <= 0xffff; ++key0) {
        key = (key0<<16) | ((llIlIlll&0xf0)<<8) | ((IIllIlll<<4)&0x0f00) | ((llIlIlll&0xf)<<4) | (IIllIlll&0xf);
        lIlIIlll = key & 0xffff;
        IllIIlll = (key>>4) & 0xffff;
        lllIIlll = (key>>8) & 0xffff;
        IIIlIlll = (key>>12) & 0xffff;
        lIIlIlll = (key>>16) & 0xffff;
        if((IlIIIlll[lIIIIlll[IlIIIlll[lIIIIlll[IlIIIlll[lIIIIlll[IlIIIlll[AAA^lIIlIlll]]^IIIlIlll]]^lllIIlll]]^IllIIlll] ^ lIlIIlll) == IllllIll[AAA])
          if((IlIIIlll[lIIIIlll[IlIIIlll[lIIIIlll[IlIIIlll[lIIIIlll[IlIIIlll[BBB^lIIlIlll]]^IIIlIlll]]^lllIIlll]]^IllIIlll] ^ lIlIIlll) == IllllIll[BBB])
            if((IlIIIlll[lIIIIlll[IlIIIlll[lIIIIlll[IlIIIlll[lIIIIlll[IlIIIlll[CCC^lIIlIlll]]^IIIlIlll]]^lllIIlll]]^IllIIlll] ^ lIlIIlll) == IllllIll[CCC])
              {lIIllIll = 1; break;}
      }
      if (lIIllIll) break;
    }
    if (lIIllIll) break;
    }
  // put key
  for (int IIlllIll = 7; IIlllIll >= 0; --IIlllIll) {
    lIlllIll[IIlllIll] = key & 0xf;
    if (lIlllIll[IIlllIll] < 10) lIlllIll[IIlllIll] += '0';
    else lIlllIll[IIlllIll] += 'a' - 10;
    key >>= 4;
  }
  fwrite(lIlllIll, 1, 8, stdout);
  putchar('\n');
  }

  return 0;
}

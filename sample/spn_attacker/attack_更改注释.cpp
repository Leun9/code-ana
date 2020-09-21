#include <cstdio>
#include <cstdlib>
#include <cstring>
 // TEST
#define AAA 439
#define BBB 396
#define CCC 777
 // TEST
#define CNT2MAX 16
 // TEST
namespace TEST { // TEST
/*** // TEST
 *  // TEST
 *  // TEST
 ***/
} // TEST
 // TEST
int n; // TEST
int pi_s[] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};  // TEST
int pi_p[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}; // symmetry ! // TEST
int pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536}; // TEST
// spn args // TEST
int x[250], y[250], x1[250], y1_[250]; // TEST
int u1, u2, u3, u4; // TEST
unsigned l; // TEST
int maxkey24, maxkey13, max; // TEST
unsigned key0, key; // TEST
unsigned raw_key; // TEST
int k1, k2, k3, k4, k5; // TEST
int cnt[65536]; // TEST
int cns[65536]; // TEST
int s[65536], p[65536], r[65536], q[65536]; // TEST
int cipher[65536]; // TEST
 // TEST
inline void preprocess() { // TEST
  for (int i = 0; i < 65536; ++i) { // TEST
    s[i] = (pi_s[i>>12]<<12) | (pi_s[(i>>8)&0xf]<<8) | (pi_s[(i>>4)&0xf]<<4) | (pi_s[i&0xf]); // TEST
    r[s[i]] = i; // TEST
    p[i] = 0; // TEST
    for (int j = 0; j < 16; ++j) { // FIXME // TEST
      if (pow2[j] & i) p[i] |= pow2[pi_p[j]]; // TEST
    } // TEST
    q[p[i]] = i; // TEST
  } // TEST
} // TEST
inline void preprocess24() { // TEST
  int k=0; // TEST
  while (k < 80) { // TEST
    x[k] = rand()%65536; // TEST
    x1[k] = x[k]^0x0b00; // TEST
    y[k] = cipher[x[k]]; // TEST
    y1_[k] = cipher[x1[k]]; // TEST
    if(!((y[k]^y1_[k])&0xf0f0)) ++k; // TEST
  } // TEST
} // TEST
inline void preprocess134() { // TEST
  int k=0; // TEST
  while (k < 240) { // TEST
    x[k] = rand()%65536; // TEST
    x1[k] = x[k]^0x0f00; // TEST
    y[k] = cipher[x[k]]; // TEST
    y1_[k] = cipher[x1[k]]; // TEST
    if(!((y[k]^y1_[k])&0x0f00)) ++k; // TEST
  } // TEST
} // TEST
char buf[16]; // TEST
inline int get16bit() { // TEST
  for (int i = 0; i < 4; ) { // TEST
    buf[i] = getchar(); // TEST
    if (buf[i] >= '0' && buf[i] <= '9') { // TEST
      buf[i] -= '0'; // TEST
      i++; // TEST
    } else if (buf[i] >= 'a' && buf[i] <= 'z') { // TEST
      buf[i] += 10 - 'a'; // TEST
      i++; // TEST
    } // TEST
  } // TEST
  getchar(); // TEST
  return (buf[0] << 12) | (buf[1] << 8) | (buf[2] << 4) | buf[3]; // TEST
} // TEST
 // TEST
 // TEST
int main() // TEST
{ // TEST
  preprocess(); // TEST
  int i,j,k; // TEST
  scanf("%d",&n); // TEST
  getchar(); // TEST
  for (k = 0; k < n; ++k) // TEST
  { // TEST
  for (i = 0; i < 65536; ++i) cipher[i] = get16bit(); // TEST
  int find = 0; // TEST
 // TEST
  // first 8 bit // TEST
  preprocess24(); // TEST
  memset(cnt, 0, 256 * sizeof(int)); // TEST
  for (i = 0; i < 80; ++i) { // TEST
    for (l = 0; l <= 0xff; ++l) { // TEST
      u2 = r[(l>>4)^((y[i]&0x0f00)>>8)] ^ r[(l>>4)^((y1_[i]&0x0f00)>>8)]; // TEST
      u4 = r[(l&0x0f)^(y[i]&0x000f)] ^ r[(l&0x0f)^(y1_[i]&0x000f)]; // TEST
      if (!(u2^6) && !(u4^6)) ++cnt[l]; // TEST
  } // TEST
  } // TEST
 // TEST
    // second 8 bit // TEST
    for (; ;) // TEST
    { // TEST
    int max = -1; // TEST
    for(l = 0; l <= 0xff; ++l) { // TEST
      if(cnt[l] > max) { // TEST
        max = cnt[l]; // TEST
        maxkey24 = l; // TEST
    } // TEST
    } // TEST
    cnt[maxkey24] = 0; // TEST
    preprocess134(); // TEST
    memset(cns, 0, 256 * sizeof(int)); // TEST
    for (i = 0; i < 240; ++i) { // TEST
      for (l = 0; l <= 0xff; ++l) { // TEST
        u1 = r[(l>>4)^((y[i]&0xf000)>>12)] ^ r[(l>>4)^((y1_[i]&0xf000)>>12)]; // TEST
        u3 = r[(l&0x0f)^((y[i]&0x00f0)>>4)] ^ r[(l&0x0f)^((y1_[i]&0x00f0)>>4)]; // TEST
        u4 = r[(maxkey24&0x0f)^(y[i]&0x000f)] ^ r[(maxkey24&0x0f)^(y1_[i]&0x000f)]; // TEST
        if (!(u1^6) & !(u3^6) && !(u4^6)) ++cns[l]; // TEST
    } // TEST
    } // TEST
 // TEST
    // other 16 bit // TEST
    for (int cnt2 = 0; cnt2 < CNT2MAX; ++cnt2) // TEST
    { // TEST
      max = -1; // TEST
      for(l = 0; l <= 0xff; ++l) { // TEST
        if(cns[l] > max) { // TEST
          max = cns[l]; // TEST
          maxkey13 = l; // TEST
      } // TEST
      } // TEST
      cns[maxkey13] = 0; // TEST
      for (key0 = 0; key0 <= 0xffff; ++key0) { // TEST
        key = (key0<<16) | ((maxkey13&0xf0)<<8) | ((maxkey24<<4)&0x0f00) | ((maxkey13&0xf)<<4) | (maxkey24&0xf); // TEST
        k5 = key & 0xffff; // TEST
        k4 = (key>>4) & 0xffff; // TEST
        k3 = (key>>8) & 0xffff; // TEST
        k2 = (key>>12) & 0xffff; // TEST
        k1 = (key>>16) & 0xffff; // TEST
        if((s[p[s[p[s[p[s[AAA^k1]]^k2]]^k3]]^k4] ^ k5) == cipher[AAA]) // TEST
          if((s[p[s[p[s[p[s[BBB^k1]]^k2]]^k3]]^k4] ^ k5) == cipher[BBB]) // TEST
            if((s[p[s[p[s[p[s[CCC^k1]]^k2]]^k3]]^k4] ^ k5) == cipher[CCC]) // TEST
              {find = 1; break;} // TEST
      } // TEST
      if (find) break; // TEST
    } // TEST
    if (find) break; // TEST
    } // TEST
  // put key // TEST
  for (int i = 7; i >= 0; --i) { // TEST
    buf[i] = key & 0xf; // TEST
    if (buf[i] < 10) buf[i] += '0'; // TEST
    else buf[i] += 'a' - 10; // TEST
    key >>= 4; // TEST
  } // TEST
  fwrite(buf, 1, 8, stdout); // TEST
  putchar('\n'); // TEST
  } // TEST
 // TEST
  return 0; // TEST
} // TEST

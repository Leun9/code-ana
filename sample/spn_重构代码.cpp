#include <stdio.h>

int n;
unsigned int key;
unsigned short plaintext, ciphertext, inverse_plaintext;
const unsigned short sBox_4[16] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
const unsigned short inverse_sBox_4[16] = {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5};
const unsigned short pos[17] = {0x0,
								0x8000, 0x4000, 0x2000, 0x1000,
                                0x0800, 0x0400, 0x0200, 0x0100,
                                0x0080, 0x0040, 0x0020, 0x0010,
                                0x0008, 0x0004, 0x0002, 0x0001};
const unsigned short pBox[17] = {0x0,
								 0x8000, 0x0800, 0x0080, 0x0008,
								 0x4000, 0x0400, 0x0040, 0x0004,
								 0x2000, 0x0200, 0x0020, 0x0002,
								 0x1000, 0x0100, 0x0010, 0x0001};
unsigned short sBox_16[65536], spBox[65536], psBox[65536], inverse_sBox_16[65536];


inline unsigned short read(){
	char ch;
	unsigned short buf = 0;
	for(int i = 0; i < 4; ){
		ch = getchar();
		if(ch >= '0' && ch <= '9'){
			buf = (buf << 4) | (ch ^ 48);
			i++;
		}
		else if(ch >= 'a' && ch <= 'z'){
			buf = (buf << 4) | (ch - 'a' + 10);
			i++;
		}
	}
	return buf;
}

inline void output(){
	int ch;
	for(int i = 0; i < 4; ++i){
		ch = (ciphertext & 0xf000) >> 12;
		if(ch < 10) putchar(ch + '0');
		else putchar(ch - 10 + 'a');
		ciphertext <<= 4;
	}
	putchar(' ');
	for(int i = 0; i < 4; ++i){
		ch = (inverse_plaintext & 0xf000) >> 12;
		if(ch < 10) putchar(ch + '0');
		else putchar(ch - 10 + 'a');
		inverse_plaintext <<= 4;
	}
	putchar('\n');
}

void get_spBox(){
	for(int i = 0; i < 65536; ++i){
		sBox_16[i] = (sBox_4[i >> 12] << 12) | (sBox_4[(i >> 8) & 0xf] << 8) | (sBox_4[(i >> 4) & 0xf] << 4) | sBox_4[i & 0xf];
		spBox[i] = 0;
		for(int j = 1; j <= 16; ++j){
			if(sBox_16[i] & pos[j]) spBox[i] |= pBox[j];
		}
	} 
}

void get_psBox(){
	unsigned short v; 
	for(int i = 0; i < 65536; ++i){
		v = 0;
		for(int j = 1; j <= 16; ++j){
			if(i & pos[j]) v |= pBox[j];
		}
		psBox[i] = (inverse_sBox_4[v >> 12] << 12) | (inverse_sBox_4[(v >> 8) & 0xf] << 8) | (inverse_sBox_4[(v >> 4) & 0xf] << 4) | inverse_sBox_4[v & 0xf];
		inverse_sBox_16[v] = psBox[i];
	}
} 

int main(){

    scanf("%d", &n);
    int k1, k2, k3, k4, k5;
    get_spBox();
    get_psBox();
    for(int i = 1; i <= n; ++i){
    	key = (read() << 16) | read();
    	plaintext = read();
    	
    	//get_roundKey();
    	k5 = key & 0xffff;
		k4 = (key >> 4) & 0xffff;
		k3 = (key >> 8) & 0xffff;
		k2 = (key >> 12) & 0xffff;
		k1 = (key >> 16) & 0xffff;
		
		//encrypt();
		ciphertext = sBox_16[spBox[spBox[spBox[plaintext ^ k1] ^ k2] ^ k3] ^ k4] ^ k5; 
    	//decrypt();
    	inverse_plaintext = psBox[psBox[psBox[inverse_sBox_16[ciphertext ^ 0x1 ^ k5] ^ k4] ^ k3] ^ k2] ^ k1;
    	
    	output();
	}  

    return 0;
}
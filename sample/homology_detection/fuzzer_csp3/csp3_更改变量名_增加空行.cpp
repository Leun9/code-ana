#include <cstdio>

#include <stack>

#include <cstring>

#include <ctype.h>



using namespace std;



char IIIlllll[1024];

int lllIllll[26][27];



int main(){

	int IllIllll;

	stack<int> sXishu;

	scanf("%d\n", &IllIllll); 

	for (int IIlIllll = 0; IIlIllll < IllIllll; IIlIllll++){

		for (int llIIllll = 0; llIIllll < 26; llIIllll++) for (int IlIIllll = 0; IlIIllll < 27; IlIIllll++) lllIllll[llIIllll][IlIIllll] = 0;

		fgets(IIIlllll, 1024, stdin);

		char* lIIIllll = IIIlllll + strlen(IIIlllll) - 2;

		char* IIIIllll = lIIIllll;

		// 等号右边循环 

		for (;;){

			for (; *IIIIllll != '+' && *IIIIllll != '='; IIIIllll--) ; // pSta为化合物前的=或+

			char* llllIlll = IIIIllll + 1;

			int IlllIlll = 0;

			for (; isdigit(*llllIlll); llllIlll++) IlllIlll = IlllIlll * 10 + *llllIlll - '0'; 

			if (IlllIlll == 0) IlllIlll = 1;

				// 等号右边循环处理化合物

				for (char* lIllIlll = lIIIllll; lIllIlll >= llllIlll;){

					if (*lIllIlll == '('){

						IlllIlll /= sXishu.top();

						sXishu.pop();

						lIllIlll--;

						continue;

					} 

					int IIllIlll = 0;

					int llIlIlll = 1;

					for (;isdigit(*lIllIlll); lIllIlll--){

						IIllIlll += (*lIllIlll - '0') * llIlIlll;

						llIlIlll *= 10;

					}

					if (IIllIlll == 0) IIllIlll = 1;

					if (*lIllIlll == ')'){

						IlllIlll *= IIllIlll;

						sXishu.push(IIllIlll);

						lIllIlll--;

						continue;

					} 

					if (isupper(*lIllIlll)){

						lllIllll[*lIllIlll-'A'][0] += IlllIlll*IIllIlll;

						lIllIlll--;

						continue;

					}

					lllIllll[*(lIllIlll-1)-'A'][*lIllIlll-'a'+1] += IlllIlll*IIllIlll;

					lIllIlll -= 2;

				}

				if (*IIIIllll == '=') break;

				IIIIllll--;

				lIIIllll = IIIIllll;

		}

		//printf("Denghao\n");

		lIIIllll = --IIIIllll;

		for (;;){

			for (; *IIIIllll != '+' && IIIIllll >= IIIlllll; IIIIllll--) ; // pSta为化合物前的=或+

			char* llllIlll = IIIIllll + 1;

			int IlllIlll = 0;

			for (; isdigit(*llllIlll); llllIlll++) IlllIlll = IlllIlll * 10 + *llllIlll - '0'; 

			if (IlllIlll == 0) IlllIlll = 1;

				// 等号右边循环处理化合物

				for (char* lIllIlll = lIIIllll; lIllIlll >= llllIlll;){

					if (*lIllIlll == '('){

						IlllIlll /= sXishu.top();

						sXishu.pop();

						lIllIlll--;

						continue;

					} 

					int IIllIlll = 0;

					int llIlIlll = 1;

					for (;isdigit(*lIllIlll); lIllIlll--){

						IIllIlll += (*lIllIlll - '0') * llIlIlll;

						llIlIlll *= 10;

					}

					if (IIllIlll == 0) IIllIlll = 1;

					if (*lIllIlll == ')'){

						IlllIlll *= IIllIlll;

						sXishu.push(IIllIlll);

						lIllIlll--;

						continue;

					} 

					if (isupper(*lIllIlll)){

						lllIllll[*lIllIlll-'A'][0] -= IlllIlll*IIllIlll;

						lIllIlll--;

						continue;

					}

					lllIllll[*(lIllIlll-1)-'A'][*lIllIlll-'a'+1] -= IlllIlll*IIllIlll;

					lIllIlll -= 2;

				}

				if (IIIIllll < IIIlllll) break;

				IIIIllll--;

				lIIIllll = IIIIllll;			

		}

		int IlIlIlll = 0;

		for (int llIIllll = 0; llIIllll < 26; llIIllll++) for (int IlIIllll = 0; IlIIllll < 27; IlIIllll++) IlIlIlll = IlIlIlll | lllIllll[llIIllll][IlIIllll];

		if (IlIlIlll == 0) printf("Y\n"); else printf("N\n");

	}

	return 0;	 

}



/*
4
H2+O2=H2O
2H2+O2=2H2O
CaCl2+2AgNO3=Ca(NO3)2+2AgCl
Cu+As=Cs+Au
*/


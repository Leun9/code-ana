#include <cstdio>

#include <stack>

#include <cstring>

#include <ctype.h>



using namespace std;



char buf[1024];

int cnt[26][27];



int main(){

	int n;

	stack<int> sXishu;

	scanf("%d\n", &n); 

	for (int zm = 0; zm < n; zm++){

		for (int i = 0; i < 26; i++) for (int j = 0; j < 27; j++) cnt[i][j] = 0;

		fgets(buf, 1024, stdin);

		char* pEnd = buf + strlen(buf) - 2;

		char* pSta = pEnd;

		// 等号右边循环 

		for (;;){

			for (; *pSta != '+' && *pSta != '='; pSta--) ; // pSta为化合物前的=或+

			char* pNum = pSta + 1;

			int xishu = 0;

			for (; isdigit(*pNum); pNum++) xishu = xishu * 10 + *pNum - '0'; 

			if (xishu == 0) xishu = 1;

				// 等号右边循环处理化合物

				for (char* p = pEnd; p >= pNum;){

					if (*p == '('){

						xishu /= sXishu.top();

						sXishu.pop();

						p--;

						continue;

					} 

					int num = 0;

					int x = 1;

					for (;isdigit(*p); p--){

						num += (*p - '0') * x;

						x *= 10;

					}

					if (num == 0) num = 1;

					if (*p == ')'){

						xishu *= num;

						sXishu.push(num);

						p--;

						continue;

					} 

					if (isupper(*p)){

						cnt[*p-'A'][0] += xishu*num;

						p--;

						continue;

					}

					cnt[*(p-1)-'A'][*p-'a'+1] += xishu*num;

					p -= 2;

				}

				if (*pSta == '=') break;

				pSta--;

				pEnd = pSta;

		}

		//printf("Denghao\n");

		pEnd = --pSta;

		for (;;){

			for (; *pSta != '+' && pSta >= buf; pSta--) ; // pSta为化合物前的=或+

			char* pNum = pSta + 1;

			int xishu = 0;

			for (; isdigit(*pNum); pNum++) xishu = xishu * 10 + *pNum - '0'; 

			if (xishu == 0) xishu = 1;

				// 等号右边循环处理化合物

				for (char* p = pEnd; p >= pNum;){

					if (*p == '('){

						xishu /= sXishu.top();

						sXishu.pop();

						p--;

						continue;

					} 

					int num = 0;

					int x = 1;

					for (;isdigit(*p); p--){

						num += (*p - '0') * x;

						x *= 10;

					}

					if (num == 0) num = 1;

					if (*p == ')'){

						xishu *= num;

						sXishu.push(num);

						p--;

						continue;

					} 

					if (isupper(*p)){

						cnt[*p-'A'][0] -= xishu*num;

						p--;

						continue;

					}

					cnt[*(p-1)-'A'][*p-'a'+1] -= xishu*num;

					p -= 2;

				}

				if (pSta < buf) break;

				pSta--;

				pEnd = pSta;			

		}

		int flag = 0;

		for (int i = 0; i < 26; i++) for (int j = 0; j < 27; j++) flag = flag | cnt[i][j];

		if (flag == 0) printf("Y\n"); else printf("N\n");

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


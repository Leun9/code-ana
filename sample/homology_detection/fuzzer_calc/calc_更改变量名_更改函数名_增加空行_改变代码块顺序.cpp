# include<stdio.h>

# include<string.h>

# define N 101

# define M 100007



long long IllllllI(long long lIlIIlll,long long IIIlllll)

;



long long lllllllI(char *IlllIlll,int lIllIlll,int IIllIlll,long long llllIlll)

;



int main()

{

	int IIIlllll,lllIllll,IllIllll,lIlIllll=0,IIlIllll,llIIllll[N],IlIIllll[N],lIIIllll;

	char IIIIllll[N+1][N];

	scanf("%s",IIIIllll[0]),getchar();

	IIlIllll=strlen(IIIIllll[0]);

	scanf("%d",&IIIlllll),getchar();

	for(lllIllll=1; lllIllll<=IIIlllll; lllIllll++)

	{

		scanf("%s",IIIIllll[lllIllll]),getchar();

		llIIllll[lllIllll]=strlen(IIIIllll[lllIllll]);

	}

	int llllIlll[11];

	for(lllIllll=0; lllIllll<=10; lllIllll++)

	llllIlll[lllIllll]=lllllllI(IIIIllll[0],0,IIlIllll-1,lllIllll-5);

	for(lllIllll=1; lllIllll<=IIIlllll; lllIllll++)

	{

		lIIIllll=1;

		for(IllIllll=0; IllIllll<=10; IllIllll++)

		if(llllIlll[IllIllll]!=lllllllI(IIIIllll[lllIllll],0,llIIllll[lllIllll]-1,IllIllll-5))

		{

			lIIIllll=0;

			break;

		}

		if(lIIIllll)

		{

			IlIIllll[lIlIllll]=lllIllll;

			lIlIllll++;

		}

	}

	for(lllIllll=0; lllIllll<lIlIllll; lllIllll++)

	{

		printf("%d%c",IlIIllll[lllIllll],(lllIllll<lIlIllll-1)?' ':10);

		

	}

	return 0;

}



long long lllllllI(char *IlllIlll,int lIllIlll,int IIllIlll,long long llllIlll)

{

	int llIlIlll=0;

    int IlIlIlll[N];

    int lIIlIlll=M;

    int IIIlIlll;

    int lllIllll;

    int lllIIlll=0;

    int IllIIlll=0;

    int lIIIllll;

	for(lllIllll=IIllIlll; lllIllll>=lIllIlll; lllIllll--)

	{

		lIIIllll=0;

		if(IlllIlll[lllIllll]==')')

		lllIIlll+=100;

		if(IlllIlll[lllIllll]=='(')

		lllIIlll-=100;

		if(IlllIlll[lllIllll]=='+')

		IlIlIlll[lllIllll]=lllIIlll+1,llIlIlll++,lIIIllll=1;

		if(IlllIlll[lllIllll]=='-')

		IlIlIlll[lllIllll]=lllIIlll+1,llIlIlll++,lIIIllll=1;

		if(IlllIlll[lllIllll]=='*')

		IlIlIlll[lllIllll]=lllIIlll+2,llIlIlll++,lIIIllll=1;

		if(IlllIlll[lllIllll]=='^')

		IlIlIlll[lllIllll]=lllIIlll+3,llIlIlll++,lIIIllll=1;

		if(lIIIllll && IlIlIlll[lllIllll]<lIIlIlll)

		lIIlIlll=IlIlIlll[lllIllll],IIIlIlll=lllIllll;		

	}

	if(llIlIlll)

	{

		if(IlllIlll[IIIlIlll]=='^')

		return IllllllI(lllllllI(IlllIlll,lIllIlll,IIIlIlll-1,llllIlll),lllllllI(IlllIlll,IIIlIlll+1,IIllIlll,llllIlll));

		if(IlllIlll[IIIlIlll]=='*')

		return (lllllllI(IlllIlll,lIllIlll,IIIlIlll-1,llllIlll)*lllllllI(IlllIlll,IIIlIlll+1,IIllIlll,llllIlll))%M;

		if(IlllIlll[IIIlIlll]=='+')

		return (lllllllI(IlllIlll,lIllIlll,IIIlIlll-1,llllIlll)+lllllllI(IlllIlll,IIIlIlll+1,IIllIlll,llllIlll))%M;

		if(IlllIlll[IIIlIlll]=='-')

		return (lllllllI(IlllIlll,lIllIlll,IIIlIlll-1,llllIlll)-lllllllI(IlllIlll,IIIlIlll+1,IIllIlll,llllIlll))%M;

	}

	else

	{

		for (lllIllll=lIllIlll; lllIllll<=IIllIlll; lllIllll++)

		{

			if(IlllIlll[lllIllll]=='x')

				return llllIlll;

			else

				if (IlllIlll[lllIllll]>='0'&& IlllIlll[lllIllll]<='9')

					IllIIlll=IllIIlll*10+IlllIlll[lllIllll]-'0';

		}

		return IllIIlll;

	}

	return 0;

}



long long IllllllI(long long lIlIIlll,long long IIIlllll)

{

	int  IIlIIlll=1;

	for(int lllIllll=0; lllIllll<IIIlllll; lllIllll++)

	IIlIIlll=IIlIIlll*lIlIIlll%M;

	return IIlIIlll;

}
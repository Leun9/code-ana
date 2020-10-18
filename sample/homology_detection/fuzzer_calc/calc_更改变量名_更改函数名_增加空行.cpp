# include<stdio.h>

# include<string.h>

# define N 101

# define M 100007



long long lllllllI(long long IIIlllll,long long lllIllll)

{

	int  IllIllll=1;

	for(int lIlIllll=0; lIlIllll<lllIllll; lIlIllll++)

	IllIllll=IllIllll*IIIlllll%M;

	return IllIllll;

}



long long IllllllI(char *IIlIllll,int llIIllll,int IlIIllll,long long lIIIllll)

{

	int IIIIllll=0;

    int llllIlll[N];

    int IlllIlll=M;

    int lIllIlll;

    int lIlIllll;

    int IIllIlll=0;

    int llIlIlll=0;

    int IlIlIlll;

	for(lIlIllll=IlIIllll; lIlIllll>=llIIllll; lIlIllll--)

	{

		IlIlIlll=0;

		if(IIlIllll[lIlIllll]==')')

		IIllIlll+=100;

		if(IIlIllll[lIlIllll]=='(')

		IIllIlll-=100;

		if(IIlIllll[lIlIllll]=='+')

		llllIlll[lIlIllll]=IIllIlll+1,IIIIllll++,IlIlIlll=1;

		if(IIlIllll[lIlIllll]=='-')

		llllIlll[lIlIllll]=IIllIlll+1,IIIIllll++,IlIlIlll=1;

		if(IIlIllll[lIlIllll]=='*')

		llllIlll[lIlIllll]=IIllIlll+2,IIIIllll++,IlIlIlll=1;

		if(IIlIllll[lIlIllll]=='^')

		llllIlll[lIlIllll]=IIllIlll+3,IIIIllll++,IlIlIlll=1;

		if(IlIlIlll && llllIlll[lIlIllll]<IlllIlll)

		IlllIlll=llllIlll[lIlIllll],lIllIlll=lIlIllll;		

	}

	if(IIIIllll)

	{

		if(IIlIllll[lIllIlll]=='^')

		return lllllllI(IllllllI(IIlIllll,llIIllll,lIllIlll-1,lIIIllll),IllllllI(IIlIllll,lIllIlll+1,IlIIllll,lIIIllll));

		if(IIlIllll[lIllIlll]=='*')

		return (IllllllI(IIlIllll,llIIllll,lIllIlll-1,lIIIllll)*IllllllI(IIlIllll,lIllIlll+1,IlIIllll,lIIIllll))%M;

		if(IIlIllll[lIllIlll]=='+')

		return (IllllllI(IIlIllll,llIIllll,lIllIlll-1,lIIIllll)+IllllllI(IIlIllll,lIllIlll+1,IlIIllll,lIIIllll))%M;

		if(IIlIllll[lIllIlll]=='-')

		return (IllllllI(IIlIllll,llIIllll,lIllIlll-1,lIIIllll)-IllllllI(IIlIllll,lIllIlll+1,IlIIllll,lIIIllll))%M;

	}

	else

	{

		for (lIlIllll=llIIllll; lIlIllll<=IlIIllll; lIlIllll++)

		{

			if(IIlIllll[lIlIllll]=='x')

				return lIIIllll;

			else

				if (IIlIllll[lIlIllll]>='0'&& IIlIllll[lIlIllll]<='9')

					llIlIlll=llIlIlll*10+IIlIllll[lIlIllll]-'0';

		}

		return llIlIlll;

	}

	return 0;

}



int main()

{

	int lllIllll,lIlIllll,lIIlIlll,IIIlIlll=0,lllIIlll,IllIIlll[N],lIlIIlll[N],IlIlIlll;

	char IIlIIlll[N+1][N];

	scanf("%s",IIlIIlll[0]),getchar();

	lllIIlll=strlen(IIlIIlll[0]);

	scanf("%d",&lllIllll),getchar();

	for(lIlIllll=1; lIlIllll<=lllIllll; lIlIllll++)

	{

		scanf("%s",IIlIIlll[lIlIllll]),getchar();

		IllIIlll[lIlIllll]=strlen(IIlIIlll[lIlIllll]);

	}

	int lIIIllll[11];

	for(lIlIllll=0; lIlIllll<=10; lIlIllll++)

	lIIIllll[lIlIllll]=IllllllI(IIlIIlll[0],0,lllIIlll-1,lIlIllll-5);

	for(lIlIllll=1; lIlIllll<=lllIllll; lIlIllll++)

	{

		IlIlIlll=1;

		for(lIIlIlll=0; lIIlIlll<=10; lIIlIlll++)

		if(lIIIllll[lIIlIlll]!=IllllllI(IIlIIlll[lIlIllll],0,IllIIlll[lIlIllll]-1,lIIlIlll-5))

		{

			IlIlIlll=0;

			break;

		}

		if(IlIlIlll)

		{

			lIlIIlll[IIIlIlll]=lIlIllll;

			IIIlIlll++;

		}

	}

	for(lIlIllll=0; lIlIllll<IIIlIlll; lIlIllll++)

	{

		printf("%d%c",lIlIIlll[lIlIllll],(lIlIllll<IIIlIlll-1)?' ':10);

		

	}

	return 0;

}
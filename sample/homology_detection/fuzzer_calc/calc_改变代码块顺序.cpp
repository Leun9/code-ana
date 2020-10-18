# include<stdio.h>
# include<string.h>
# define N 101
# define M 100007

long long power(long long x,long long n)
;

long long cal(char *p,int op,int ed,long long a)
;

int main()
{
	int n,i,j,t=0,l,len[N],ans[N],flag;
	char str[N+1][N];
	scanf("%s",str[0]),getchar();
	l=strlen(str[0]);
	scanf("%d",&n),getchar();
	for(i=1; i<=n; i++)
	{
		scanf("%s",str[i]),getchar();
		len[i]=strlen(str[i]);
	}
	int a[11];
	for(i=0; i<=10; i++)
	a[i]=cal(str[0],0,l-1,i-5);
	for(i=1; i<=n; i++)
	{
		flag=1;
		for(j=0; j<=10; j++)
		if(a[j]!=cal(str[i],0,len[i]-1,j-5))
		{
			flag=0;
			break;
		}
		if(flag)
		{
			ans[t]=i;
			t++;
		}
	}
	for(i=0; i<t; i++)
	{
		printf("%d%c",ans[i],(i<t-1)?' ':10);
		
	}
	return 0;
}

long long cal(char *p,int op,int ed,long long a)
{
	int cnt=0;
    int pro[N];
    int min=M;
    int mloc;
    int i;
    int proi=0;
    int num=0;
    int flag;
	for(i=ed; i>=op; i--)
	{
		flag=0;
		if(p[i]==')')
		proi+=100;
		if(p[i]=='(')
		proi-=100;
		if(p[i]=='+')
		pro[i]=proi+1,cnt++,flag=1;
		if(p[i]=='-')
		pro[i]=proi+1,cnt++,flag=1;
		if(p[i]=='*')
		pro[i]=proi+2,cnt++,flag=1;
		if(p[i]=='^')
		pro[i]=proi+3,cnt++,flag=1;
		if(flag && pro[i]<min)
		min=pro[i],mloc=i;		
	}
	if(cnt)
	{
		if(p[mloc]=='^')
		return power(cal(p,op,mloc-1,a),cal(p,mloc+1,ed,a));
		if(p[mloc]=='*')
		return (cal(p,op,mloc-1,a)*cal(p,mloc+1,ed,a))%M;
		if(p[mloc]=='+')
		return (cal(p,op,mloc-1,a)+cal(p,mloc+1,ed,a))%M;
		if(p[mloc]=='-')
		return (cal(p,op,mloc-1,a)-cal(p,mloc+1,ed,a))%M;
	}
	else
	{
		for (i=op; i<=ed; i++)
		{
			if(p[i]=='x')
				return a;
			else
				if (p[i]>='0'&& p[i]<='9')
					num=num*10+p[i]-'0';
		}
		return num;
	}
	return 0;
}

long long power(long long x,long long n)
{
	int  rst=1;
	for(int i=0; i<n; i++)
	rst=rst*x%M;
	return rst;
}
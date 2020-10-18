#include<stdio.h>
#define MAX_LEN 1000
void build_tree(int *IIIlllll,int *IllIllll,int IIlIllll,int llIIllll,int IlIIllll)
;
void update_tree(int * IIIlllll,int *IllIllll,int IIlIllll,int llIIllll,int IlIIllll,int IlIlIlll,int lIIlIlll)
 ;
int qury_tree(int * IIIlllll,int *IllIllll,int IIlIllll,int llIIllll,int IlIIllll,int lIIIllll,int IIIIllll)
;
int main()
 {
    int IIIlllll[]={1,3,5,7,9,11};
    int lllIllll = 6;
    int IllIllll[MAX_LEN] = {0};
     
    build_tree(IIIlllll,IllIllll,0,0,lllIllll-1);
    printf("create:\n");
    int lIlIllll;
    for(lIlIllll=0;lIlIllll<15;++lIlIllll)
    {
        printf("tree[%d] = %d\n",lIlIllll,IllIllll[lIlIllll]);
     }
      
    printf("update:\n");
    update_tree(IIIlllll,IllIllll,0,0,lllIllll - 1,4,6);
    for(lIlIllll=0;lIlIllll<15;++lIlIllll)
    {
        printf("tree[%d] = %d\n",lIlIllll,IllIllll[lIlIllll]);
    }
     
    printf("qury_tree:%d\n",qury_tree(IIIlllll,IllIllll,0,0,lllIllll-1,2,5));
    return 0;
 }

int qury_tree(int * IIIlllll,int *IllIllll,int IIlIllll,int llIIllll,int IlIIllll,int lIIIllll,int IIIIllll)
{
    //计算范围在L-R的和
    
    printf("start = %d\n",llIIllll);
    printf("end = %d\n",IlIIllll);
    printf("\n");
    if(IIIIllll<llIIllll||lIIIllll>IlIIllll)
    {
        return 0;
    }
    else if(lIIIllll<= llIIllll && IlIIllll<=IIIIllll)
    {
        return IllIllll[IIlIllll];
    }
    else if(llIIllll==IlIIllll)
    {
        return IllIllll[IIlIllll];
    }
    else
    {  
        int llllIlll = (llIIllll+IlIIllll)/2;
        int IlllIlll=2*IIlIllll+1;
        int lIllIlll=2*IIlIllll+2;
        int IIllIlll = qury_tree(IIIlllll,IllIllll,IlllIlll,llIIllll,llllIlll,lIIIllll,IIIIllll);
        int llIlIlll =  qury_tree(IIIlllll,IllIllll,lIllIlll,llllIlll+1,IlIIllll,lIIIllll,IIIIllll);
        return  IIllIlll+llIlIlll;
    }
}

void update_tree(int * IIIlllll,int *IllIllll,int IIlIllll,int llIIllll,int IlIIllll,int IlIlIlll,int lIIlIlll)
 {
    /*
         int idx:要更改的数在arr中的编号
         int val:要替换更改的数的值
     */
    if(llIIllll==IlIIllll)
    {
        IIIlllll[IlIlIlll] = lIIlIlll;
        IllIllll[IIlIllll]=lIIlIlll;
    }
    else
    {
         int llllIlll = (llIIllll+IlIIllll)/2;
         int IlllIlll=2*IIlIllll+1;
         int lIllIlll=2*IIlIllll+2;
          
         //判断要改的数据落在了哪一个分支上
         if(IlIlIlll >= llIIllll && IlIlIlll <=llllIlll)
         {
            update_tree(IIIlllll,IllIllll,IlllIlll,llIIllll,llllIlll,IlIlIlll,lIIlIlll);
         }
         else
         {
            update_tree(IIIlllll,IllIllll,lIllIlll,llllIlll+1,IlIIllll,IlIlIlll,lIIlIlll);
         }
         IllIllll[IIlIllll] = IllIllll[IlllIlll]+IllIllll[lIllIlll];
    }
     
 }

void build_tree(int *IIIlllll,int *IllIllll,int IIlIllll,int llIIllll,int IlIIllll)
{
    /*
        int * arr:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            y
        int *tree:
        int node:树的根节点
        int start:arr数组的
        int end:arr数组的
    */
    if(llIIllll==IlIIllll)
    {
        IllIllll[IIlIllll] = IIIlllll[llIIllll];   
    }
    else
    {
        int llllIlll = (llIIllll+IlIIllll)/2;
        int IlllIlll = 2*IIlIllll+1;
        int lIllIlll = 2*IIlIllll+2;
         
        build_tree(IIIlllll,IllIllll,IlllIlll,llIIllll,llllIlll);
        build_tree(IIIlllll,IllIllll,lIllIlll,llllIlll+1,IlIIllll);
        IllIllll[IIlIllll] = IllIllll[IlllIlll]+IllIllll[lIllIlll];
    }
 }
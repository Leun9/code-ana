#include<stdio.h>
#define MAX_LEN 1000
void build_tree(int *IIIlllll,int *lllIllll,int IllIllll,int lIlIllll,int IIlIllll)
{
    /*
        int * arr:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            y
        int *tree:
        int node:树的根节点
        int start:arr数组的
        int end:arr数组的
    */
    if(lIlIllll==IIlIllll)
    {
        lllIllll[IllIllll] = IIIlllll[lIlIllll];   
    }
    else
    {
        int llIIllll = (lIlIllll+IIlIllll)/2;
        int IlIIllll = 2*IllIllll+1;
        int lIIIllll = 2*IllIllll+2;
         
        build_tree(IIIlllll,lllIllll,IlIIllll,lIlIllll,llIIllll);
        build_tree(IIIlllll,lllIllll,lIIIllll,llIIllll+1,IIlIllll);
        lllIllll[IllIllll] = lllIllll[IlIIllll]+lllIllll[lIIIllll];
    }
 }
void update_tree(int * IIIlllll,int *lllIllll,int IllIllll,int lIlIllll,int IIlIllll,int IIIIllll,int llllIlll)
 {
    /*
         int idx:要更改的数在arr中的编号
         int val:要替换更改的数的值
     */
    if(lIlIllll==IIlIllll)
    {
        IIIlllll[IIIIllll] = llllIlll;
        lllIllll[IllIllll]=llllIlll;
    }
    else
    {
         int llIIllll = (lIlIllll+IIlIllll)/2;
         int IlIIllll=2*IllIllll+1;
         int lIIIllll=2*IllIllll+2;
          
         //判断要改的数据落在了哪一个分支上
         if(IIIIllll >= lIlIllll && IIIIllll <=llIIllll)
         {
            update_tree(IIIlllll,lllIllll,IlIIllll,lIlIllll,llIIllll,IIIIllll,llllIlll);
         }
         else
         {
            update_tree(IIIlllll,lllIllll,lIIIllll,llIIllll+1,IIlIllll,IIIIllll,llllIlll);
         }
         lllIllll[IllIllll] = lllIllll[IlIIllll]+lllIllll[lIIIllll];
    }
     
 }
int qury_tree(int * IIIlllll,int *lllIllll,int IllIllll,int lIlIllll,int IIlIllll,int IlllIlll,int lIllIlll)
{
    //计算范围在L-R的和
    
    printf("start = %d\n",lIlIllll);
    printf("end = %d\n",IIlIllll);
    printf("\n");
    if(lIllIlll<lIlIllll||IlllIlll>IIlIllll)
    {
        return 0;
    }
    else if(IlllIlll<= lIlIllll && IIlIllll<=lIllIlll)
    {
        return lllIllll[IllIllll];
    }
    else if(lIlIllll==IIlIllll)
    {
        return lllIllll[IllIllll];
    }
    else
    {  
        int llIIllll = (lIlIllll+IIlIllll)/2;
        int IlIIllll=2*IllIllll+1;
        int lIIIllll=2*IllIllll+2;
        int IIllIlll = qury_tree(IIIlllll,lllIllll,IlIIllll,lIlIllll,llIIllll,IlllIlll,lIllIlll);
        int llIlIlll =  qury_tree(IIIlllll,lllIllll,lIIIllll,llIIllll+1,IIlIllll,IlllIlll,lIllIlll);
        return  IIllIlll+llIlIlll;
    }
}
int main()
 {
    int IIIlllll[]={1,3,5,7,9,11};
    int IlIlIlll = 6;
    int lllIllll[MAX_LEN] = {0};
     
    build_tree(IIIlllll,lllIllll,0,0,IlIlIlll-1);
    printf("create:\n");
    int lIIlIlll;
    for(lIIlIlll=0;lIIlIlll<15;++lIIlIlll)
    {
        printf("tree[%d] = %d\n",lIIlIlll,lllIllll[lIIlIlll]);
     }
      
    printf("update:\n");
    update_tree(IIIlllll,lllIllll,0,0,IlIlIlll - 1,4,6);
    for(lIIlIlll=0;lIIlIlll<15;++lIIlIlll)
    {
        printf("tree[%d] = %d\n",lIIlIlll,lllIllll[lIIlIlll]);
    }
     
    printf("qury_tree:%d\n",qury_tree(IIIlllll,lllIllll,0,0,IlIlIlll-1,2,5));
    return 0;
 }
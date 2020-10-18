#include<stdio.h>

#define MAX_LEN 1000

void lIlllllI(int *arr,int *tree,int node,int start,int end)

;

void IllllllI(int * arr,int *tree,int node,int start,int end,int idx,int val)

 ;

int lllllllI(int * arr,int *tree,int node,int start,int end,int L,int R)

;

int main()

 {

    int arr[]={1,3,5,7,9,11};

    int size = 6;

    int tree[MAX_LEN] = {0};

     

    lIlllllI(arr,tree,0,0,size-1);

    printf("create:\n");

    int i;

    for(i=0;i<15;++i)

    {

        printf("tree[%d] = %d\n",i,tree[i]);

     }

      

    printf("update:\n");

    IllllllI(arr,tree,0,0,size - 1,4,6);

    for(i=0;i<15;++i)

    {

        printf("tree[%d] = %d\n",i,tree[i]);

    }

     

    printf("qury_tree:%d\n",lllllllI(arr,tree,0,0,size-1,2,5));

    return 0;

 }



int lllllllI(int * arr,int *tree,int node,int start,int end,int L,int R)

{

    //计算范围在L-R的和

    

    printf("start = %d\n",start);

    printf("end = %d\n",end);

    printf("\n");

    if(R<start||L>end)

    {

        return 0;

    }

    else if(L<= start && end<=R)

    {

        return tree[node];

    }

    else if(start==end)

    {

        return tree[node];

    }

    else

    {  

        int mid = (start+end)/2;

        int left_node=2*node+1;

        int right_node=2*node+2;

        int sum_left = lllllllI(arr,tree,left_node,start,mid,L,R);

        int sum_right =  lllllllI(arr,tree,right_node,mid+1,end,L,R);

        return  sum_left+sum_right;

    }

}



void IllllllI(int * arr,int *tree,int node,int start,int end,int idx,int val)

 {

    /*
         int idx:要更改的数在arr中的编号
         int val:要替换更改的数的值
     */

    if(start==end)

    {

        arr[idx] = val;

        tree[node]=val;

    }

    else

    {

         int mid = (start+end)/2;

         int left_node=2*node+1;

         int right_node=2*node+2;

          

         //判断要改的数据落在了哪一个分支上

         if(idx >= start && idx <=mid)

         {

            IllllllI(arr,tree,left_node,start,mid,idx,val);

         }

         else

         {

            IllllllI(arr,tree,right_node,mid+1,end,idx,val);

         }

         tree[node] = tree[left_node]+tree[right_node];

    }

     

 }



void lIlllllI(int *arr,int *tree,int node,int start,int end)

{

    /*
        int * arr:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            y
        int *tree:
        int node:树的根节点
        int start:arr数组的
        int end:arr数组的
    */

    if(start==end)

    {

        tree[node] = arr[start];   

    }

    else

    {

        int mid = (start+end)/2;

        int left_node = 2*node+1;

        int right_node = 2*node+2;

         

        lIlllllI(arr,tree,left_node,start,mid);

        lIlllllI(arr,tree,right_node,mid+1,end);

        tree[node] = tree[left_node]+tree[right_node];

    }

 }
#ifndef _H_hash_change
#define _H_hash_change
#include<stdlib.h>
#include"data_2.h"

__int64 hashkey32;     //数组哈希值
__int64 hashkey64;     //局面哈希值
__int64 hashitem32[2][high+1][wide+1];           //32位随机数组
__int64 hashitem64[2][high+1][wide+1];           //64位随机数组
typedef struct{
	__int64 hashchecksum;  //64位效验码
	int value;
	int depth;
	int turn;
	int exact;
	int humannum;
	int computernum;
}hash;
hash hashtable[h_maxsize]={{0,0,0,0,0}};


__int64 rand64()          //产生随机的64位哈希值；
{__int64 x;
x=rand()^((__int64)rand()<<15)^((__int64)rand()<<30)^((__int64)rand()<<41);
__int64 y ; 
y = x>> 36 ; 
 x=(x^y)-y ;
 return x;

}


/*
************************************************************************ 
*/


void inithashtable()         //初始化哈希表
{
	int i,j,k;
   srand(5);

for(i=0;i<2;i++)
for(j=0;j<high+1;j++)
for(k=0;k<wide+1;k++)
{hashitem32[i][j][k]=rand64();
 hashitem64[i][j][k]=rand64();
}

 hashkey32=0;
 hashkey64=0;
}

/*
************************************************************************ 
*/

int lookuphashtable(int depth,int turn,int &exact)
{__int64 x;
x=hashkey32%(__int64)h_maxsize;

if(hashtable[x].hashchecksum==hashkey64&&hashtable[x].depth>=depth&&hashtable[x].humannum==humannum
   &&hashtable[x].computernum==computernum&&turn==hashtable[x].turn)
{exact=hashtable[x].exact; return hashtable[x].value;}

else return notfind;
}


void inserthashtable(int depth,int turn,int value,int exact)
{__int64 x=hashkey32%(__int64)h_maxsize;
 hashtable[x].turn=turn;
 hashtable[x].depth=depth;
 hashtable[x].value=value;
 hashtable[x].hashchecksum=hashkey64;
 hashtable[x].humannum=humannum;
 hashtable[x].computernum=computernum;
 hashtable[x].exact=exact;
}



#endif
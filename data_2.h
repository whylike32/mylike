//data2.h

#ifndef _H_data
#define _H_data

#include <stdio.h>

#define wide    5      //���̿��
#define high    5      //���̸߶�
#define human    -1    //��ǰ������Ϊhuman����-1��ʾ����ͬ
#define computer 1
#define maxdepth 20   //����������
#define randstep 10 //����µĲ���
#define horizon 0       //���
#define vertice 1       //����
#define h_maxsize   1000000    //��ϣ��
#define infinit     100000             //���ֵ 
#define basescore  7500    //��������ֵ
#define notfind    666666
//int     winscore=basescore+(wide*high/2+1)*53-(wide*high-wide*high/2-1)*3;  //ʲô��˼��
int modify;      //�������������ֵ
int timeout;
#define time_per_step  5     //ÿ����ʱ���룩
clock_t start_time;
clock_t end_time;
#define yes 1
#define no  0


#define nothing   0
#define dead      1
#define l_dead    2
#define chain_1   1       
#define type_2    2        
#define type_3    3
//#define type_4    4
#define chain_2   5
//#define type_5    6
#define cycle     7
#define longchain 8      
/*
************************************************************************ 
*/
struct box{ int x;
            int y;
			int indegree;        //����
			int typekind;        //��������������
			int isdead;          //�Ƿ��������ӻ�С���񣨱���Ϊ3�ĸ��Ӽ������ٵı���Ϊ2���ھӣ�
			struct box* neighbor[4];   //�ھ�����
			int neighbornum;            //�ھ���
			int longchainsign;        //����ǳ��������и�������ǩ
			};
struct box boxs[high][wide];       //���ӵ����ݽṹ

struct cyc{int length;              //���ĳ���
           struct box* location;    //����ĳһ��ĵ�ַ
		   };
struct cyc cycle_length[6];

int round1;        //�غ���
int turn;          //���巽
int chain_1num;
int type_2num;
int type_3num;
//int type_4num
int chain_2num;
//int type_5num;
int cyclenum;
int longchainnum;
int deadnum;
int l_deadnum;
int signnumber;       //������ǩ��

 int horiz[high+1][wide];       //ֵΪ1��ʾ���бߣ�0��ʾ��ռ��
 int vertic[high][wide+1];      //ͬ��

 char cshowbox[high][wide];     
 char choriz[high+1][wide];
 char cvertic[high][wide+1];

 int humannum;
 int computernum;
 int boxnum=wide*high;  
 int humanfirst=0;
 int computerfirst=0;

 void Initgame()
 {  int i;
    int j;

	for(i=1;i<high-1;i++)
	  for(j=1;j<wide-1;j++)
	     { boxs[i][j].x=i;
	       boxs[i][j].y=j;
		   boxs[i][j].indegree=0;
		   boxs[i][j].typekind=nothing;
		   boxs[i][j].neighbor[0]=&boxs[i][j-1];
		   boxs[i][j].neighbor[1]=&boxs[i-1][j];
		   boxs[i][j].neighbor[2]=&boxs[i][j+1];
		   boxs[i][j].neighbor[3]=&boxs[i+1][j];
		   boxs[i][j].neighbornum=4;
		   boxs[i][j].longchainsign=nothing;
		 }
   for(j=1;j<wide-1;j++)
	  {boxs[0][j].x=0;
       boxs[0][j].y=j;
	   boxs[0][j].indegree=0;
	   boxs[0][j].typekind=nothing;
	   boxs[0][j].isdead=nothing;
	   boxs[0][j].neighbor[0]=&boxs[0][j-1];
	   boxs[0][j].neighbor[1]=&boxs[0][j+1];
	   boxs[0][j].neighbor[2]=&boxs[1][j];
	   boxs[0][j].neighbornum=3;
	   boxs[0][j].longchainsign=nothing;

	   boxs[high-1][j].x=high-1;
	   boxs[high-1][j].y=j;
	   boxs[high-1][j].indegree=0;
	   boxs[high-1][j].typekind=nothing;
	   boxs[high-1][j].isdead=nothing;
	   boxs[high-1][j].neighbor[0]=&boxs[high-1][j-1];
	   boxs[high-1][j].neighbor[1]=&boxs[high-2][j];
	   boxs[high-1][j].neighbor[2]=&boxs[high-1][j+1];
	   boxs[high-1][j].neighbornum=3;
	   boxs[high-1][j].longchainsign=nothing;
	  }

   for(i=1;i<high-1;i++)
     {boxs[i][0].x=i;
      boxs[i][0].y=0;
	  boxs[i][0].indegree=0;
	  boxs[i][0].typekind=nothing;
	  boxs[i][0].isdead=nothing;
	  boxs[i][0].neighbor[0]=&boxs[i-1][0];
	  boxs[i][0].neighbor[1]=&boxs[i][1];
	  boxs[i][0].neighbor[2]=&boxs[i+1][0];
	  boxs[i][0].neighbornum=3;
	  boxs[i][0].longchainsign=nothing;

	  boxs[i][wide-1].x=i;
	  boxs[i][wide-1].y=wide-1;
	  boxs[i][wide-1].indegree=0;
	  boxs[i][wide-1].typekind=nothing;
	  boxs[i][wide-1].isdead=nothing;
	  boxs[i][wide-1].neighbor[0]=&boxs[i][wide-2];
	  boxs[i][wide-1].neighbor[1]=&boxs[i-1][wide-1];
	  boxs[i][wide-1].neighbor[2]=&boxs[i+1][wide-1];
	  boxs[i][wide-1].neighbornum=3;
	  boxs[i][wide-1].longchainsign=nothing;
	 }

   boxs[0][0].x=0;
   boxs[0][0].y=0;
   boxs[0][0].indegree=0;
   boxs[0][0].typekind=nothing;
   boxs[0][0].isdead=nothing;
   boxs[0][0].neighbor[0]=&boxs[0][1];
   boxs[0][0].neighbor[1]=&boxs[1][0];
   boxs[0][0].neighbornum=2;
   boxs[0][0].longchainsign=nothing;
   
   boxs[0][wide-1].x=0;
   boxs[0][wide-1].y=wide-1;
   boxs[0][wide-1].indegree=0;
   boxs[0][wide-1].typekind=nothing;
   boxs[0][wide-1].isdead=nothing;
   boxs[0][wide-1].neighbor[0]=&boxs[0][wide-2];
   boxs[0][wide-1].neighbor[1]=&boxs[1][wide-1];
   boxs[0][wide-1].neighbornum=2;
   boxs[0][wide-1].longchainsign=nothing;

   boxs[high-1][0].x=high-1;
   boxs[high-1][0].y=0;
   boxs[high-1][0].indegree=0;
   boxs[high-1][0].typekind=nothing;
   boxs[high-1][0].isdead=nothing;
   boxs[high-1][0].neighbor[0]=&boxs[high-2][0];
   boxs[high-1][0].neighbor[1]=&boxs[high-1][1];
   boxs[high-1][0].neighbornum=2;
   boxs[high-1][0].longchainsign=nothing;

   boxs[high-1][wide-1].x=high-1;
   boxs[high-1][wide-1].y=wide-1;
   boxs[high-1][wide-1].indegree=0;
   boxs[high-1][wide-1].typekind=nothing;
   boxs[high-1][wide-1].isdead=nothing;
   boxs[high-1][wide-1].neighbor[0]=&boxs[high-1][wide-2];
   boxs[high-1][wide-1].neighbor[1]=&boxs[high-2][wide-1];
   boxs[high-1][wide-1].neighbornum=2;
   boxs[high-1][wide-1].longchainsign=nothing;



   //������ʼ��

 chain_1num=0;
 type_2num=0;
 type_3num=0;
//int type_4num
 chain_2num=0;
//type_5num=0;
 cyclenum=0;
 longchainnum=0;
 deadnum=0;
 l_deadnum=0;
 signnumber=1;   //��1�ſ�ʼ��������ǩ
 modify=0;       //����ֵΪ��ʼΪ0

 humannum=0;
 computernum=0;
 round1=1;
for(i=0;i<6;i++)
{cycle_length[i].length=0;
 cycle_length[i].location=NULL;}
 

//ͼ�γ�ʼ��

for(i=0;i<wide;i++)
for(j=0;j<high;j++)
{
 cshowbox[i][j]=' ';
}
for(i=0;i<high+1;i++)
for(j=0;j<wide;j++)
{horiz[i][j]=0;
 choriz[i][j]=' ';
}
for(i=0;i<high;i++)
for(j=0;j<wide+1;j++)
{vertic[i][j]=0;
 cvertic[i][j]=' ';
}

}//end initgame




#endif //data2.h
#include<iostream.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include"data_2.h"
#include"search_1.h"
#include"printfbox_1.h"
#include"hash_2.h"
/***************************************************************
 *                                                             *
 *                                                             *
 ***************************************************************/
struct box r_boxs[high][wide];  
struct cyc r_cycle_length[6];
int r_round1;        //回合数
int r_turn;          //下棋方
int r_chain_1num;
int r_type_2num;
int r_type_3num;
//int type_4num
int r_chain_2num;
//int type_5num;
int r_cyclenum;
int r_longchainnum;
int r_deadnum;
int r_l_deadnum;
int r_signnumber;       //长链标签号

 int r_horiz[high+1][wide];       //值为1表示已有边，0表示可占边
 int r_vertic[high][wide+1];      //同上

 char r_cshowbox[high][wide];     
 char r_choriz[high+1][wide];
 char r_cvertic[high][wide+1];

 int r_humannum;
 int r_computernum;

 void storestep()
 {int i;int j;
  for(i=0;i<high;i++)
	  for(j=0;j<wide;j++)
	  {r_boxs[i][j]=boxs[i][j];
	   r_cshowbox[i][j]=cshowbox[i][j];
	  }
  for(i=0;i<6;i++)
  {r_cycle_length[i]=cycle_length[i];}
   r_round1=round1;        
   r_turn=turn;         
   r_chain_1num=chain_1num;
   r_type_2num=type_2num;
   r_type_3num=type_3num;
   r_chain_2num=chain_2num;
   r_cyclenum=cyclenum;
   r_longchainnum=longchainnum;
   r_deadnum=deadnum;
   r_l_deadnum=l_deadnum;
   r_signnumber=signnumber; 
   for(i=0;i<high+1;i++)
	   for(j=0;j<wide;j++)
	   {r_horiz[i][j]=horiz[i][j];
	    r_choriz[i][j]=choriz[i][j];
	   }
   for(i=0;i<high;i++)
      for(j=0;j<wide+1;j++)
	  {r_vertic[i][j]=vertic[i][j];
	   r_cvertic[i][j]=cvertic[i][j];
	  }
   r_humannum=humannum;
   r_computernum=computernum;
 }

void back_laststep()
{int i;int j;
  for(i=0;i<high;i++)
	  for(j=0;j<wide;j++)
	  {boxs[i][j]=r_boxs[i][j];
	   cshowbox[i][j]=r_cshowbox[i][j];
	  }
  for(i=0;i<6;i++)
  {cycle_length[i]=r_cycle_length[i];}
   round1=r_round1;        
   turn=r_turn;         
   chain_1num=r_chain_1num;
   type_2num=r_type_2num;
   type_3num=r_type_3num;
   chain_2num=r_chain_2num;
   cyclenum=r_cyclenum;
   longchainnum=r_longchainnum;
   deadnum=r_deadnum;
   l_deadnum=r_l_deadnum;
   signnumber=r_signnumber; 
   for(i=0;i<high+1;i++)
	   for(j=0;j<wide;j++)
	   {horiz[i][j]=r_horiz[i][j];
	    choriz[i][j]=r_choriz[i][j];
	   }
   for(i=0;i<high;i++)
      for(j=0;j<wide+1;j++)
	  {vertic[i][j]=r_vertic[i][j];
	   cvertic[i][j]=r_cvertic[i][j];
	  }
   humannum=r_humannum;
   computernum=r_computernum;
}
/***************************************************************
 *                                                             *
 *                                                             *
 ***************************************************************/

void main()
{
 Initgame();
 inithashtable();   //初始化哈希表
 srand(time(0));
    int turn;
	nextmove bestmove;
    int kind;       //边的类型
    int x;
    int y;
	int addstep;

cout<<"who to move first:"<<endl;
cout<<"human first,enter -1;"<<endl;
cout<<"computer first,enter 1."<<endl;
cin>>turn;
if(turn==human)
humanfirst=1;
else computerfirst=1;

while(computernum+humannum<boxnum)
{	printfbox();
   if(turn==human)
   {while(1)
   {   
	   cout<<"you select:(horizon=0,vertice=1)"<<endl;
	    cin>>kind;
		cin>>x;
		cin>>y;
		if(kind==9&&x==9&&y==9)
		{back_laststep();
		 printfbox();
		 continue;
		}
        storestep();
		if(kind==horizon)
		{if(horiz[x][y]==0)
		    break;
	      printf("select error edge ,try again\n");
		}
		else{if(vertic[x][y]==0)
			   break;
		else printf("select error edge ,try again\n");}
		}
        bestmove.edgekind=kind;
		bestmove.x=x;
		bestmove.y=y;
	addstep=makemove(bestmove,human);}
	   else
	   {timeout=no;
	    start_time=clock();
		end_time=start_time+time_per_step*CLOCKS_PER_SEC;
        int depth=3;
		nextmove last_bestmove;
		do{
			bestmove=bestsearch(depth,infinit,computer,round1);
		   if(timeout)
		   {bestmove=last_bestmove;
		    break;
		   }
           else{last_bestmove=bestmove;depth++;}
		}while(depth<=maxdepth);
		  
	  
	    addstep=makemove(bestmove,computer);
	   }
          
	if(addstep==0)
			turn=-turn;
	round1++;
}

printfbox();
cout<<"game over"<<endl;
if(humannum>computernum)
cout<<"you win the game"<<endl;
else cout<<"you loss"<<endl;
char cwait=getch();
}
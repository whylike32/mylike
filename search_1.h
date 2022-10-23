#ifndef H_evaluation_
#define H_evaluation_
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include"data_2.h"
#include"hash_2.h"


typedef struct {
	int edgekind;      //储存值为horizon或vertice
	int x;         //边的横坐标
	int y;         //边的纵坐标
}nextmove;
/*
*************************************************************************
*/
//函数定义

int search(int depth,int beta,int turn,int round,int gametobeend); //返回搜索值,在search里使用recover数组；
nextmove bestsearch(int depth,int beta,int turn,int round);      //返回下法信息；
int gameover();              //若游戏结束，返回1，否则返回0（即某一方的格子数大于总格子数的一半+1）
void findNextmove(nextmove p[],int &movenum,int round,int &gametobeend);
void unmakemove( nextmove p,int turn,int recovertype[][wide],int recoverdead[][wide],int recoversign[][wide],int recovertypenum[10]);
int makemove( nextmove p,int turn);        //返回值为1，表示下法占格，可以继续下，否则返回0
int value(int turn,int gametobeend);                      //估值函数
void cuttypenum(int typekind);
int randxy();
void savegame(int recovertype[][wide],int recoverdead[][wide],int recoversign[][wide],int recovertypenum[10]);   //在执行makemove前保存局面信息，
/*
*************************************************************************
*/


int gameover()   //！！！！！！！！！！！！！！！！！！！！
{if(humannum<(wide*high)/2+1&&computernum<(wide*high)/2+1)
return 0;
else
return 1;
}

int randxy()
  {int a;
   a=rand()%wide;
   return a;
   
  }
//?

void savegame(int recovertype[][wide],int recoverdead[][wide],int recoversign[][wide],int recovertypenum[10])
{int i;int j;
 for(i=0;i<high;i++)
	 for(j=0;j<wide;j++)
	 {recovertype[i][j]=boxs[i][j].typekind;
	  recoverdead[i][j]=boxs[i][j].isdead;
	  recoversign[i][j]=boxs[i][j].longchainsign;}
 recovertypenum[0]=chain_1num;
 recovertypenum[1]=type_2num;
 recovertypenum[2]=type_3num;
 recovertypenum[3]=chain_2num;
 recovertypenum[4]=signnumber;
 recovertypenum[5]=cyclenum;
 recovertypenum[6]=longchainnum;
 recovertypenum[7]=deadnum;
 recovertypenum[8]=l_deadnum;
 recovertypenum[9]=modify;
}

void cuttypenum(int typekind,int number)
  {switch(typekind)
  {case nothing:break;
   case chain_1:{chain_1num--;break;}
   case type_3:{type_3num--;break;}
   case chain_2:{chain_2num--;break;}
   case cycle:{int i;int min=0; struct cyc temp;
	           for(i=0;i<cyclenum;i++)
			   {if(cycle_length[i].location->longchainsign==number)
			       break;
			   }
			   temp=cycle_length[i];
			   cycle_length[i]=cycle_length[cyclenum-1];
			   cycle_length[cyclenum-1]=temp;
			   
	           cyclenum--;break;}
   case longchain:{longchainnum--;break;}
   default:{printf("cuttypenum error\n");}
  }
  }//end cuttypenum//?

//*****************************************  估值函数value()  ****************************************************
//
//
int value(int turn,int gametobeend)        //！！！！！！！！！！！！！！！！！！！！
{if(gametobeend==1)
	{
		if(turn==human)
         return -(basescore-humannum*5+computernum*7);
        else  return basescore+computernum*7-humannum*5;
	}
	int i;int j;
	int v_longchainnum=longchainnum;
	int chainlength=0;
	int humanbox=humannum;
	int computerbox=computernum;
	for(i=0;i<high;i++)
		for(j=0;j<wide;j++)
		{if(boxs[i][j].typekind==longchain)
		   {chainlength++;continue;}
		 if(boxs[i][j].indegree<2&&boxs[i][j].typekind==nothing)
		 {int m=0;  int a[4]={0};  int x; int y;
		      for(x=0;x<boxs[i][j].neighbornum;x++)
			     {if(boxs[i][j].neighbor[x]->typekind==longchain)
				      {m++;a[x]++;}
			      }
			  if(m==0)
			  {continue;}
			  if(m==1)
			  {chainlength++;continue;}
			  if(m>1)
			  {int havecycle=0;
			   chainlength++;
			   for(x=0;x<boxs[i][j].neighbornum-1;x++)
			    for(y=x+1;y<boxs[i][j].neighbornum;y++)
				   {if(a[x]>0&&a[y]>0&&boxs[i][j].neighbor[x]->longchainsign==boxs[i][j].neighbor[y]->longchainsign)
				            havecycle++;}
				if(havecycle==0)
					v_longchainnum--;
				else v_longchainnum++;
			  }
		 }
		}


	if(turn==human)
	{humanbox+=type_2num;
		if(humanfirst)
	   {
        if((v_longchainnum+modify)%2==wide%2)       //对先下不利
	      return -basescore-(chainlength-(v_longchainnum-1)*2)*3-computerbox*7+humanbox*6;
	      else return -basescore+(chainlength-(v_longchainnum-1)*2)*3-computerbox*7+humanbox*6;
		  }
	   else {
		   if((v_longchainnum+modify)%2==wide%2)
		      return -basescore+(chainlength-(v_longchainnum-1)*2)*3-computerbox*7+humanbox*6;
	            else return  -basescore-(chainlength-(v_longchainnum-1)*2)*3-computerbox*7+humanbox*6;
				}
	   }
	else {
		computerbox+=type_2num;
		if(humanfirst)
		    {
			if((v_longchainnum+modify)%2==wide%2)      
			   return basescore+(chainlength-(v_longchainnum-1)*2)*3+computerbox*7-humanbox*6;
	         else return basescore-(chainlength-(v_longchainnum-1)*2)*3+computerbox*7-humanbox*6;
			 }
	       else {
			   
			   if((v_longchainnum+modify)%2==wide%2)
		        return basescore-(chainlength-(v_longchainnum-1)*2)*3+computerbox*7-humanbox*6;
	            else return basescore+(chainlength-(v_longchainnum-1)*2)*3+computerbox*7-humanbox*6;
				}
	 }
}
  

//***************************************************************************************************************
//搜索算法search()
//
int search(int depth,int beta,int turn,int round,int gametobeend)
{int val;
 int current;
 int result;
 int movenum=0;
 int addstep;
 int hashval;
 int exact=0;
 int i;
 nextmove move[2*wide*high];

 if(depth<=0)
	 return value(turn,gametobeend);
 result=gameover();
 if(result!=0)    //游戏结束
 {
	 if(turn==human)
		 return -(basescore-humannum*5+computernum*7);
 else  return basescore+computernum*7-humannum*5;
 }
if(clock()>end_time)
{timeout=yes;
 return infinit;
}
int recovertype[high][wide];int recoverdead[high][wide];int recoversign[high][wide];int recovertypenum[10];
savegame(recovertype,recoverdead,recoversign,recovertypenum);//保存当前局面
findNextmove(move,movenum,round,gametobeend);                      //搜索可行棋位
/*if(round>=40&&movenum==1)             
depth=depth+1; */       
addstep=makemove(move[0],turn);
hashval=lookuphashtable(depth-1,round,exact);
if(hashval!=notfind)        //查询置换表
{current=addstep==0?-hashval:hashval;
 if(current>=beta)
	 {inserthashtable(depth,round,current,0);
         return current;
	  }
}

if(!exact)      //如果在置换表中未查询到数据或查询到数据但exact=0且不造成剪枝,对节点进行重新搜索
{
if(addstep==0)
{ current=-search(depth-1,infinit,-turn,round+1,gametobeend);}
else {current=search(depth-1,infinit,turn,round+1,gametobeend);}
}
unmakemove(move[0],turn,recovertype,recoverdead,recoversign,recovertypenum);

for(i=1;i<movenum;i++)
{  exact=0;
	if(current<beta)
    {addstep=makemove(move[i],turn);
         hashval=lookuphashtable(depth-1,round,exact);  //查询置换表
        if(hashval!=notfind)
		{val=addstep==0?-hashval:hashval;
		 if(val>=beta)
			 {inserthashtable(depth,round,current,0);
		        return val;
			 }
		}

		if(!exact){                      //如果在置换表中未查询到数据或查询到数据但exact=0且不造成剪枝,对节点进行重新搜索
          if(addstep==0)                 //若下一着是对手下，则取子树搜索值的相反数
			  val=-search(depth-1,-current,-turn,round+1,gametobeend);
		  else val=search(depth-1,infinit,turn,round+1,gametobeend);
		    }
	 unmakemove(move[i],turn,recovertype,recoverdead,recoversign,recovertypenum);
	 
	 if(val>current)
		 current=val;
	 }
    else	return current;   //若值大于上界，则将余下子树裁减
}
if(!timeout)
inserthashtable(depth,round,current,1);
return current;
}


nextmove bestsearch(int depth,int beta,int turn,int round)    //返回下一招的下法信息；
{int val;
 int current;
 int i;
 int movenum=0;
 int addstep;
 int gametobeend=0;
 nextmove move[2*wide*high];
 nextmove bestmove;
 int recovertype[high][wide];int recoverdead[high][wide];int recoversign[high][wide];int recovertypenum[10];
 
 findNextmove(move,movenum,round,gametobeend);
 savegame(recovertype,recoverdead,recoversign,recovertypenum);
 addstep=makemove(move[0],turn);


 if(addstep==0)
 {current=-search(depth-1,infinit,-turn,round+1,gametobeend);}
 else current=search(depth-1,infinit,turn,round+1,gametobeend);
     

 bestmove=move[0];
 unmakemove(move[0],turn,recovertype,recoverdead,recoversign,recovertypenum);
/*if(current>=winscore)
 return bestmove;*/
 for(i=1;i<movenum;i++)
 {   
	 if(current<beta)
	 {addstep=makemove(move[i],turn);
	 
	  if(addstep==0)
		  val=-search(depth-1,-current,-turn,round+1,gametobeend);
	  else val=search(depth-1,infinit,turn,round+1,gametobeend);

	  unmakemove(move[i],turn,recovertype,recoverdead,recoversign,recovertypenum);
	 /* if(val>=winscore)
	  {bestmove=move[i];
	   return bestmove;
	  }*/
	  if(val>current)
	  {current=val;
	   bestmove=move[i];
	  }
	 }
 }
return bestmove;
}//end bestsearch




/********************************************FINDNEXTMOVE && MAKEMOVE && UNMAKEMOVE************************************/
/********************************************                                      ************************************/

void findNextmove(nextmove move[],int &movenum,int round,int &gametobeend)
{int x;int y;int i; int j;int k;
  
 if(type_2num>0)
	{for(i=0;i<high;i++)
	   for(j=0;j<wide;j++)
		   if(boxs[i][j].typekind==type_2)
		      {if(horiz[i][j]==0)
			      {move[movenum].x=i;
		           move[movenum].y=j;
				   move[movenum].edgekind=horizon;
				   movenum++;
				   return;
				   }
		       if(horiz[i+1][j]==0)
				  {move[movenum].x=i+1;
			       move[movenum].y=j;
				   move[movenum].edgekind=horizon;
				   movenum++;
				   return;
				   }
			   if(vertic[i][j]==0)
				  {move[movenum].x=i;
			       move[movenum].y=j;
				   move[movenum].edgekind=vertice;
				   movenum++;
				   return;
				   }
			   if(vertic[i][j+1]==0)
				  {move[movenum].x=i;
			       move[movenum].y=j+1;
				   move[movenum].edgekind=vertice;
				   movenum++;
				   return;
				   }
		   }
  }
 

	  if(deadnum>0)
	   { 
		  for(i=0;i<high;i++)
	       for(j=0;j<wide;j++)
			  if(boxs[i][j].isdead==dead)
			    {struct box* neighbor;
                 neighbor=boxs[i][j].neighbor[0];
		     //1.
				 if(neighbor->neighbornum==1)
					{move[movenum].x=(i+neighbor->x+1)/2;
				     move[movenum].y=(j+neighbor->y+1)/2;
					 move[movenum].edgekind=(neighbor->x-i==0)?vertice:horizon;
					 movenum++;
					 if(neighbor->isdead==dead)               //doublecross 
						 return;
					 if(horiz[neighbor->x][neighbor->y]==0&&!(move[movenum-1].x==neighbor->x
					       &&move[movenum-1].y==neighbor->y&&move[movenum-1].edgekind==horizon))
						   {move[movenum].x=neighbor->x;
						    move[movenum].y=neighbor->y;
							move[movenum].edgekind=horizon;
							movenum++; 
							return;
							}
					   if(vertic[neighbor->x][neighbor->y]==0&&!(move[movenum-1].x==neighbor->x
					       &&move[movenum-1].y==neighbor->y&&move[movenum-1].edgekind==vertice))
						   {move[movenum].x=neighbor->x;
						    move[movenum].y=neighbor->y;
							move[movenum].edgekind=vertice;
							movenum++;
							return;
							}
						if(horiz[neighbor->x+1][neighbor->y]==0&&!(move[movenum-1].x==neighbor->x+1
					       &&move[movenum-1].y==neighbor->y&&move[movenum-1].edgekind==horizon))
						   {move[movenum].x=neighbor->x+1;
						    move[movenum].y=neighbor->y;
							move[movenum].edgekind=horizon;
							movenum++;
							return;
							}
                        if(vertic[neighbor->x][neighbor->y+1]==0&&!(move[movenum-1].x==neighbor->x
					       &&move[movenum-1].y==(neighbor->y+1)&&move[movenum-1].edgekind==vertice))
						   {move[movenum].x=neighbor->x;
						    move[movenum].y=neighbor->y+1;
							move[movenum].edgekind=vertice;
							movenum++;
							return;
							}
				 }
           //2.
				 if(neighbor->neighbornum==2)
				  {int m;
				   if(neighbor->neighbor[0]==&boxs[i][j])
				   { m=1;}
				   else m=0;
				   if((neighbor->neighbor[m]->indegree==2)&&(neighbor->neighbor[m]->isdead!=l_dead))
				   {move[movenum].x=(i+neighbor->x+1)/2;
				     move[movenum].y=(j+neighbor->y+1)/2;
					 move[movenum].edgekind=(neighbor->x-i==0)?vertice:horizon;
					 movenum++;
					 return;
				   }
				   else
				   { move[movenum].x=(i+neighbor->x+1)/2;
				     move[movenum].y=(j+neighbor->y+1)/2;
					 k=move[movenum].edgekind=(neighbor->x-i==0)?vertice:horizon;
					 movenum++;
					 if(neighbor->neighbor[m]->indegree==3)
					 {return;}
					if(horiz[neighbor->x][neighbor->y]==0&&!(move[movenum-1].x==neighbor->x
					       &&move[movenum-1].y==neighbor->y&&move[movenum-1].edgekind==horizon))
						   {move[movenum].x=neighbor->x;
						    move[movenum].y=neighbor->y;
							move[movenum].edgekind=horizon;
							movenum++;
							return;
							}
					   if(vertic[neighbor->x][neighbor->y]==0&&!(move[movenum-1].x==neighbor->x
					       &&move[movenum-1].y==neighbor->y&&move[movenum-1].edgekind==vertice))
						   {move[movenum].x=neighbor->x;
						    move[movenum].y=neighbor->y;
							move[movenum].edgekind=vertice;
							movenum++;
							return;
							}
						if(horiz[neighbor->x+1][neighbor->y]==0&&!(move[movenum-1].x==neighbor->x+1
					       &&move[movenum-1].y==neighbor->y&&move[movenum-1].edgekind==horizon))
						   {move[movenum].x=neighbor->x+1;
						    move[movenum].y=neighbor->y;
							move[movenum].edgekind=horizon;
							movenum++;
							return;
							}
                        if(vertic[neighbor->x][neighbor->y+1]==0&&!(move[movenum-1].x==neighbor->x
					       &&move[movenum-1].y==(neighbor->y+1)&&move[movenum-1].edgekind==vertice))
						   {move[movenum].x=neighbor->x;
						    move[movenum].y=neighbor->y+1;
							move[movenum].edgekind=vertice;
							movenum++;
							return;
							}
				 }
				 }
			  }
	  }
//前randstep回合随机下
if(round<=randstep)
   {
	  while(1)
	  {
	   move[movenum].edgekind=rand()%2;
       x=move[movenum].x=randxy();
	   y=move[movenum].y=randxy();
	   if(move[movenum].edgekind==horizon)
	    {if(x==0||x>=wide||y>=wide)
	      {continue;}
		   if(horiz[x][y]==0&&(boxs[x][y].indegree<2)&&(boxs[x-1][y].indegree<2))
		   {break;}
	  
	    }
	   else
	    {if(y==0||y>=wide||x>=wide)
		  {continue;}
		   if(vertic[x][y]==0&&(boxs[x][y].indegree<2)&&(boxs[x][y-1].indegree<2))
		   { break;}
	    }
	  }
       movenum++;
	   return;
   }
//*************randstep回合之后的处理
	  int checkedge[2][high+1][wide+1]={{0,0,0}};
	  for(i=1;i<high-1;i++)
		  for(j=1;j<wide-1;j++)
		  {if(((boxs[i][j].indegree==0)||(boxs[i][j].indegree==1))&&boxs[i][j].typekind==nothing)
		     {for(k=0;k<boxs[i][j].neighbornum;k++)
			    {if((boxs[i][j].neighbor[k]->typekind!=longchain)&&(boxs[i][j].neighbor[k]->typekind!=chain_2))
				  {move[movenum].x=(i+boxs[i][j].neighbor[k]->x+1)/2;
		           move[movenum].y=(j+boxs[i][j].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=((boxs[i][j].neighbor[k]->x)-i==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
		         }
		      }
		    if(boxs[i][j].typekind==chain_2)      //在中间找到的chain_2肯定是非封闭的
				{if(boxs[i][j].neighbor[0]->typekind==chain_2)
				   k=0;
			     else k=1;
                   move[movenum].x=(i+boxs[i][j].neighbor[k]->x+1)/2;
		           move[movenum].y=(j+boxs[i][j].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=((boxs[i][j].neighbor[k]->x)-i==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }


		  }//for->if


	  for(j=0;j<wide;j++)
		 {if(((boxs[0][j].indegree==0)||(boxs[0][j].indegree==1))&&boxs[0][j].typekind==nothing)
		     {for(k=0;k<boxs[0][j].neighbornum;k++)
			    {if((boxs[0][j].neighbor[k]->typekind!=longchain)&&(boxs[0][j].neighbor[k]->typekind!=chain_2))
				  {move[movenum].x=(0+boxs[0][j].neighbor[k]->x+1)/2;
		           move[movenum].y=(j+boxs[0][j].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=(boxs[0][j].neighbor[k]->x-0==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
                 }
	          
	           if(horiz[0][j]==0)
			   {move[movenum].x=0;
			    move[movenum].y=j;
				move[movenum].edgekind=horizon;
				movenum++;
			   }
			   }
			   if(boxs[0][j].typekind==chain_2&&(boxs[0][j].neighbornum==2||(boxs[0][j].neighbor[0]->neighbornum)==2))//查找非封闭的chain_2
			    {if(boxs[0][j].neighbor[0]->typekind==chain_2)
				   k=0;
			     else k=1;
                   move[movenum].x=(0+boxs[0][j].neighbor[k]->x+1)/2;
		           move[movenum].y=(j+boxs[0][j].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=((boxs[0][j].neighbor[k]->x)-0==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
			   
			  

           if((boxs[high-1][j].indegree==0||boxs[high-1][j].indegree==1)&&boxs[high-1][j].typekind==nothing)
		     {for(k=0;k<boxs[high-1][j].neighbornum;k++)
			    {if((boxs[high-1][j].neighbor[k]->typekind!=longchain)&&(boxs[high-1][j].neighbor[k]->typekind!=chain_2))
				  {move[movenum].x=(high-1+boxs[high-1][j].neighbor[k]->x+1)/2;
		           move[movenum].y=(j+boxs[high-1][j].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=(boxs[high-1][j].neighbor[k]->x-(high-1)==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
                 }
	           if(horiz[high][j]==0)
			   {move[movenum].x=high;
			    move[movenum].y=j;
				move[movenum].edgekind=horizon;
				movenum++;}
		   }
            if(boxs[high-1][j].typekind==chain_2&&(boxs[high-1][j].neighbornum==2||(boxs[high-1][j].neighbor[0]->neighbornum)==2))//查找非封闭的chain_2
			    {if(boxs[high-1][j].neighbor[0]->typekind==chain_2)
				   k=0;
			     else k=1;
                   move[movenum].x=(high-1+boxs[high-1][j].neighbor[k]->x+1)/2;
		           move[movenum].y=(j+boxs[high-1][j].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=((boxs[high-1][j].neighbor[k]->x)-high+1==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
			   
	  }

	  for(i=0;i<high;i++)
	  {if((boxs[i][0].indegree==0||boxs[i][0].indegree==1)&&boxs[i][0].typekind==nothing)
		     {for(k=0;k<boxs[i][0].neighbornum;k++)
			    {if((boxs[i][0].neighbor[k]->typekind!=longchain)&&(boxs[i][0].neighbor[k]->typekind!=chain_2))
				  {move[movenum].x=(i+boxs[i][0].neighbor[k]->x+1)/2;
		           move[movenum].y=(0+boxs[i][0].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=(boxs[i][0].neighbor[k]->x-i==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
                 }
	           if(vertic[i][0]==0)
			   {move[movenum].x=i;
			    move[movenum].y=0;
				move[movenum].edgekind=vertice;
				movenum++;}
			   }
           if(boxs[i][0].typekind==chain_2&&(boxs[i][0].neighbornum==2||(boxs[i][0].neighbor[0]->neighbornum)==2))//查找非封闭的chain_2
			    {if(boxs[i][0].neighbor[0]->typekind==chain_2)
				   k=0;
			     else k=1;
                   move[movenum].x=(i+boxs[i][0].neighbor[k]->x+1)/2;
		           move[movenum].y=(0+boxs[i][0].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=((boxs[i][0].neighbor[k]->x)-i==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
           if((boxs[i][wide-1].indegree==0||boxs[i][wide-1].indegree==1)&&boxs[i][wide-1].typekind==nothing)
		     {for(k=0;k<boxs[i][wide-1].neighbornum;k++)
			    {if((boxs[i][wide-1].neighbor[k]->typekind!=longchain)&&(boxs[i][wide-1].neighbor[k]->typekind!=chain_2))
				  {move[movenum].x=(i+boxs[i][wide-1].neighbor[k]->x+1)/2;
		           move[movenum].y=(wide-1+boxs[i][wide-1].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=(boxs[i][wide-1].neighbor[k]->x-i==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
                 }
	           if(vertic[i][wide]==0)
			   {move[movenum].x=i;
			    move[movenum].y=wide;
				move[movenum].edgekind=vertice;
				movenum++;}
		   }
		   if(boxs[i][wide-1].typekind==chain_2&&(boxs[i][wide-1].neighbornum==2||(boxs[i][wide-1].neighbor[0]->neighbornum)==2))//查找非封闭的chain_2
			    {if(boxs[i][wide-1].neighbor[0]->typekind==chain_2)
				   k=0;
			     else k=1;
                   move[movenum].x=(i+boxs[i][wide-1].neighbor[k]->x+1)/2;
		           move[movenum].y=(wide-1+boxs[i][wide-1].neighbor[k]->y+1)/2;
				   move[movenum].edgekind=((boxs[i][wide-1].neighbor[k]->x)-i==0)?vertice:horizon;
				   if(checkedge[move[movenum].edgekind][ move[movenum].x][move[movenum].y]==1)
				   { ;}
				   else{movenum++;
				        checkedge[move[movenum-1].edgekind][move[movenum-1].x][move[movenum-1].y]++;
						}
				   }
	  }


	
//********************残局处理
	  if(movenum<=8)
	  {  if(movenum<=2)
		  gametobeend=1;
		  if(type_3num>0)     
	      {if(boxs[0][0].typekind==type_3)
		    {move[movenum].x=0;
	         move[movenum].y=0;
			 move[movenum].edgekind=horizon;
			 movenum++;
			/* if(horiz[0][0]==1)
		  { printf("i'm findnextmove type_3,error in horiz[0][0]\n");
		  char cwait=getch();}*/
			 return;
			 }
	       
	       if(boxs[0][wide-1].typekind==type_3)
			 {move[movenum].x=0;
		      move[movenum].y=wide-1;
			  move[movenum].edgekind=horizon;
			  movenum++;
			  /*if(horiz[0][wide-1]==1)
		  { printf("i'm findnextmove type_3,error in horiz[0][wide-1]\n");
		  char cwait=getch();}*/
			  return;
			  }

		   if(boxs[high-1][wide-1].typekind==type_3)
			 {move[movenum].x=high;
		      move[movenum].y=wide-1;
			  move[movenum].edgekind=horizon;
			  movenum++;
			  /*if(horiz[high][wide-1]==1)
		  { printf("i'm findnextmove type_3,error in horiz[high][wide-1]\n");
		  char cwait=getch();}*/
			  return;
			  }

		   if(boxs[high-1][0].typekind==type_3)
			 {move[movenum].x=high;
		      move[movenum].y=0;
			  move[movenum].edgekind=horizon;
			  movenum++;
			 /* if(horiz[high][0]==1)
		  { printf("i'm findnextmove type_3,error in horiz[high][0]\n");
		  char cwait=getch();}*/
			  return;
			  }
		   }


		  if(chain_1num>0)
			{
			  if(boxs[0][0].typekind==chain_1)
			  {move[movenum].x=0;
			   move[movenum].y=0;
			   move[movenum].edgekind=horizon;
			   movenum++;
			  /* if(horiz[0][0]==1)
		  { printf("i'm findnextmove chain_1,error in horiz[0][0]\n");
		  char cwait=getch();}*/
			   return;
			  }

              if(boxs[0][wide-1].typekind==chain_1)
			  {move[movenum].x=0;
			   move[movenum].y=wide-1;
			   move[movenum].edgekind=horizon;
			   movenum++;
			   /*if(horiz[0][wide-1]==1)
		  { printf("i'm findnextmove chain_1,error in horiz[0][wide-1]\n");
		  char cwait=getch();}*/
			   return;
			  }

			  if(boxs[high-1][0].typekind==chain_1)
			  {move[movenum].x=high;
			   move[movenum].y=0;
			   move[movenum].edgekind=horizon;
			   movenum++;
			   /*if(horiz[high][0]==1)
		  { printf("i'm findnextmove chain_1,error in horiz[high][0]\n");
		  char cwait=getch();}*/
			   return;
			  }

			  if(boxs[high-1][wide-1].typekind==chain_1)
			  {move[movenum].x=high;
			   move[movenum].y=wide-1;
			   move[movenum].edgekind=horizon;
			   movenum++;
			   /*if(horiz[high][wide-1]==1)
		  { printf("i'm findnextmove chain_1,error in horiz[high][wide-1]\n");
		  char cwait=getch();}*/
			   return;
			  }
		  }

		  if(chain_2num>0)
			{for(i=0;i<high;i++)
			   for(j=0;j<wide;j++)
			   {
				 if(boxs[i][j].typekind==chain_2)
			     {
				   if(boxs[i][j].neighbor[0]->typekind==chain_2)
				   {move[movenum].x=(i+boxs[i][j].neighbor[0]->x+1)/2;
				    move[movenum].y=(j+boxs[i][j].neighbor[0]->y+1)/2;
					move[movenum].edgekind=((i-boxs[i][j].neighbor[0]->x)==0)?vertice:horizon;
					movenum++;
					return;
				
				   }
				   else
				   {move[movenum].x=(i+boxs[i][j].neighbor[1]->x+1)/2;
				    move[movenum].y=(j+boxs[i][j].neighbor[1]->y+1)/2;
					move[movenum].edgekind=((i-boxs[i][j].neighbor[1]->x)==0)?vertice:horizon;
					movenum++;
					return;
				   }
				 }
			   }
		  }

		  if(cyclenum>0)
			{int min=0;
			  for(i=0;i<cyclenum;i++)
			  {if(cycle_length[i].length<cycle_length[min].length)
			      min=i;
			  }
			  move[movenum].x=(cycle_length[min].location->x+cycle_length[min].location->neighbor[0]->x+1)/2;
              move[movenum].y=(cycle_length[min].location->y+cycle_length[min].location->neighbor[0]->y+1)/2;
			  move[movenum].edgekind=(cycle_length[min].location->x-cycle_length[min].location->neighbor[0]->x)==0?vertice:horizon;
			  movenum++;
				   /*if(horiz[move[movenum-1].x][move[movenum-1].y]==1)
			{ printf("i'm findnextmove cycle,error in horiz[%d][%d]\n",move[movenum-1].x,move[movenum-1].y);
		      char cwait=getch();}*/
			  return;
				
			  }

		  if(longchainnum>0)
			{for(i=0;i<high;i++)
			   for(j=0;j<wide;j++)
			    {if(boxs[i][j].typekind==longchain)
				    {move[movenum].x=(i+boxs[i][j].neighbor[0]->x+1)/2;
			         move[movenum].y=(j+boxs[i][j].neighbor[0]->y+1)/2;
					 move[movenum].edgekind=((i-boxs[i][j].neighbor[0]->x)==0)?vertice:horizon;
					 movenum++;
                    /*if(horiz[move[movenum-1].x][move[movenum-1].y]==1)
		  { printf("i'm findnextmove longchain,error in horiz[%d][%d]\n",move[movenum-1].x,move[movenum-1].y);
		  char cwait=getch();}*/
					 return;
					 }
			   }
			  }
  }//end if(movenum==0)
}//end findNextmove;





//*************************** makemove;*/



						      
void unmakemove(nextmove move,int turn,int recovertype[][wide],int recoverdead[][wide],int recoversign[][wide],int recovertypenum[10])
{int i;int j;

 for(i=0;i<high;i++)
	 for(j=0;j<wide;j++)
	 {boxs[i][j].typekind=recovertype[i][j];
	 boxs[i][j].isdead=recoverdead[i][j];
	 boxs[i][j].longchainsign=recoversign[i][j];}
 chain_1num=recovertypenum[0];
 type_2num=recovertypenum[1];
 type_3num=recovertypenum[2];
 chain_2num=recovertypenum[3];
 signnumber=recovertypenum[4];
 cyclenum=recovertypenum[5];
 longchainnum=recovertypenum[6];
 deadnum=recovertypenum[7];
 l_deadnum=recovertypenum[8];
 modify=recovertypenum[9];

 i=move.x;j=move.y;
 if(move.edgekind==horizon)
 {//if(horiz[i][j]==1)
   
	 horiz[i][j]=0;
	 choriz[i][j]=' ';
//undo hash

	  hashkey32=hashkey32-hashitem32[0][i][j];
	  hashkey64=hashkey64-hashitem64[0][i][j];
	  //end
	 switch(i)
	 {case 0:{
		      boxs[i][j].indegree--;
			  if(boxs[i][j].indegree==3)
			  {cshowbox[i][j]=' ';
			    if(turn==human)
				{humannum--;}
				else computernum--;
			  }break;
			  }
	 case high:{boxs[i-1][j].indegree--;
		          if(boxs[i-1][j].indegree==3)
				  {cshowbox[i-1][j]=' ';
				   if(turn==human)
				   {humannum--;}
				   else computernum--;
				  }break;
				 }
	 default:{boxs[i][j].indegree--;
		      boxs[i][j].neighbor[boxs[i][j].neighbornum]=&boxs[i-1][j];
			  boxs[i][j].neighbornum++;
			  if(boxs[i][j].indegree==3)
			  {cshowbox[i][j]=' ';
			   if(turn==human)
			   {humannum--;}
			   else computernum--;
			  }
			  boxs[i-1][j].indegree--;
			  boxs[i-1][j].neighbor[boxs[i-1][j].neighbornum]=&boxs[i][j];
			  boxs[i-1][j].neighbornum++;
			  if(boxs[i-1][j].indegree==3)
			  {cshowbox[i-1][j]=' ';
			     if(turn==human)
				 {humannum--;}
				 else computernum--;
			  }
			 break;}
	 }
 }//end if
 else
 { vertic[i][j]=0;
     cvertic[i][j]=' ';
//undo hash

	  hashkey32=hashkey32-hashitem32[1][i][j];
	  hashkey64=hashkey64-hashitem64[1][i][j];
	  //end
switch(j)
{
case 0:{   boxs[i][j].indegree--;
			  if(boxs[i][j].indegree==3)
			  {cshowbox[i][j]=' ';
			    if(turn==human)
				{humannum--;}
				else computernum--;
			  }break;
			  }
case wide:{    boxs[i][j-1].indegree--;
		          if(boxs[i][j-1].indegree==3)
				  {cshowbox[i][j-1]=' ';
				   if(turn==human)
				   {humannum--;}
				   else computernum--;
				  }break;
				 }
default:{     boxs[i][j].indegree--;
		      boxs[i][j].neighbor[boxs[i][j].neighbornum]=&boxs[i][j-1];
			  boxs[i][j].neighbornum++;
			  if(boxs[i][j].indegree==3)
			  {cshowbox[i][j]=' ';
			   if(turn==human)
			   {humannum--;}
			   else computernum--;
			  }
			  boxs[i][j-1].indegree--;
			  boxs[i][j-1].neighbor[boxs[i][j-1].neighbornum]=&boxs[i][j];
			  boxs[i][j-1].neighbornum++;
			  if(boxs[i][j-1].indegree==3)
			  {cshowbox[i][j-1]=' ';
			     if(turn==human)
				 {humannum--;}
				 else computernum--;
			  }
			 break;}
	 }
 }//end else
}//end unmakemove

		  


  
 int makemove(nextmove move,int turn)
 {int x=move.x; int y=move.y;int typeequal=0;int addstep=0;int i;
 if(move.edgekind==horizon)
 {/*if(horiz[x][y]==1)
 {printf("findnextmove have error in X horiz[%d][%d]:double edge\n",x,y);char cwait=getch();}*/
	 horiz[x][y]=1;
	  choriz[x][y]='-';
//更新哈希表：
		
            hashkey32= hashkey32+hashitem32[0][x][y];
			hashkey64=hashkey64+hashitem64[0][x][y];
//end
       switch(x)
	   {case 0:{   boxs[x][y].indegree++;
			       if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					   }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}

					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }

					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{
							  boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {
					      if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						  addstep=1;
						  }
					 break;}
	   case high:{x=high-1;
			       boxs[x][y].indegree++;
			       if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					     }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}
					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }

					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{
							  boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {
					      if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						  addstep=1;
						  }
					 break;}
	   default:{boxs[x][y].indegree++;
			      for(int i=0;i<boxs[x][y].neighbornum;i++)
					  {if(boxs[x][y].neighbor[i]==&boxs[x-1][y])
					     {for(int j=i;j<boxs[x][y].neighbornum-1;j++)
						    {boxs[x][y].neighbor[j]=boxs[x][y].neighbor[j+1];}
				          break;
						  }
				      }
                       boxs[x][y].neighbornum--;       
				  while(1)
				  {
			        if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					     }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}

					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }

					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						 if(boxs[x][y].typekind==boxs[x-1][y].typekind)
							 typeequal=1;
						 
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{
							  boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }

						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {
						 if(boxs[x-1][y].indegree==3)
							 modify++;
						 if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						  addstep=1;
						  }
					 break;}
					 
//*************************default的另一种情况
					 x=x-1;
					 boxs[x][y].indegree++;
                     for( i=0;i<boxs[x][y].neighbornum;i++)
					  {if(boxs[x][y].neighbor[i]==&boxs[x+1][y])
					     {for(int j=i;j<boxs[x][y].neighbornum-1;j++)
						    {boxs[x][y].neighbor[j]=boxs[x][y].neighbor[j+1];}
				          break;
						  }
				      }
                      boxs[x][y].neighbornum--;
			      
					    if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					     }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}
					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }
					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						 
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{if(typeequal==1)
                               {boxs[x][y].isdead=dead;}
						 else{boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
						      }

                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }

						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						  addstep=1;
						  }
					 break;}
	   }//end switch
 
 }//end if

//*****************************************下的是竖边的情况***************************************************
 else
 {	/*if(vertic[x][y]==1)
 {printf("findnextmove have error in Y vertic[%d][%d] :double edge\n",x,y);char cwait=getch();}*/
	 vertic[x][y]=1;
            cvertic[x][y]='|';
//hash update

	 hashkey32=hashkey32+hashitem32[1][x][y];
	 hashkey64=hashkey64+hashitem64[1][x][y];
//end update
			switch(y)
			{case 0:{boxs[x][y].indegree++;
			        if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					     }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}

					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }

					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{
							  boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						  addstep=1;
						  }
					 break;}
			case wide:{y=y-1;
			       boxs[x][y].indegree++;
			        if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					     }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}
					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }

					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{
							  boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						  addstep=1;
						  }
					 break; }
			default:{boxs[x][y].indegree++;
			        for(int i=0;i<boxs[x][y].neighbornum;i++)
					  {if(boxs[x][y].neighbor[i]==&boxs[x][y-1])
					     {for(int j=i;j<boxs[x][y].neighbornum-1;j++)
						    {boxs[x][y].neighbor[j]=boxs[x][y].neighbor[j+1];}
				          
				          break;
						  }
				      }
                   boxs[x][y].neighbornum--;       
				   
				   while(1)
				  {
			        if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					     }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}

					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }

					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						 if(boxs[x][y].typekind==boxs[x][y-1].typekind)
							 typeequal=1;
						 
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{
							  boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }

						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {if(boxs[x][y-1].indegree==3)
						    modify++;
						 if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						  addstep=1;
						  }
					 break;}
//*************************default的另一种情况
					 y=y-1;
					 boxs[x][y].indegree++;
                     for(i=0;i<boxs[x][y].neighbornum;i++)
					  {if(boxs[x][y].neighbor[i]==&boxs[x][y+1])
					     {for(int j=i;j<boxs[x][y].neighbornum-1;j++)
						    {boxs[x][y].neighbor[j]=boxs[x][y].neighbor[j+1];}
				         
				          break;
						  }
				      }
                     boxs[x][y].neighbornum--;
			        if(boxs[x][y].indegree==1&&boxs[x][y].neighbornum==1)
				   {
					   if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->neighbornum==1))
					     {boxs[x][y].typekind=type_3;
					       boxs[x][y].neighbor[0]->typekind=type_3;
						   type_3num++;
					     }
					
					   break;
				   }

			   if(boxs[x][y].indegree==2)
				{
					     if(boxs[x][y].neighbornum==0)
						  {boxs[x][y].typekind=chain_1;
						   chain_1num++;	break;
						   }
                     if(boxs[x][y].neighbornum==1)
					 {
						 if((boxs[x][y].neighbor[0]->neighbornum)==1&&
							 boxs[x][y].neighbor[0]->indegree==1)
							{boxs[x][y].typekind=type_3;
						     boxs[x][y].neighbor[0]->typekind=type_3;
							 type_3num++;	break;
							 }
                         
						 if(boxs[x][y].typekind==type_3)
							{boxs[x][y].typekind=chain_2;
						     boxs[x][y].neighbor[0]->typekind=chain_2;
							 type_3num--;
							 chain_2num++;	break;
							 }
						 if((boxs[x][y].neighbor[0]->indegree)==2&&
							 (boxs[x][y].neighbor[0]->typekind)==nothing)
							{
							 
                                        boxs[x][y].typekind=chain_2;
								        boxs[x][y].neighbor[0]->typekind=chain_2;
										chain_2num++;	break;


							  }
						 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						   {boxs[x][y].typekind=longchain;
						    boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						      break;}
						 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						   { boxs[x][y].neighbor[0]->typekind=longchain;
						     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
                             boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++;
							 longchainnum++;
							 chain_2num--;	break;
							}
						 
				   break;}  

				  if(boxs[x][y].neighbornum==2)
					{
				     if(((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[1]->indegree)<2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
						 ||((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[0]->indegree)<2&&(boxs[x][y].neighbor[1]->typekind)==nothing))
					  {boxs[x][y].typekind=chain_2;
					   if(boxs[x][y].neighbor[0]->indegree==2)
						  {boxs[x][y].neighbor[0]->typekind=chain_2;}
					   else{boxs[x][y].neighbor[1]->typekind=chain_2;}
					   chain_2num++;	break;
					   }
//长链处理：有一邻居indegree<2;
				if((boxs[x][y].neighbor[0]->indegree)<2)
				 {
					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)   //接的是一个chain_2
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
                         boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 signnumber++;
						 longchainnum++;
						 chain_2num--;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{boxs[x][y].typekind=longchain;
					     boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;      
					     break;}
					 break;
				}


//长链处理：一邻居indegree=2;
				if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->typekind)==nothing)
				 {

                     if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
						 {boxs[x][y].typekind=longchain;
					      boxs[x][y].neighbor[0]->typekind=longchain;
						  boxs[x][y].neighbor[1]->typekind=longchain;
						  boxs[x][y].longchainsign=signnumber;
					      boxs[x][y].neighbor[0]->longchainsign=signnumber;
						  boxs[x][y].neighbor[1]->longchainsign=signnumber;
						  signnumber++;
						  longchainnum++;	break;
						  }

					 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						{boxs[x][y].neighbor[1]->typekind=longchain;
					     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
						 boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[1]->longchainsign=signnumber;
					     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
						 boxs[x][y].neighbor[0]->longchainsign=signnumber;
						 signnumber++;
						 chain_2num--;
						 longchainnum++;	break;
						 }

					 if((boxs[x][y].neighbor[1]->typekind)==longchain)
					    {boxs[x][y].typekind=longchain;
					     boxs[x][y].neighbor[0]->typekind=longchain;
						 boxs[x][y].longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					     boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						 break;}
					 break;
				}
					 
//长链处理：一邻居是chain_2;

					 if((boxs[x][y].neighbor[0]->typekind)==chain_2)
						{boxs[x][y].neighbor[0]->typekind=longchain;
					     boxs[x][y].neighbor[0]->neighbor[0]->typekind=longchain;
						 boxs[x][y].neighbor[0]->neighbor[1]->typekind=longchain;
						 chain_2num--;
						 if((boxs[x][y].neighbor[1]->indegree)<2)
							{
							 boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 signnumber++; 
							 longchainnum++;
						     break;}

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[0]->longchainsign=signnumber;
					         boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						     boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							 boxs[x][y].neighbor[1]->longchainsign=signnumber;
							 signnumber++;
						     longchainnum++;
							 
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
						   {boxs[x][y].neighbor[1]->typekind=longchain;
					        boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
						    boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;

                            boxs[x][y].neighbor[0]->longchainsign=signnumber;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=signnumber;
							boxs[x][y].neighbor[1]->longchainsign=signnumber;
                            boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=signnumber;
						    boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=signnumber;
							signnumber++;
							chain_2num--;
							longchainnum++;	break;}

						if((boxs[x][y].neighbor[1]->typekind)==longchain)
						{   boxs[x][y].neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
					        boxs[x][y].neighbor[0]->neighbor[0]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;
						    boxs[x][y].neighbor[0]->neighbor[1]->longchainsign=boxs[x][y].neighbor[1]->longchainsign;	
							break;              }

						break;}

//长链处理：一邻居是longchain;

					 if((boxs[x][y].neighbor[0]->typekind)==longchain)
						{
					     if(boxs[x][y].neighbor[1]->indegree<2)
						 {      boxs[x][y].typekind=longchain; 	
						        boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;  break;      }

						 if((boxs[x][y].neighbor[1]->indegree)==2&&(boxs[x][y].neighbor[1]->typekind)==nothing)
							{boxs[x][y].typekind=longchain;
							 boxs[x][y].neighbor[1]->typekind=longchain;
							 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 break;}

						 if((boxs[x][y].neighbor[1]->typekind)==chain_2)
							{boxs[x][y].neighbor[1]->typekind=longchain;
						     boxs[x][y].neighbor[1]->neighbor[0]->typekind=longchain;
							 boxs[x][y].neighbor[1]->neighbor[1]->typekind=longchain;
							 boxs[x][y].neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
						     boxs[x][y].neighbor[1]->neighbor[0]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							 boxs[x][y].neighbor[1]->neighbor[1]->longchainsign=boxs[x][y].neighbor[0]->longchainsign;

							 chain_2num--;break;
							 }

						 if((boxs[x][y].neighbor[1]->typekind)==longchain)
							{struct box* prior;struct box* checkdot;
						     
			 //判断是否有环
							 
							 if(boxs[x][y].neighbor[0]->longchainsign==boxs[x][y].neighbor[1]->longchainsign)       //有环
								{/*printf("change typekind to cycle\n")
								 getch();*/
								 int length=1;
								 boxs[x][y].typekind=cycle;
								 boxs[x][y].longchainsign=boxs[x][y].neighbor[0]->longchainsign;
							     prior=&boxs[x][y];
								 checkdot=boxs[x][y].neighbor[0];
								while((checkdot->typekind)==longchain)
								{
									checkdot->typekind=cycle;
									if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
									length++;
								 }
								cycle_length[cyclenum].length=length;
								cycle_length[cyclenum].location=&boxs[x][y];
								cyclenum++;
								longchainnum--;	break;
								}
							 else{
							 int sign=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?boxs[x][y].neighbor[0]->longchainsign
								                                                                                   :boxs[x][y].neighbor[1]->longchainsign;
							 int m=(boxs[x][y].neighbor[0]->longchainsign<boxs[x][y].neighbor[1]->longchainsign)?1:0;

                                prior=&boxs[x][y];
                                checkdot=boxs[x][y].neighbor[m];
								while((checkdot->typekind)==longchain)  //修改其中一个长链的标签号，使其统一
								{checkdot->longchainsign=sign;
									if(checkdot->neighbornum==1)
								    {break;}
								 if((checkdot->neighbor[0])!=prior)
								   {prior=checkdot;
							        checkdot=checkdot->neighbor[0];
									}
							        else{prior=checkdot;
								      checkdot=checkdot->neighbor[1];
									  }
								}

								 boxs[x][y].typekind=longchain;     //无环，则把两长链合成一长链，长链数减1 
							     boxs[x][y].longchainsign=sign;
							      longchainnum--;	break;
								  }
							 }
						 }
					 }
					 }

					 if(boxs[x][y].indegree==3)
						{
					     deadnum++;
						 
						 if(boxs[x][y].isdead==l_dead)
							 {boxs[x][y].isdead=dead;
						      l_deadnum--;}
						 
						 else{if(typeequal==1)
                               {boxs[x][y].isdead=dead;}
						 else{boxs[x][y].isdead=dead;
							  cuttypenum(boxs[x][y].typekind,boxs[x][y].longchainsign);
							  }
						      }

                         if((boxs[x][y].neighbornum==0)||(boxs[x][y].neighbor[0]->indegree)<2)
							{boxs[x][y].typekind=type_2;
						     type_2num++;
							 deadnum--; break;
							 }

						 if((boxs[x][y].neighbor[0]->indegree)==2&&(boxs[x][y].neighbor[0]->isdead)!=l_dead)
							 {boxs[x][y].neighbor[0]->isdead=l_dead;
						      l_deadnum++;}
						 break;
						 }

					 if(boxs[x][y].indegree==4)
						 {if(boxs[x][y].typekind==type_2)
							  type_2num--;
						    else deadnum--;
							boxs[x][y].typekind=nothing;
							boxs[x][y].isdead=nothing;
					      if(turn==human)
							 { humannum++;
						       cshowbox[x][y]='H';
							   }
						  else{computernum++;
						       cshowbox[x][y]='c';}
						 addstep=1;
						  }
                   break;}
			}
 }//end else
 return addstep;
 }//endmakemove

#endif H_evaluation_
   










		  
	 
		     
  
  

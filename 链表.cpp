#include <stdio.h>
#include <stdlib.h>
//创建节点  第一种
typedef struct node
{
	int data;
	struct node *next;
}Lnode;
//第二种尽量不要用
/*typedef struct        
{
	int data;
	struct node *next;
}Lnode;*/

/*struct node         第三种
{
	int data;
	struct node *next;
};
//结构体别名
typedef struct node	Lnode;*/

//创建链表
Lnode* creatlink()
{
	Lnode *head;
	head=(Lnode *)malloc(sizeof(Lnode));
	head->next=NULL;
	return head;
}

void main()
{
	Lnode *L;
	L=creatlink();
	L->data=1;
	printf("%d",L->data);
}

	
	

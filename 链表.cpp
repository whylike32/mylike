#include <stdio.h>
#include <stdlib.h>
//�����ڵ�  ��һ��
typedef struct node
{
	int data;
	struct node *next;
}Lnode;
//�ڶ��־�����Ҫ��
/*typedef struct        
{
	int data;
	struct node *next;
}Lnode;*/

/*struct node         ������
{
	int data;
	struct node *next;
};
//�ṹ�����
typedef struct node	Lnode;*/

//��������
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

	
	

//�������������㷨
#include<iostream>
#include<string>
#include<stdlib.h>
#include"SqString.h"
using namespace std;
typedef char ElemType;
typedef struct lnode
{
	int tag;					//������ͱ�ʶ��ԭ��Ϊ0
	union
	{
		SqString District;			//������
		struct lnode* sublist;	//ָ���ӱ��ָ��
	} val;
	struct lnode* link;			//ָ����һ��Ԫ��
} GLNode;						//�����������

int GLLength(GLNode* g)			//������g�ĳ���
{
	int n = 0;
	GLNode* g1;
	g1 = g->val.sublist;			//gָ������ĵ�һ��Ԫ��
	while (g1 != NULL)
	{
		n++;					//�ۼ�Ԫ�ظ���
		g1 = g1->link;
	}
	return n;
}
int GLDepth(GLNode* g)			//������g�����
{
	GLNode* g1;
	int maxd = 0, dep;
	if (g->tag == 0)				//Ϊԭ��ʱ����0
		return 0;
	g1 = g->val.sublist;			//g1ָ���һ��Ԫ��
	if (g1 == NULL)				//Ϊ�ձ�ʱ����1
		return 1;
	while (g1 != NULL)			//�������е�ÿһ��Ԫ��
	{
		if (g1->tag == 1)			//Ԫ��Ϊ�ӱ�����
		{
			dep = GLDepth(g1);	//�ݹ��������ӱ�����
			if (dep > maxd)		//maxdΪͬһ����������ӱ�����ȵ����ֵ
				maxd = dep;
		}
		g1 = g1->link;			//ʹg1ָ����һ��Ԫ��
	}
	return(maxd + 1);				//���ر�����
}

GLNode* CreateGL(char*& s)				//���������ű�ʾ����ʾs�Ĺ������ʽ�洢�ṹ
{
	GLNode* g;
	char ch = *(s++);						//ȡһ���ַ�
	if (ch != '\0')						//��δ�����ж�
	{
		g = (GLNode*)malloc(sizeof(GLNode));//����һ���½��
		if (ch == '(')					//��ǰ�ַ�Ϊ������ʱ
		{
			if (g)//�ж�g malloc���Ƿ�ɹ����䵽�ռ䣬���Բ�д�����������ᾯ�棬�Ҿͼ�����
			{

				g->tag = 1;					//�½����Ϊ��ͷ���
				g->val.sublist = CreateGL(s); //�ݹ鹹���ӱ�������ͷ���
			}
		}
		else if (ch == '#')				//����'#'�ַ�����ʾΪ�ձ�
			g = NULL;

		else							//Ϊԭ���ַ�
		{
			if (g)//�ж�g malloc���Ƿ�ɹ����䵽�ռ䣬���Բ�д�����������ᾯ�棬�Ҿͼ�����
			{
				
				g->tag = 0;					//�½����Ϊԭ�ӽ��
				//��data��ֵ
				int i = 0;
				while (ch != '(' && ch != ')' && ch != ',' && ch)//��ȡ��������Ϣ
				{
					
					g->val.District.data[i] = ch;					
					ch = *(s++);
					i++;
				}
				g->val.District.length = i;
			}
		}
	}
	else								//������,g��Ϊ��
		g = NULL;
	/*if(g->tag==1)
	{
		ch = *s++;
	}*/

	if (g != NULL)                      	//��δ���������������ֵܽ��
	{
		if (ch == ',')                  	//��ǰ�ַ�Ϊ','
			g->link = CreateGL(s);   		//�ݹ鹹���ֵݽ��

		else                            //û���ֵ���,���ֵ�ָ����ΪNULL
			g->link = NULL;
	}
	return g;                     		//���ع����g
}
void DestroyGL(GLNode*& g)		//���ٹ����
{
	GLNode* g1, * g2;
	g1 = g->val.sublist;			//g1ָ������ĵ�һ��Ԫ��
	while (g1 != NULL)			//��������Ԫ��
	{
		if (g1->tag == 0)			//��Ϊԭ�ӽ��
		{
			g2 = g1->link;		//g2��ʱ�����ֵܽ��
			free(g1);			//�ͷ�g1��ָԭ�ӽ��
			g1 = g2;				//g1ָ�����ֵܽ��
		}
		else					//��Ϊ�ӱ�
		{
			g2 = g1->link;		//g2��ʱ�����ֵܽ��
			DestroyGL(g1);		//�ݹ��ͷ�g1��ָ�ӱ�Ŀռ�
			g1 = g2;				//g1ָ�����ֵܽ��
		}
	}
	free(g);					//�ͷ�ͷ���ռ�
}

void DispGL(GLNode* g)					//��������g
{
	if (g != NULL)                 		//��Ϊ���ж�
	{									//�ȴ���g��Ԫ��
		if (g->tag == 0)               	//g��Ԫ��Ϊԭ��ʱ
			DispStr(g->val.District);  //���������
		else							//g��Ԫ��Ϊ�ӱ�ʱ
		{
			printf("(");                //���'('
			if (g->val.sublist == NULL)  	//Ϊ�ձ�ʱ
				printf("#");
			else						//Ϊ�ǿ��ӱ�ʱ
				DispGL(g->val.sublist); //�ݹ�����ӱ�
			printf(")");               	//���')'
		}
		if (g->link != NULL)
		{
			printf(",");
			DispGL(g->link);            //�ݹ���������������
		}
	}
}

int main()
{
	GLNode* g1;
	GLNode* g2;
	//char a[] = "(������,�Ϻ���,�㶫ʡ(������,��ͷ��),�Ĵ�ʡ(������(ͨ����(��ɽ��))),����ʡ(������(¡����(��ɽ��))),������ʡ(�׸���(�����(��ɽ��))),�㽭ʡ(������,������))";
	char b[] = "(a(c))";
	//char* s1 = a;
	//char b[] = "(a,b)";
	char* s2 = b;

	//char c = '��';
	//printf("%c", c);
	//g1 = CreateGL(s1);
	g2 = CreateGL(s2);
	//DispGL(g1);
	cout << endl;
	DispGL(g2);
	
	//DestroyGL(g1);
	DestroyGL(g2);
	return 1;
}
/*
*（1）从文件读入行政区划，以广义表的单链表示法存储。
*（2）用比较直观的方式显示。
*（3）查找：查找给定行政单位，并显示其所有管辖行政单位。
			查找给定行政单位，并显示其所有上属行政单位。
*（4）统计：总体行政单位个数、县级或乡镇行政单位个数、
			无下属行政单位个数等等。
*（5）求出共有多少行政级别，显示指定级别的行政单位。
*（6）插入新的行政单位。
*（7）删除指定行政单位。
*/
#include<iostream>
#include<string>
#include<stdlib.h>
#include"SqString.h"
#include<fstream>

using namespace std;
typedef char ElemType;
typedef struct lnode
{
	int tag;//原子表示为0，子表表示为1
	int level;//行政级别,
	union
	{
		SqString district;//行政区名称，省级行政区为1，依次递增
		struct lnode* sub_level;//管辖区
	}val;
	struct lnode* same_level;//	同级行政区
} DistrictNode;

string ReadDistrictFile() //从文件中读取字符串
{
	ifstream ifs;
	string s;
	ifs.open("DistrictInfo.txt", ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;

	}	
	ifs >> s;
	ifs.close();	
	return s;
}

//广义表的单链表示法存储

DistrictNode* CreateGL(char*& s,int level) {
	DistrictNode* d;
	char ch = *(s++);						//
	if (ch != '\0')						//
	{
		d = (DistrictNode*)malloc(sizeof(DistrictNode));//
		if (ch == '(')					//
		{
			if (d)//
			{

				d->tag = 1;					//
				d->level = level;
				
				d->val.sub_level = CreateGL(s,level+1);
			}
		}


		else
		{
			if (d)
			{

				d->tag = 0;
				d->level = level;
				int i = 0;
				while (ch != '(' && ch != ')' && ch != ',' && ch)
				{

					d->val.district.data[i] = ch;
					ch = *(s++);
					i++;
				}
				d->val.district.length = i;
			}
		}
	}
	else
		d = NULL;
	if(d->tag==1)
	{
		ch = *s++;
	}

	if (d != NULL)
	{
		if (ch == ',')
			d->same_level = CreateGL(s,d->level);
		
		else
			d->same_level = NULL;
	}
	return d;
}
void DispDistrict(DistrictNode* d,int level=0)			//直观显示		
{
	if (d != NULL)
	{
		if (d->tag == 0)
		{
			DispStr(d->val.district);
		}
		else
		{			
			if (d->val.sub_level == NULL)
				printf("#");
			else
				DispDistrict(d->val.sub_level);
		}
		if (d->same_level != NULL)
		{
			if(d->same_level->tag==0)
			{
				printf("\n");
			
					for (int i = d->level-level; i > 1; i--) {
						printf("\t\t\t");
					}
			
				
			}
			else
			{
				int i = d->val.district.length / 8;
				for (; i < 3; i++) {
					printf("\t");
				}
			}
			DispDistrict(d->same_level);
		}
	}
}

void DispDistrictBracket(DistrictNode* d)			//括号法显示		
{
	if (d != NULL)
	{
		if (d->tag == 0)
			DispStr(d->val.district);
		else
		{
			printf("(");
			if (d->val.sub_level == NULL)
				printf("#");
			else
				DispDistrictBracket(d->val.sub_level);
			printf(")");
		}
		if (d->same_level != NULL)
		{
			printf(",");
			DispDistrictBracket(d->same_level);
		}
	}
}
void DispMenu(DistrictNode* d)
{
	int choice;
	while (true)
	{
		cout << "======================================================================================" << endl;
		cout << "请输入你希望的显示方式" << endl;
		cout << "1---------括号法显示" << endl;
		cout << "2---------直观显示" << endl;
		cout << "0---------退出显示" << endl;
		cout << "======================================================================================" << endl;
		cout << "请输入您的选择：" << endl;
		cin >> choice;
		if (choice == 0) {
			return;
		}

		switch (choice)
		{
		case 1:
			cout << endl << endl;
			DispDistrictBracket(d);
			cout << endl << endl;
			break;
		case 2:
			cout << endl << endl;
			printf("省级行政区\t\t地级行政区\t\t县级行政区\t\t乡级行政区\n");
			printf("--------------------------------------------------------------------------------------\n");
			DispDistrict(d);
			cout << endl << endl;
			break;

		default:
			break;
		}
	}
}
bool SearchSuperior(DistrictNode* d, SqString str,int& count1) //查找上属行政区
{

	if (d == NULL)
	{
		return false;
	}
	if (d->tag == 1)
	{		
		SearchSuperior(d->same_level, str, count1);
		return SearchSuperior(d->val.sub_level, str, count1);
	}
	else if (d->tag == 0)
	{
		if (StrEqual(d->val.district, str))
		{
			count1++;
			return true;
		}
		if (SearchSuperior(d->same_level, str, count1)&& d->same_level->tag==1)
		{
			DispStr(d->val.district);
			cout << " ";
			if(d->level==1)
				cout << endl;
			return true;
		}
		else {
			return false;
		}
	}

	
}
void SearchSub(DistrictNode* d, SqString str, int& count1) //查找管辖行政区
{

	if (d == NULL)
	{
		return;
	}

	if (d->tag == 1)
	{
		SearchSub(d->val.sub_level, str, count1);
		SearchSub(d->same_level, str, count1);
	}
	else if (d->tag == 0)
	{
		if (StrEqual(d->val.district, str))
		{
			count1++;
			if (d->same_level == NULL || d->same_level->tag == 0)
			{
				cout << "无管辖地区" << endl;
			}
			else
			{
				DispDistrict(d->same_level->val.sub_level, d->level);
			}
		}
		SearchSub(d->same_level, str, count1);
	}

}

void StatisticSame(DistrictNode* d, SqString str,int &count, int & level) {
	//count统计查找的同名行政单位出现几次
	if (d != NULL)
	{
		if (d->tag == 1)
		{
			StatisticSame(d->val.sub_level, str, count,level);
			StatisticSame(d->same_level, str, count,level);
		}
		else if (d->tag == 0)
		{
			if (StrEqual(d->val.district, str))
			{
				count++;
				level = d->level;
			}


			StatisticSame(d->same_level, str, count,level);
		}
	}
	
}
void SearchMenu(DistrictNode* d)
{
	int choice;
	while (true)
	{
		cout << "======================================================================================" << endl;
		cout << "请输入你的查询目的" << endl;
		cout << "1---------查找给定行政单位，及其所有管辖行政单位" << endl;
		cout << "2---------查找给定行政单位，及其所有上属行政单位" << endl;
		cout << "0---------退出查找" << endl;
		cout << "======================================================================================" << endl;
		cout << "请输入您的选择：" << endl;
		cin >> choice;
		if (choice == 0) {
			return;
		}
		else if (choice != 1 && choice != 2)
		{
			cout << "输入有误" << endl;
			break;
			return;
		}
		
		string s;
		cout << "请输入查找的行政单位：" << endl;
		cin >> s;
		SqString sqs;
		strcpy_s(sqs.data, s.c_str());
		sqs.length = s.size();
		int count = 0;
		int level = 0;

		switch (choice)
		{
		case 1:
			cout << "其所有管辖行政单位有：" << endl;
			SearchSub(d, sqs, count);
			cout << endl;
			cout << "全国共有" << count << "个同名行政单位" << endl;
			
			break;
		case 2:
			cout << "其所有上属行政单位有：" << endl;
			SearchSuperior(d, sqs,count);
			cout << endl;
			cout << "全国共有" << count << "个同名行政单位" << endl;
			break;
		
		default:
			break;
		}
	}
	
	
}

void Statistic(DistrictNode* d,int& total,int & no_sub) 
{
	if (d != NULL)
	{
		if (d->tag == 0) {
			total++;
			if (d->same_level == NULL)
			{
				no_sub++;
			}
			else if (d->same_level->tag == 0)
			{
				no_sub++;
			}
			
		}
		else
		{
			Statistic(d->val.sub_level, total, no_sub);
		}
		Statistic(d->same_level, total, no_sub);

	}
}
void StatisticLevel(DistrictNode* d, int level,int& level_statistic)
{
	if (d != NULL)
	{
		if (d->tag == 0) {
			
			if (d->level == level)
			{
				level_statistic++;
				/*DispStr(d->val.district);
				cout << endl;*/
			}
			
		}
		else
		{
			StatisticLevel(d->val.sub_level, level, level_statistic);
		}
		StatisticLevel(d->same_level, level, level_statistic);

	}
}

void StatisticMenu(DistrictNode* d)
{
	int total = 0, no_sub = 0;
	Statistic(d, total, no_sub);
	int level_statistic[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		/*cout <<i<<":  " << endl;*/
		StatisticLevel(d,i+1,level_statistic[i]);
		
	}
	cout << "总体行政单位个数=" << total << endl;
	cout << "省级行政单位个数=" << level_statistic[0] << endl;
	cout << "地级行政单位个数=" << level_statistic[1] << endl;
	cout << "县级行政单位个数=" << level_statistic[2] << endl;
	cout << "乡级行政单位个数=" << level_statistic[3] << endl;
	cout << "无下属行政单位个数=" << no_sub << endl;
}

int GLLength(DistrictNode* d)			//原广义表中写好的函数 未作修改 如果没用可以删除
{
	int n = 0;
	DistrictNode* g1;
	g1 = d->val.sub_level;			//
	while (g1 != NULL)
	{
		n++;					// 
		g1 = g1->same_level;
	}
	return n;
}
int GLDepth(DistrictNode* d)			//原广义表中写好的函数 未作修改 如果没用可以删除
{
	DistrictNode* g1;
	int maxd = 0, dep;
	if (d->tag == 0)				//
		return 0;
	g1 = d->val.sub_level;			//
	if (g1 == NULL)				//
		return 1;
	while (g1 != NULL)			//
	{
		if (g1->tag == 1)			//
		{
			dep = GLDepth(g1);	//
			if (dep > maxd)		//
				maxd = dep;
		}
		g1 = g1->same_level;			//
	}
	return(maxd + 1);				//
}

void DestroyGL(DistrictNode*& d)		
{
	DistrictNode* g1, * g2;
	g1 = d->val.sub_level;
	while (g1 != NULL)			
	{
		if (g1->tag == 0)			
		{
			g2 = g1->same_level;		
			free(g1);			
			g1 = g2;				
		}
		else					
		{
			g2 = g1->same_level;		
			DestroyGL(g1);		
			g1 = g2;				  
		}
	}
	free(d);					
}

void Menu() {
	DistrictNode* d1;

	string s = ReadDistrictFile();
	char a[MaxSize] = { 0 };
	strcpy_s(a, s.c_str());
	char* a1 = a;
	d1 = CreateGL(a1, 0);
	DistrictNode* d2 = d1;
	DistrictNode* d3 = d1;
	DistrictNode* d4 = d1;
	int choice;
	while (true)
	{
		cout << "=========================================菜单=========================================" << endl;
		cout << "1---------查看行政区划" << endl;
		cout << "2---------查找行政单位" << endl;
		cout << "3---------统计行政单位" << endl;
		cout << "0---------退出" << endl;
		cout << "======================================================================================" << endl;
		cout << "请输入您的选择：" << endl;
		cin >> choice;
		if (choice == 0) {
			return;
		}

		switch (choice)
		{
		case 1:
			
			DispMenu(d2);
			cout << endl;
			break;
		case 2:
			
			SearchMenu(d3);
			cout << endl;
			break;

		case 3:
			
			StatisticMenu(d1);
			cout << endl;
			break;
		default:
			break;
		}
	}
	DestroyGL(d1);
}


int main()
{

	Menu();
	return 1;
}
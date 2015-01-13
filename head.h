
# pragma once
# include <string>
# include <map>
# include "include.h"
# include "statistics.h"
using namespace std;

class StatisChar_Identify;
extern string origin_position;
// codeBook单字结构，元素间用链表实现
struct UnitInCB
{
	string content;
	int * hierachy;
	UnitInCB * next;
	UnitInCB ** substitute;
	UnitInCB(){ content = ""; hierachy = NULL; next = NULL; substitute = NULL; }
};

// codeBook词条到单字链接结构，元素间用链表实现
struct ItemInCB
{
	UnitInCB **words;
	double weight;
	ItemInCB * next;
	ItemInCB(){ words = NULL; weight = 0.0; next = NULL; }
};

struct myString
{
	string content;
};
//见code.cpp里边重写了myString类的<，当content相同时，必须返回false。

class message_classifier
{
private:
	ItemInCB * codeBook;
	UnitInCB * headOfUnit;
public:
	StatisChar_Identify sta;

private:
	void keyStructure_generator(MessageStructure& msg);
	UnitInCB* insert(UnitInCB* current);
	int SmsCut_SortedInsert(string& source_str, multimap<myString, int>& target, int& location);
	void targetSortedInsert(const string& soc, multimap<myString, int>& target, int& location);
	int codeBook_normalizer(string);
	int substitute_normalizer(string);
	int **malloc2d(int r, int c);

public:
	void Unit_clear()
	{
		for(UnitInCB* i = this->headOfUnit->next; i != NULL; i=i->next)
		{
			delete i->hierachy ;
			i->hierachy = NULL;
		}
	};
	int init(string pathOfCB);
	void print_all();
	int process(MessageStructure & msg);


};


/*
int operator < ( const string a, const string b) 
{
    for( int i = 0; i < a.length() && i < b.length(); i++ )
	{
		if( a[i] < b[i] )
			return -1;
		else if ( a[i] > b[i])
			return 1;
	}
	if (a.length() < b.length() )
		return -1;
	else if ( a.length() > b.length() )
		return 1;
	return 0;
}*/

/*
//插入排序
void insert_sort(node_t * pstHead)
{
	node_t * pstToSort = NULL;
	node_t * pstTmpHead = NULL;
	node_t * pstTmp = NULL;

	if ( !pstHead )
	{
		printf("错误！头节点为空./n");
		exit(1);
	}
	if ( pstHead->pstNext && pstHead->pstNext->pstNext )//如果只有一个元素则不用排序
	{
		pstToSort = pstHead->pstNext->pstNext;//要排序的部分的头节点 第一个不用排序
		pstTmpHead = pstHead;
		pstTmpHead->pstNext->pstNext = NULL; //将第一个元素先挂上
		while ( pstToSort )
		{
			while ( pstTmpHead->pstNext && pstToSort->data > pstTmpHead->pstNext->data )
			{
				pstTmpHead = pstTmpHead->pstNext;
			}
			pstTmp = pstToSort; //将带排序的最前面的节点取出
			pstToSort = pstToSort->pstNext; //带排序链表头节点后移
			pstTmp->pstNext = pstTmpHead->pstNext;//插入pstTmp
			pstTmpHead->pstNext = pstTmp;
			pstTmpHead = pstHead;//pstTmpHead指向头节点进行新一轮插入
		}
	}
}
*/

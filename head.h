
# pragma once
# include <string>
# include <map>
# include "include.h"
# include "statistics.h"
using namespace std;

class StatisChar_Identify;
extern string origin_position;
// codeBook���ֽṹ��Ԫ�ؼ�������ʵ��
struct UnitInCB
{
	string content;
	int * hierachy;
	UnitInCB * next;
	UnitInCB ** substitute;
	UnitInCB(){ content = ""; hierachy = NULL; next = NULL; substitute = NULL; }
};

// codeBook�������������ӽṹ��Ԫ�ؼ�������ʵ��
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
//��code.cpp�����д��myString���<����content��ͬʱ�����뷵��false��

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
//��������
void insert_sort(node_t * pstHead)
{
	node_t * pstToSort = NULL;
	node_t * pstTmpHead = NULL;
	node_t * pstTmp = NULL;

	if ( !pstHead )
	{
		printf("����ͷ�ڵ�Ϊ��./n");
		exit(1);
	}
	if ( pstHead->pstNext && pstHead->pstNext->pstNext )//���ֻ��һ��Ԫ����������
	{
		pstToSort = pstHead->pstNext->pstNext;//Ҫ����Ĳ��ֵ�ͷ�ڵ� ��һ����������
		pstTmpHead = pstHead;
		pstTmpHead->pstNext->pstNext = NULL; //����һ��Ԫ���ȹ���
		while ( pstToSort )
		{
			while ( pstTmpHead->pstNext && pstToSort->data > pstTmpHead->pstNext->data )
			{
				pstTmpHead = pstTmpHead->pstNext;
			}
			pstTmp = pstToSort; //�����������ǰ��Ľڵ�ȡ��
			pstToSort = pstToSort->pstNext; //����������ͷ�ڵ����
			pstTmp->pstNext = pstTmpHead->pstNext;//����pstTmp
			pstTmpHead->pstNext = pstTmp;
			pstTmpHead = pstHead;//pstTmpHeadָ��ͷ�ڵ������һ�ֲ���
		}
	}
}
*/

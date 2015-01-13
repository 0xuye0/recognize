# pragma once
# include <map>
# include <vector>
# include <string>
# include <set>
# include <fstream>
# include <iostream>
# include <math.h>
using namespace std;

typedef struct KeySearch
{
	string keyname;
	float keywordvalue;//������ �ؼ���Ȩֵ �������յ�Ȩֵ����
	vector<int> position;
}KeySearch;//������ȡģ�顢���Ŷȼ�����

typedef struct SmsClassResult
{
	map<string, double> WordNumber;//����ؼ��ʼ�����
	float ScoreFinal;//Ȩֵ�÷�
	int type;//������
	vector<double> FeatureType;//����������ǩ
	vector<int> Feature;
}SmsClassResult;//���Ŷȼ�����

struct MessageStructure//���Žṹ�塪����֮�乵ͨý�顢���Ž���洢���
{
	string msg_content;
	multimap<int, KeySearch> ResultKeyWord;
	double value;
	int type;// �����Ƿ�Ҳ��Ҫɾ��������������������������������������������������
    vector<double> FeatureTag;//����������ǩ

	vector<double> FeatureSum;//��ǩȨֵ
	vector<int> FeatureSumFinal;//��ǩ�����ܺ�
	double ValueSum;//Ȩֵ�ܺ�
	map<string, double> WordValueFinal;
	map<string, double> WordSumFinal;

	MessageStructure()
	{
		msg_content="";
		value=0;
		type=0;
	}
	void clear()
	{
		ResultKeyWord.clear();
		FeatureTag.clear();
		FeatureSum.clear();
		WordValueFinal.clear();
		WordSumFinal.clear();
	}
};//������ȡģ�顢���Ŷȼ�����

struct char_index_contents{
	vector<string> char_contents;
	vector<int> word_label;
};

typedef std::map<int, char_index_contents> charIndexTy;
typedef std::map<int, std::vector<int> > wordIndexTy;
typedef std::map< int, float> valueIndexTy;
typedef std::map< string,double > wordIndexTr;


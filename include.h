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
	float keywordvalue;//增加项 关键词权值 用于最终的权值计算
	vector<int> position;
}KeySearch;//特征抽取模块、可信度计算用

typedef struct SmsClassResult
{
	map<string, double> WordNumber;//输出关键词及个数
	float ScoreFinal;//权值得分
	int type;//分类结果
	vector<double> FeatureType;//分类特征标签
	vector<int> Feature;
}SmsClassResult;//可信度计算用

struct MessageStructure//短信结构体——类之间沟通媒介、短信结果存储结果
{
	string msg_content;
	multimap<int, KeySearch> ResultKeyWord;
	double value;
	int type;// 这里是否也需要删除！！！！！！！！！！！！！！！！！！！！！！！！
    vector<double> FeatureTag;//分类特征标签

	vector<double> FeatureSum;//标签权值
	vector<int> FeatureSumFinal;//标签数量总和
	double ValueSum;//权值总和
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
};//特征抽取模块、可信度计算用

struct char_index_contents{
	vector<string> char_contents;
	vector<int> word_label;
};

typedef std::map<int, char_index_contents> charIndexTy;
typedef std::map<int, std::vector<int> > wordIndexTy;
typedef std::map< int, float> valueIndexTy;
typedef std::map< string,double > wordIndexTr;


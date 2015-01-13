#ifndef IDENTIFY_H_
#define IDENTIFY_H_

#pragma once
# include <vector>
# include <map>
# include <set>
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <math.h>
using namespace std;

class result;

struct Base
{
	//double Chn_ratio;
	double En_ratio;
	double Punc_ratio;
	double Junk_ratio;
	double Num_ratio;
};
struct Contents
{
	int totalNum;
	double ratio;
};
struct Sentive_Base
{	
	double weight;
	int fre;
};
class StatisChar_Identify
{
public:
	bool checkByDistribution;
	bool checkBySentiveWords;
	// 几种字符的数目以及比例
	Contents En;
	Contents punc;
	Contents Chn;
	Contents junk;
	Contents num;
	int num_total;
	//double scoreOfSentiveWords;
	
	// 分布特征数据(初始化！！)
	Base A,B;
	// 敏感字表
	map< string, Sentive_Base > SentiveWord;
	// 敏感字权值判决门限
	double threshold;
	double score_sentive;

	double score_dis;
	// 标点符号
	set< std::string > PuncSet;
	// 文件数据
	//const char* identify_file_path;
	// const char* study_file_path;
public:
	void preparation();
	// 用于分类功能的方式
	void process_identify(string & message);	
	//~StatisChar_Identify();

	int sentive_fre_clear();
	int dis_clear();
	int result_transfer(result & src);
	
private:
	// 特征提取函数
	int featureExt(string& str);
	int statistic_score_set(string string_temp,double& score);
	int threshold_set( string  string_temp);

};

#endif

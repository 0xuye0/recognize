#ifndef SMSCLASSIFYEXT_H__
#define SMSCLASSIFYEXT_H__

#pragma once
#include "include.h"
//#include "MessageStructure.h"//数据库
//#include "DBIntSmsClassify.h"
extern string origin_position;
class SmsClassifyExtraction
{
public :
	//构造函数
	SmsClassifyExtraction();
	void start(map<int, char_index_contents> char_index_n,map<int,vector<int> > word_index_n,map<int,float> wordvalue_index_n);
	
	//析构函数
	~SmsClassifyExtraction();
	
	//类外可调用函数
	int process( MessageStructure & );//该模块对外的调用处理方法（特征抽取）
	//int PrintResult(const std::string &smsoriginal); //调试用的结果输出
private:
	int init();
	//int init(ClsSmsDBSmsClassify *); //数据库
	//ClsSmsDBSmsClassify * m_pDBConn;//数据库
	/*
	std::map<std::string, std::vector<int> > char_index;//字到词索引
	std::map<int,std::vector<std::string> > word_index;//词到字索引
	*/
	std::map<int, char_index_contents> char_index;
	std::map<int, std::vector<int> > word_index;
	std::map<int,float> wordvalue_index;//词到词权值索引
	
	/**************************************************************************/
	//
	// 以下两个数据结构需要进行修改
	//
	/**************************************************************************/
	map< int, vector<int> > SmsLabel;
	map< int, vector< vector<int> > > KeyWord;// 或者KeyWord直接修改ResultKeyWord内容
	multimap<int, KeySearch> ResultKeyWord; // 目标输出

	//短信标注子模块
	void OriginalSmsLable(const std::string &smsoriginal);//短信标注子模块by sxj
	std::set< std::string > PuncSet;//存放标点、符号
	int SmsCut(const std::string &originalsms,std::vector<std::string> &characters);//短信逐字分解
	void LoadPunc();//初始化时需要载入！！！
	int GetFinalKeyword(std::vector<std::string> &characters);
	
	//标注结果处理子模块
	int LabledSmsAnlysis();//标注结果处理子模块by gjy
	int ProcessOneKeyword( map< int,vector< vector<int> > >::iterator& it,float keywordvalue, const string& KeyShowOut);//单个关键词处理
	//ProcessOneKeyword处理单个关键词，函数内部：先调用int GenerateKeyPosInSms(vector<vector<int>>& KeyPosition,vector<string> SingleKey)获得
	//当前关键词中全部关键字在短信中的位置，再调用int GenerateSequence(vector<vector<int>> KeyPosition,vector<int>& Sequence)生成可能排列序列
	//对每一种可能序列调用int ArithmeticSequenceDetect(vector<int> Sequence)判断该序列是否为等差数列
	//若为等差数列，将结果写入map< string, vector<int>> ResultKeyWord
	int GenerateKeyPosInSms(vector< vector<int> >& KeyPosition,const vector<string>& SingleKey);//记录当前关键词中全部关键字在短信中的位置
	int GenerateSequence(vector< vector<int> >& KeyPosition,vector< vector<int> >& Sequence);//生成待处理的可能排列序列
	int ArithmeticSequenceDetect(const vector<int>& Sequence);//检测序列是否等差
	int OrderSequenceDetect(const vector<int>& Sequence);//检测非升序降序序列

	int SmsProcess(const std::string &smsoriginal);


	/******************************************************/
	//													  //
	//		Additional:额外添加的需要的数据结构			  //
	//													  //
	/******************************************************/
	map< int, vector<string> > homo;// 键值为人工主键，内容为对应的同音字序列，初始化在 init()函数进行

};

#endif


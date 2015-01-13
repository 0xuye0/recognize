#ifndef SMSCLASSIFYCALCULATE_H__
#define SMSCLASSIFYCALCULATE_H__

#pragma once
#include "include.h"
extern string origin_position;

class SmsClassifyCalculate//class关键词第一个字母是小写TT
{
public:
	SmsClassifyCalculate();//构造函数
	~SmsClassifyCalculate();//析构函数
	int process(MessageStructure & sms);//该模块对外的调用处理方法（可信度计算）
	int PrintResult();//调试用的结果输出
private:
	SmsClassResult ResultType;//可信度计算的结果
	multimap<int, KeySearch> ResultKeyWord;

	int KeyOriginalJudge(const vector<int>& Sequence);//抽取出的关键词中是否都为原字的判定
	int KeyDoubleJudge(const vector<int>& Sequence); //双字符关键词判定
	float KeyDoubleCal(const vector<int>& Sequence); //双字符关键词权值计算
	int KeyArithmetic(const vector<int>& Sequence);//判断序列是否为等差数列
	float ReverseJudgeAndCal (const vector<int>& Sequence);//判断序列是否为倒序数列 是则给出倒序权值 不是则为1
	float KeyApproxCal(const vector<int>& Sequence);//对于近似等差形式给出该形式权值
	float KeySpaceCal(const vector<int>& Sequence);//对于等差形式给出该形式权值
	int KeyVerticalJudge(const vector<int>& Sequence);//对于竖排形式特征进行判断
	float KeyAppearance(const vector<int>& Sequence);//随机出现权值


	float KeyScoreCal();//计算该特征综合权值
	int TypeJudge();//判别结果并存入ClassResult ResultType

};
#endif


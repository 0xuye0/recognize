#ifndef SMSCLASSIFYCALCULATE_H__
#define SMSCLASSIFYCALCULATE_H__

#pragma once
#include "include.h"
extern string origin_position;

class SmsClassifyCalculate//class�ؼ��ʵ�һ����ĸ��СдTT
{
public:
	SmsClassifyCalculate();//���캯��
	~SmsClassifyCalculate();//��������
	int process(MessageStructure & sms);//��ģ�����ĵ��ô����������Ŷȼ��㣩
	int PrintResult();//�����õĽ�����
private:
	SmsClassResult ResultType;//���Ŷȼ���Ľ��
	multimap<int, KeySearch> ResultKeyWord;

	int KeyOriginalJudge(const vector<int>& Sequence);//��ȡ���Ĺؼ������Ƿ�Ϊԭ�ֵ��ж�
	int KeyDoubleJudge(const vector<int>& Sequence); //˫�ַ��ؼ����ж�
	float KeyDoubleCal(const vector<int>& Sequence); //˫�ַ��ؼ���Ȩֵ����
	int KeyArithmetic(const vector<int>& Sequence);//�ж������Ƿ�Ϊ�Ȳ�����
	float ReverseJudgeAndCal (const vector<int>& Sequence);//�ж������Ƿ�Ϊ�������� �����������Ȩֵ ������Ϊ1
	float KeyApproxCal(const vector<int>& Sequence);//���ڽ��ƵȲ���ʽ��������ʽȨֵ
	float KeySpaceCal(const vector<int>& Sequence);//���ڵȲ���ʽ��������ʽȨֵ
	int KeyVerticalJudge(const vector<int>& Sequence);//����������ʽ���������ж�
	float KeyAppearance(const vector<int>& Sequence);//�������Ȩֵ


	float KeyScoreCal();//����������ۺ�Ȩֵ
	int TypeJudge();//�б���������ClassResult ResultType

};
#endif


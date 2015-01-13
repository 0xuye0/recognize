#ifndef SMSCLASSIFYEXT_H__
#define SMSCLASSIFYEXT_H__

#pragma once
#include "include.h"
//#include "MessageStructure.h"//���ݿ�
//#include "DBIntSmsClassify.h"
extern string origin_position;
class SmsClassifyExtraction
{
public :
	//���캯��
	SmsClassifyExtraction();
	void start(map<int, char_index_contents> char_index_n,map<int,vector<int> > word_index_n,map<int,float> wordvalue_index_n);
	
	//��������
	~SmsClassifyExtraction();
	
	//����ɵ��ú���
	int process( MessageStructure & );//��ģ�����ĵ��ô�������������ȡ��
	//int PrintResult(const std::string &smsoriginal); //�����õĽ�����
private:
	int init();
	//int init(ClsSmsDBSmsClassify *); //���ݿ�
	//ClsSmsDBSmsClassify * m_pDBConn;//���ݿ�
	/*
	std::map<std::string, std::vector<int> > char_index;//�ֵ�������
	std::map<int,std::vector<std::string> > word_index;//�ʵ�������
	*/
	std::map<int, char_index_contents> char_index;
	std::map<int, std::vector<int> > word_index;
	std::map<int,float> wordvalue_index;//�ʵ���Ȩֵ����
	
	/**************************************************************************/
	//
	// �����������ݽṹ��Ҫ�����޸�
	//
	/**************************************************************************/
	map< int, vector<int> > SmsLabel;
	map< int, vector< vector<int> > > KeyWord;// ����KeyWordֱ���޸�ResultKeyWord����
	multimap<int, KeySearch> ResultKeyWord; // Ŀ�����

	//���ű�ע��ģ��
	void OriginalSmsLable(const std::string &smsoriginal);//���ű�ע��ģ��by sxj
	std::set< std::string > PuncSet;//��ű�㡢����
	int SmsCut(const std::string &originalsms,std::vector<std::string> &characters);//�������ַֽ�
	void LoadPunc();//��ʼ��ʱ��Ҫ���룡����
	int GetFinalKeyword(std::vector<std::string> &characters);
	
	//��ע���������ģ��
	int LabledSmsAnlysis();//��ע���������ģ��by gjy
	int ProcessOneKeyword( map< int,vector< vector<int> > >::iterator& it,float keywordvalue, const string& KeyShowOut);//�����ؼ��ʴ���
	//ProcessOneKeyword�������ؼ��ʣ������ڲ����ȵ���int GenerateKeyPosInSms(vector<vector<int>>& KeyPosition,vector<string> SingleKey)���
	//��ǰ�ؼ�����ȫ���ؼ����ڶ����е�λ�ã��ٵ���int GenerateSequence(vector<vector<int>> KeyPosition,vector<int>& Sequence)���ɿ�����������
	//��ÿһ�ֿ������е���int ArithmeticSequenceDetect(vector<int> Sequence)�жϸ������Ƿ�Ϊ�Ȳ�����
	//��Ϊ�Ȳ����У������д��map< string, vector<int>> ResultKeyWord
	int GenerateKeyPosInSms(vector< vector<int> >& KeyPosition,const vector<string>& SingleKey);//��¼��ǰ�ؼ�����ȫ���ؼ����ڶ����е�λ��
	int GenerateSequence(vector< vector<int> >& KeyPosition,vector< vector<int> >& Sequence);//���ɴ�����Ŀ�����������
	int ArithmeticSequenceDetect(const vector<int>& Sequence);//��������Ƿ�Ȳ�
	int OrderSequenceDetect(const vector<int>& Sequence);//��������������

	int SmsProcess(const std::string &smsoriginal);


	/******************************************************/
	//													  //
	//		Additional:������ӵ���Ҫ�����ݽṹ			  //
	//													  //
	/******************************************************/
	map< int, vector<string> > homo;// ��ֵΪ�˹�����������Ϊ��Ӧ��ͬ�������У���ʼ���� init()��������

};

#endif


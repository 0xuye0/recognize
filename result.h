# pragma once
# include "include.h"
# include "head.h"
struct MessageStructure;

class message_classifier;
class SmsClassifyCalculate;
//class StatisChar_Identify;

extern string origin_position;

class result{
public:
	string messageContent;
	int identifyResult;
	
	bool checkBySentiveWords;
	bool checkByDistribution;
	int checkByHierarchy;// 0 1 or 2

	double enRatio;
	double puncRatio;
	double junkRatio;
	double numRatio;
	
	double scoreOfSentiveWords;
	double scoreOfHierarchy;
	double scoreOfDistribution;
	
	vector<int> featureTag_OfHierarchy;
	multimap<int,KeySearch> featureKeyWords_OfHierarchy;

	//分类对象
	//SmsOperationCal test_Hir;
	//StatisChar_Identify test_Sta;
private:


private:
	
	
	void sentiveWords_normalize(double & x);
	void distribution_normalize(double &y);
public:
	//result();
	void clear();
	bool sentence();
	//void store();

public:		
	//friend class StatisChar_Identify;
	void result_transfer_sta(message_classifier& tar);
	void result_transfer_hir(MessageStructure& msg);
	friend class SmsClassifyCalculate;
};

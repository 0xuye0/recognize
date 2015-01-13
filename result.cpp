#include "result.h"
#include <math.h>
#include <iomanip>

void result::clear()
{
	this->messageContent = "";
	this->identifyResult = false;

	this->checkByDistribution = false;
	this->checkByHierarchy = false;
	this->checkBySentiveWords = false;

	this->featureKeyWords_OfHierarchy.clear();
	this->featureTag_OfHierarchy.clear();

	this->enRatio = 0.0;
	this->puncRatio = 0.0;
	this->numRatio = 0.0;
	this->junkRatio = 0.0;

	this->scoreOfSentiveWords = 0.0;
	this->scoreOfHierarchy = 0.0;
}


void result::sentiveWords_normalize(double &x)
{
	double a = 0.1734;
	double b = 0.0056;
	double c = 0.3666;
	double score = 0.0;
	if ( x < 0.6)
	{
		score += pow( (0.6-x), (double)1/3);
		score *= -1;
	}
	else
	{
		score += pow( (x-0.6), (double)1/3);
	}
	score *= a;
	score += b ;
	score += c;
	x = score;
	
}

void result::distribution_normalize(double &y)
{
	if ( y > 0.15)
	{
		y = (log(y-0.149)+ 9.7996)/ 19.277;
	}
	else if ( y < 0)
	{
		y = 0;
	}
}

bool result::sentence()
{
	
	//变换
	distribution_normalize(this->scoreOfDistribution);
	if( this->scoreOfSentiveWords > 0.25 )
	{
		sentiveWords_normalize(this->scoreOfSentiveWords);		
	}
	if ( this->scoreOfDistribution < 0)
	{
		this->scoreOfDistribution = 0.0;
	}
	if ( this->scoreOfHierarchy < 0.01)
	{
		this->scoreOfSentiveWords *= 0.5;
	}

	//最终门限文件
	string temp_merge = origin_position + "final_threshold.txt"; 
	const char* filePath_final_threshold = &temp_merge[0];

	double threshold = 0.0;
	ifstream in(filePath_final_threshold);
	if( !in.is_open() )
	{
		throw 555;
	}
	string string_temp = "";
	getline(in, string_temp);
	threshold = atof( &string_temp[0] );
	// 判决
	if ( this->scoreOfDistribution + this->scoreOfHierarchy + this->scoreOfSentiveWords > threshold)
	{
		this->identifyResult = 2;
		return true;
	}

	if ( this->checkByHierarchy )
	{
		this->identifyResult = 2;
		return true;
	}	
	else
	{
		this->identifyResult = 0;	
		return false;
	}
}

void result::result_transfer_sta(message_classifier& tar)
{	
	this->enRatio = tar.sta.En.ratio;// 1st part
	this->puncRatio = tar.sta.punc.ratio;
	this->junkRatio = tar.sta.junk.ratio;
	this->numRatio = tar.sta.num.ratio;

	this->scoreOfSentiveWords = tar.sta.score_sentive;
	this->scoreOfDistribution = tar.sta.score_dis;
	
	this-> checkByDistribution = tar.sta.checkByDistribution;
	this->checkBySentiveWords = tar.sta.checkBySentiveWords;
}
void result::result_transfer_hir(MessageStructure& msg)
{
	this->messageContent = msg.msg_content;
	this->scoreOfHierarchy = msg.value;// 2nd part
	this->checkByHierarchy = msg.type;
	
	for( vector<double>::iterator iter = msg.FeatureTag.begin(); iter != msg.FeatureTag.end(); iter++)
	{
		this->featureTag_OfHierarchy.push_back( int(*iter) );
	}

	for( multimap<int,KeySearch>::iterator iter = msg.ResultKeyWord.begin(); 
		iter != msg.ResultKeyWord.end(); iter++ )
	{
		// 添加过程
		KeySearch temp_result;
		temp_result.keyname = (iter->second).keyname;
		temp_result.keywordvalue = (iter->second).keywordvalue;
		for(vector<int>::iterator it = (iter->second).position.begin();
			it != (iter->second).position.end();
			it++)
		{
			temp_result.position.push_back( *it );
		}
		this->featureKeyWords_OfHierarchy.insert( make_pair( iter->first, temp_result ) );	
	}
}

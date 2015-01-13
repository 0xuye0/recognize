# include "statistics.h"
# include "result.h"

using namespace std;

extern string origin_position;

// 构造函数，数据置零,敏感词表、标点符号表载入
void StatisChar_Identify::preparation()
{
	//identify_file_path = (const char*)&"toIdentify.txt";
	
	// 数据初始化
	junk.ratio =0.0;
	junk.totalNum = 0;
	En.totalNum = 0;
	En.ratio = 0.0;
	punc.totalNum = 0;
	punc.ratio = 0.0;
	Chn.totalNum = 0;
	Chn.ratio = 0.0;
	num.totalNum = 0;
	num.ratio = 0.0;
	num_total = 0;
	// 表载入（4个）
		// 标点符号表载入
	string temp_merge0 = origin_position + "Punc.txt";
	const char* punc_file_path = &temp_merge0[0];

	fstream file1(punc_file_path,ios::in);
	if ( !file1.is_open() )
	{
		throw 1;
	}
	//file1.open(punc_file_path);
	string temp = "";
	int i = 0;
	while( getline( file1,temp ) )
	{
		PuncSet.insert(temp);
	}
	file1.close();
		// 敏感字表载入(重写)
	string temp_merge1 = origin_position + "SentiveWords_weight.txt";
	const char* sentive_file_path = &temp_merge1[0];
	fstream file2(sentive_file_path,ios::in);
	if ( !file2.is_open() )
	{
		throw 6;
	}
	//file2.open(sentive_file_path);
	string name = "";
	Sentive_Base content;
			// 利用具体内容生成敏感词表
	while( getline(file2,name) )
	{
		std::istringstream stream(name);
		stream >> name;
		stream >> content.weight; // 消耗掉频度内容
		content.fre = 0;
		SentiveWord.insert(std::make_pair(name,content));
	}
	file2.close();
		// 分布特征载入
	string temp_merge2 = origin_position + "StaResult.txt";
	const char* statistic_file_path = &temp_merge2[0];
	fstream file3(statistic_file_path, ios::in);
	if ( !file3.is_open() )
	{
		throw 3;
	}
	//file3.open(statistic_file_path);
			// 根据文件格式提取
	double score =0.0;
	string string_temp = "";
	getline( file3,string_temp );// 读取
	// A类 
			// 英文符号`
	//getline( file3, string_temp);
    statistic_score_set(string_temp.substr(5,6),A.En_ratio);
			// 数字符号
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(12,6),A.Num_ratio);
			// 标点符号
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(19,6),A.Punc_ratio);
			// 垃圾符号
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(26,6),A.Junk_ratio);

	getline(file3,string_temp);
	// B类
			// 英文符号
	//getline( file3, string_temp);
	//cout<<string_temp<<endl;
	//cout<<string_temp.substr(5,6)<<endl;

	statistic_score_set(string_temp.substr(5,6),B.En_ratio);
			// 数字符号
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(12,6),B.Num_ratio);
			// 标点符号
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(19,6),B.Punc_ratio);
			// 垃圾符号
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(26,6),B.Junk_ratio);

	file3.close();
		// 敏感字门限
	string temp_merge3 = origin_position + "SentiveWords_threshold.txt";
	const char* sentiveWords_threshold_file_path = &temp_merge3[0];
	fstream file4(sentiveWords_threshold_file_path, ios::in);
	if ( !file4.is_open() )
	{
		throw 4;
	}
	getline( file4,string_temp);
	threshold_set(string_temp);
	file4.close();

}
// 特征提取函数：两类功能都会调用
int StatisChar_Identify::featureExt(string& source)
{
	// 针对GB18030进行字符分隔，同时统计特征
	std::string::size_type i = 0;
	std::string::size_type end = source.length()-1;
	string str_temp = "";

	while(i<=end)
	{
		// 首先找到4字节字符
		// 其次找到2字节字符
		// 针对第一个字节的值从0x81到0xFE
		if( source[i] >= (char)0x81 && source[i] <= (char)0xfe )
		{
			if ( i+3 <= end)
			{
				// 考虑是否是四字节内容，针对(四字节)第二字节0x30到0x39 && 第三个字节从0x81到0xFE && 第四个字节从0x30到0x39
				if ( source[i+1] >= (char)0x30 && source[i+1] <= (char)0x39 
					&& source[i+2] >= (char)0x81 && source[i+2] <= (char)0xfe
					&& source[i+3] >= (char)0x30 && source[i+3] <= (char)0x39 )
				{
					num_total++;
					// 已经确定是四字节字符，判断是否为中文字符					
					if( (source[i] == (char)0x81 ||  source[i] == (char)0x82)	// 针对CJK扩充A
						||  (source[i] >= (char)0x95 && source[i] <= (char)0x98)) // 针对CJK扩充B
					{
						// 中文字符处理
						Chn.totalNum++;
						//characters.push_back(source.substr(i,4));
						
						// 敏感词处理
						str_temp = source.substr(i,4);
						i += 4;
						if( SentiveWord.find(str_temp) != SentiveWord.end() )
						{
							SentiveWord[str_temp].fre++;							
						}				
						continue;
					}
					else // 非中文字符
					{
						if( PuncSet.find( source.substr(i,4) ) != PuncSet.end() )
						{
							punc.totalNum++;
							//characters.push_back( punc );
							i += 4;						
							continue;
						}
						else
						{
							junk.totalNum++;// 这里认为，既不是中文字符，又不存在于标点符号文件中的字符为垃圾字符
							//characters.push_back(source.substr(i,4));
							i += 4;
							continue;
						}
					}
				}
			}
			if ( i+1 <= end )
			{
				if ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
					|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) )
				{
					num_total++;
					// 考虑双字节中文字符的情况：
					if ( (source[i] >= (char)0xb0 && source[i] <= (char)0xf7
						&& source[i+1] >= (char)0xa1 && source[i+1] <= (char)0xfe) // 6763个汉字

						||( (source[i] >= (char)0x81 && source[i] <= (char)0xa0)// 高字节
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) ) )// 6080个汉字

						||( source[i] >= (char)0xaa && source[i] <= (char)0xfe
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xa0) ) ) ) // 8160个汉字
					{
						Chn.totalNum++;
						//characters.push_back(source.substr(i,2));
						
						// 敏感词处理
						str_temp = source.substr(i,2);
						i += 2;
						if( SentiveWord.find(str_temp) != SentiveWord.end() )
						{
							SentiveWord[str_temp].fre++;
							/*
							// 判断该敏感词是否已经在SentiveWord_result中已经存在
							if ( SentiveWord_result.find(str_temp) != SentiveWord_result.end() )
							{
							// 已经存在
							SentiveWord_result[str_temp]++;
							}
							else
							{
							SentiveWord_result.insert( make_pair( str_temp, 1));
							}
							*/
						}
						continue;
					}
					else// 非中文字符
					{
						if( PuncSet.find( source.substr(i,2) ) != PuncSet.end() )
						{
							punc.totalNum++;
							//characters.push_back( punc );
							i += 2;
							continue;
						}
						else
						{
							junk.totalNum++;
							//characters.push_back(source.substr(i,4));
							i += 2;
							continue;
						}
					}
				}// 双字节考虑 循环结束
			}
			else
			{				
				i++;				
			}
		}// 2 4字节考虑 循环结束
		// 最后考虑单字节字符（目前存在的其他情况字符，应该如何处理）
		else if( source[i] >= (char)0x0 && source[i] <= (char)0x7f )
		{
			str_temp = "";
			// 连续英文字符的合并处理 61-7a： 41-5a
				// 英文字符是否需要进行大小写转换
			if( (source[i] >= (char)0x61 && source[i] <= (char)0x7a)
				|| (source[i]>= (char)0x41 && source[i] <= (char)0x5a) )
			{
				do
				{
					// 转换为小写，再存入
					if ( source[i] <= (char)0x5a )
					{
						str_temp += ( source[i] + (char)0x20 );
					}
					else
					{
						str_temp += source[i];
					}
					En.totalNum++;
					i++;
					num_total++;
				}
				while( (source[i] >= (char)0x61 && source[i] <= (char)0x7a)
				|| (source[i]>= (char)0x41 && source[i] <= (char)0x5a) );
				// 敏感词判断
				if( SentiveWord.find(str_temp) != SentiveWord.end() )
				{
					SentiveWord[str_temp].fre++;
					/*
					// 判断该敏感词是否已经在SentiveWord_result中已经存在
					if ( SentiveWord_result.find(str_temp) != SentiveWord_result.end() )
					{
						// 已经存在
						SentiveWord_result[str_temp]++;
					}
					else
					{
						SentiveWord_result.insert( make_pair( str_temp, 1) );
					}
					*/
				}
				continue;
			}
			// 连续数字的合并处理 30-39
			else if( source[i] >= (char)0x30 && source[i] <= (char)0x39 )
			{
				do
				{
					str_temp += source[i];
					num.totalNum++;
					i++;
					num_total++;
				}
				while ( source[i] >= (char)0x30 && source[i] <= (char)0x39);
				continue;
			}
			// 判断标点符号
			if( PuncSet.find(source.substr(i,1)) != PuncSet.end() )
			{
				punc.totalNum++;
				//characters.push_back( punc );
				i ++;
				num_total++;
				continue;
			}
			else
			{
				junk.totalNum++;
				//characters.push_back(source.substr(i,4));
				i ++;
				num_total++;
				continue;
			}
			++i;
			num_total++;
			continue;
		}
		else
		{
			//cout << "错误代码， Identify.cpp Line 256" << endl;
			i++;
			// 抛出异常的部分
		}
	}
	// 计算比例部分
	this->En.ratio = (double)this->En.totalNum / this->num_total;
	this->Chn.ratio = (double)this->Chn.totalNum / this->num_total;
	this->punc.ratio = (double)this->punc.totalNum / this->num_total;
	this->junk.ratio = (double)this->junk.totalNum / this->num_total;
	this->num.ratio = (double)this->num.totalNum / this->num_total;
	return 0;
}


/*******************************************************/
//
//	分类功能 函数
//
//
/******************************************************/

// 针对一条输入短信，分析短信文本，根据 分布特征、敏感字特征给出可信的分类输出或标签化结构输出
void StatisChar_Identify::process_identify(string & message)
{	
	// 分布特征可疑度计算
		// 利用余弦相似度
	double identify_zero_devide = 0.0;
	identify_zero_devide += this->En.ratio * this->En.ratio	+ this->junk.ratio * this->junk.ratio + this->num.ratio * this->num.ratio +this->punc.ratio * this->punc.ratio;
	double score_A, score_B;
	score_A = (A.En_ratio * this->En.ratio + A.Junk_ratio * this->junk.ratio + A.Num_ratio * this->num.ratio + A.Punc_ratio * this->punc.ratio)
		/ ( sqrt(A.En_ratio * A.En_ratio + A.Junk_ratio * A.Junk_ratio	+ A.Num_ratio * A.Num_ratio + A.Punc_ratio * A.Punc_ratio) );


	score_B = (B.En_ratio * this->En.ratio + B.Junk_ratio * this->junk.ratio + B.Num_ratio * this->num.ratio + B.Punc_ratio * this->punc.ratio)
		/ ( sqrt( B.En_ratio * B.En_ratio + B.Junk_ratio * B.Junk_ratio + B.Num_ratio * B.Num_ratio + B.Punc_ratio * B.Punc_ratio) );
	
	if ( identify_zero_devide != 0.0 )
	{
		score_A /= sqrt(identify_zero_devide);
		score_B /= sqrt(identify_zero_devide);
		//cout << score_A << "\t" << score_B<< endl;
	}
	//cout << (score_A- score_B) / score_A  << endl;
	// 敏感字特征可疑度计算
	score_sentive = 0.0;
	
	for( map<string, Sentive_Base>::iterator iter = SentiveWord.begin(); iter != SentiveWord.end(); iter++ )
	{
		score_sentive +=	iter->second.fre * iter->second.weight;
		
	}
	if ( score_A != 0 )
	{
		this->score_dis = (score_A-score_B)/ (score_A+score_B);
	}
	else
	{
		this->score_dis = 0.0;
	}
	if (score_A > score_B) // 分布特征判为有害 
	{
		this->checkByDistribution = true;
		
	}
	else 
	{
		this->checkByDistribution = false;
		//this->score_dis = ( (score_B-score_A) / score_B );
	}
	if (score_sentive > this->threshold ) // 敏感字特征判为有害 
	{
		this->checkBySentiveWords = true;
		this->score_sentive /= /*(double)this->threshold*/  10;
		//return 0;
	}
	else // 敏感字特征判为无害
	{
		this->checkBySentiveWords = false;
		this->score_sentive /= 10;
		//return 1;
	}
}
int StatisChar_Identify::statistic_score_set(string string_temp,double& score)//change in 11.28
//int StatisChar_Identify::statistic_score_set(string & string_temp,double& score)
{   
	const char*temp = &string_temp[0];
	score = atof( temp );//change in 11.28
	//score = atof( &sting_temp[0] );
	/*
	string consume = "";
	std::istringstream temp(string_temp);
	temp >> consume;
	temp >> score;
	*/
	return 0;
}

int StatisChar_Identify::threshold_set( string  string_temp)//change in 11.28
//int StatisChar_Identify::threshold_set( string & string_temp)
{
	std::istringstream temp(string_temp);
	temp >> this->threshold;
	return 0;
}

// 敏感字表fre清零
int StatisChar_Identify::sentive_fre_clear()
{
	for( map<string, Sentive_Base>::iterator iter = SentiveWord.begin(); iter != SentiveWord.end(); iter++ )
	{
		iter->second.fre = 0;
		//score_sentive = 
	}
	return 0;

}

// 分布特征清零
int StatisChar_Identify::dis_clear() 
{
	junk.ratio =0.0;
	junk.totalNum = 0;
	En.totalNum = 0;
	En.ratio = 0.0;
	punc.totalNum = 0;
	punc.ratio = 0.0;
	Chn.totalNum = 0;
	Chn.ratio = 0.0;
	num.totalNum = 0;
	num.ratio = 0.0;
	num_total = 0;
	
	return 0;
}

int StatisChar_Identify::result_transfer(result & src)
{
	// 测度1
	src.enRatio = this->En.ratio;
	src.puncRatio = this->punc.ratio;
	src.junkRatio = this->junk.ratio;
	src.numRatio = this->num.ratio;

	// 测度2
	src.scoreOfSentiveWords = this->score_sentive;
	src.scoreOfDistribution = this->score_dis;
	// 结果
	src. checkByDistribution = this->checkByDistribution;
	src.checkBySentiveWords = this->checkBySentiveWords;

	return 0;
}

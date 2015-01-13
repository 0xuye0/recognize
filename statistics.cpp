# include "statistics.h"
# include "result.h"

using namespace std;

extern string origin_position;

// ���캯������������,���дʱ������ű�����
void StatisChar_Identify::preparation()
{
	//identify_file_path = (const char*)&"toIdentify.txt";
	
	// ���ݳ�ʼ��
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
	// �����루4����
		// �����ű�����
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
		// �����ֱ�����(��д)
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
			// ���þ��������������дʱ�
	while( getline(file2,name) )
	{
		std::istringstream stream(name);
		stream >> name;
		stream >> content.weight; // ���ĵ�Ƶ������
		content.fre = 0;
		SentiveWord.insert(std::make_pair(name,content));
	}
	file2.close();
		// �ֲ���������
	string temp_merge2 = origin_position + "StaResult.txt";
	const char* statistic_file_path = &temp_merge2[0];
	fstream file3(statistic_file_path, ios::in);
	if ( !file3.is_open() )
	{
		throw 3;
	}
	//file3.open(statistic_file_path);
			// �����ļ���ʽ��ȡ
	double score =0.0;
	string string_temp = "";
	getline( file3,string_temp );// ��ȡ
	// A�� 
			// Ӣ�ķ���`
	//getline( file3, string_temp);
    statistic_score_set(string_temp.substr(5,6),A.En_ratio);
			// ���ַ���
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(12,6),A.Num_ratio);
			// ������
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(19,6),A.Punc_ratio);
			// ��������
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(26,6),A.Junk_ratio);

	getline(file3,string_temp);
	// B��
			// Ӣ�ķ���
	//getline( file3, string_temp);
	//cout<<string_temp<<endl;
	//cout<<string_temp.substr(5,6)<<endl;

	statistic_score_set(string_temp.substr(5,6),B.En_ratio);
			// ���ַ���
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(12,6),B.Num_ratio);
			// ������
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(19,6),B.Punc_ratio);
			// ��������
	//getline( file3, string_temp);
	statistic_score_set(string_temp.substr(26,6),B.Junk_ratio);

	file3.close();
		// ����������
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
// ������ȡ���������๦�ܶ������
int StatisChar_Identify::featureExt(string& source)
{
	// ���GB18030�����ַ��ָ���ͬʱͳ������
	std::string::size_type i = 0;
	std::string::size_type end = source.length()-1;
	string str_temp = "";

	while(i<=end)
	{
		// �����ҵ�4�ֽ��ַ�
		// ����ҵ�2�ֽ��ַ�
		// ��Ե�һ���ֽڵ�ֵ��0x81��0xFE
		if( source[i] >= (char)0x81 && source[i] <= (char)0xfe )
		{
			if ( i+3 <= end)
			{
				// �����Ƿ������ֽ����ݣ����(���ֽ�)�ڶ��ֽ�0x30��0x39 && �������ֽڴ�0x81��0xFE && ���ĸ��ֽڴ�0x30��0x39
				if ( source[i+1] >= (char)0x30 && source[i+1] <= (char)0x39 
					&& source[i+2] >= (char)0x81 && source[i+2] <= (char)0xfe
					&& source[i+3] >= (char)0x30 && source[i+3] <= (char)0x39 )
				{
					num_total++;
					// �Ѿ�ȷ�������ֽ��ַ����ж��Ƿ�Ϊ�����ַ�					
					if( (source[i] == (char)0x81 ||  source[i] == (char)0x82)	// ���CJK����A
						||  (source[i] >= (char)0x95 && source[i] <= (char)0x98)) // ���CJK����B
					{
						// �����ַ�����
						Chn.totalNum++;
						//characters.push_back(source.substr(i,4));
						
						// ���дʴ���
						str_temp = source.substr(i,4);
						i += 4;
						if( SentiveWord.find(str_temp) != SentiveWord.end() )
						{
							SentiveWord[str_temp].fre++;							
						}				
						continue;
					}
					else // �������ַ�
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
							junk.totalNum++;// ������Ϊ���Ȳ��������ַ����ֲ������ڱ������ļ��е��ַ�Ϊ�����ַ�
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
					// ����˫�ֽ������ַ��������
					if ( (source[i] >= (char)0xb0 && source[i] <= (char)0xf7
						&& source[i+1] >= (char)0xa1 && source[i+1] <= (char)0xfe) // 6763������

						||( (source[i] >= (char)0x81 && source[i] <= (char)0xa0)// ���ֽ�
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) ) )// 6080������

						||( source[i] >= (char)0xaa && source[i] <= (char)0xfe
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xa0) ) ) ) // 8160������
					{
						Chn.totalNum++;
						//characters.push_back(source.substr(i,2));
						
						// ���дʴ���
						str_temp = source.substr(i,2);
						i += 2;
						if( SentiveWord.find(str_temp) != SentiveWord.end() )
						{
							SentiveWord[str_temp].fre++;
							/*
							// �жϸ����д��Ƿ��Ѿ���SentiveWord_result���Ѿ�����
							if ( SentiveWord_result.find(str_temp) != SentiveWord_result.end() )
							{
							// �Ѿ�����
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
					else// �������ַ�
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
				}// ˫�ֽڿ��� ѭ������
			}
			else
			{				
				i++;				
			}
		}// 2 4�ֽڿ��� ѭ������
		// ����ǵ��ֽ��ַ���Ŀǰ���ڵ���������ַ���Ӧ����δ���
		else if( source[i] >= (char)0x0 && source[i] <= (char)0x7f )
		{
			str_temp = "";
			// ����Ӣ���ַ��ĺϲ����� 61-7a�� 41-5a
				// Ӣ���ַ��Ƿ���Ҫ���д�Сдת��
			if( (source[i] >= (char)0x61 && source[i] <= (char)0x7a)
				|| (source[i]>= (char)0x41 && source[i] <= (char)0x5a) )
			{
				do
				{
					// ת��ΪСд���ٴ���
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
				// ���д��ж�
				if( SentiveWord.find(str_temp) != SentiveWord.end() )
				{
					SentiveWord[str_temp].fre++;
					/*
					// �жϸ����д��Ƿ��Ѿ���SentiveWord_result���Ѿ�����
					if ( SentiveWord_result.find(str_temp) != SentiveWord_result.end() )
					{
						// �Ѿ�����
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
			// �������ֵĺϲ����� 30-39
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
			// �жϱ�����
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
			//cout << "������룬 Identify.cpp Line 256" << endl;
			i++;
			// �׳��쳣�Ĳ���
		}
	}
	// �����������
	this->En.ratio = (double)this->En.totalNum / this->num_total;
	this->Chn.ratio = (double)this->Chn.totalNum / this->num_total;
	this->punc.ratio = (double)this->punc.totalNum / this->num_total;
	this->junk.ratio = (double)this->junk.totalNum / this->num_total;
	this->num.ratio = (double)this->num.totalNum / this->num_total;
	return 0;
}


/*******************************************************/
//
//	���๦�� ����
//
//
/******************************************************/

// ���һ��������ţ����������ı������� �ֲ������������������������ŵķ���������ǩ���ṹ���
void StatisChar_Identify::process_identify(string & message)
{	
	// �ֲ��������ɶȼ���
		// �����������ƶ�
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
	// �������������ɶȼ���
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
	if (score_A > score_B) // �ֲ�������Ϊ�к� 
	{
		this->checkByDistribution = true;
		
	}
	else 
	{
		this->checkByDistribution = false;
		//this->score_dis = ( (score_B-score_A) / score_B );
	}
	if (score_sentive > this->threshold ) // ������������Ϊ�к� 
	{
		this->checkBySentiveWords = true;
		this->score_sentive /= /*(double)this->threshold*/  10;
		//return 0;
	}
	else // ������������Ϊ�޺�
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

// �����ֱ�fre����
int StatisChar_Identify::sentive_fre_clear()
{
	for( map<string, Sentive_Base>::iterator iter = SentiveWord.begin(); iter != SentiveWord.end(); iter++ )
	{
		iter->second.fre = 0;
		//score_sentive = 
	}
	return 0;

}

// �ֲ���������
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
	// ���1
	src.enRatio = this->En.ratio;
	src.puncRatio = this->punc.ratio;
	src.junkRatio = this->junk.ratio;
	src.numRatio = this->num.ratio;

	// ���2
	src.scoreOfSentiveWords = this->score_sentive;
	src.scoreOfDistribution = this->score_dis;
	// ���
	src. checkByDistribution = this->checkByDistribution;
	src.checkBySentiveWords = this->checkBySentiveWords;

	return 0;
}

#include "SmsExt.h"
#include <cmath>

//数据库交互版的构造函数
SmsClassifyExtraction::SmsClassifyExtraction()
{
	init();
}
//析构函数
SmsClassifyExtraction::~SmsClassifyExtraction()
{

}
//数据库or类内初始化关键词库
int SmsClassifyExtraction::init()
{
	LoadPunc();//加载标点符号词典
	return 0;
}
/*
//构造函数
SmsClassifyExtraction::SmsClassifyExtraction
{
	this->start( map<int,char_index_contents> char_index_n, map<int,vector<int> > word_index_n, map<int,float> wordvalue_index_n );

}
*/
void SmsClassifyExtraction::start( map<int,char_index_contents> char_index_n, map<int,vector<int> > word_index_n, map<int,float> wordvalue_index_n)
{
	char_index = char_index_n;


	word_index = word_index_n;
	wordvalue_index=wordvalue_index_n;
	LoadPunc();
}



/***************************短信标注子模块******************************/
//短信标注子模块
void SmsClassifyExtraction::OriginalSmsLable(const std::string &smsoriginal)
{
	std::vector< std::string > characters;
	SmsCut(smsoriginal,characters);
	GetFinalKeyword(characters);
}
// 短信逐字分解（基于GB18030版本）


//短信逐字分解
int SmsClassifyExtraction::SmsCut(const std::string &source,std::vector<std::string> &characters)
{
	std::string::size_type i = 0;
	std::string::size_type end = source.length()-1;

	string str_temp = "";

	while( i <= end )
	{
		if( source[i] >= (char)0x81 && source[i] <= (char)0xfe )
		{
			// 考虑是否是四字节内容，针对(四字节)第二字节0x30到0x39 && 第三个字节从0x81到0xFE && 第四个字节从0x30到0x39
			if ( i+3 <= end)
			{
				if( source[i+1] >= (char)0x30 && source[i+1] <= (char)0x39 
					&& source[i+2] >= (char)0x81 && source[i+2] <= (char)0xfe
					&& source[i+3] >= (char)0x30 && source[i+3] <= (char)0x39 )
				{
					// 已经确定是四字节字符，判断是否为中文字符					
					if( (source[i] == (char)0x81 ||  source[i] == (char)0x82)	// 针对CJK扩充A
						||  (source[i] >= (char)0x95 && source[i] <= (char)0x98)) // 针对CJK扩充B
					{
						characters.push_back(source.substr(i,4));
					}
						i+=4;
						continue;					
				}
			}
			if ( i+1 <= end )
			{
				if((source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
					|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) )
				{
					if ( (source[i] >= (char)0xb0 && source[i] <= (char)0xf7
						&& source[i+1] >= (char)0xa1 && source[i+1] <= (char)0xfe) // 6763个汉字

						||( (source[i] >= (char)0x81 && source[i] <= (char)0xa0)// 高字节
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) ) )// 6080个汉字

						||( source[i] >= (char)0xaa && source[i] <= (char)0xfe
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xa0) ) ) ) // 8160个汉字
					{
						// 中文符号
						characters.push_back(source.substr(i,2));
					}	
						i+=2;
						continue;				
				}// 双字节考虑 循环结束
			}
		}// 2 4字节考虑 循环结束
		// 最后考虑单字节字符（目前存在的其他情况字符，应该如何处理） 不存储
		else if( source[i] >= (char)0x0 && source[i] <= (char)0x7f )
		{
			// 英文字母合并
			str_temp = "";
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
					i++;
				}
				while( i <= end
					&& ((source[i] >= (char)0x61 && source[i] <= (char)0x7a)
					|| (source[i]>= (char)0x41 && source[i] <= (char)0x5a)) );
				
				characters.push_back( str_temp);
				continue;
			}
			// 数字存储
			else if( source[i] >= (char)0x30 && source[i] <= (char)0x39 )
			{
				characters.push_back( source.substr(i,1) );
				i++;
				continue;
			}
		
		}
		i++;
	}
	return 1;
}

void SmsClassifyExtraction::LoadPunc() 
{
	string temp_merge = origin_position + "Punc.txt";
	const char* file_punc = &temp_merge[0];
	std::ifstream infile(file_punc,std::ifstream::in);
	std::string content(" ");
	while(std::getline(infile,content))
	{
		PuncSet.insert(content);
	}
}

// SmsLabel中的second属性，虽然仍为vector<int>但意义发生变化
	// 不再记录对应word_index中的位置，而是记录当前关键字的短信绝对位置
int SmsClassifyExtraction::GetFinalKeyword(std::vector<std::string> &characters)
{
	std::map< int,int > keywordInfo;

	// 两个迭代指示器
	std::map< int, char_index_contents >::iterator find_char_index;
	std::map< int, std::vector< int > >::iterator find_SmsLabel;

	int i=1;//label the posion of a character in a sms
			// 注意：基于1的索引
	for(std::vector<std::string>::iterator iter_char = characters.begin();iter_char != characters.end();++iter_char,++i)
	{
		bool jump_flag = false;

		map<int, char_index_contents>::iterator char_index_iterator,char_index_iterator_end;
		char_index_iterator_end = char_index.end();
		
		for( char_index_iterator = char_index.begin(); char_index_iterator != char_index_iterator_end; char_index_iterator++) 
//			int char_index_i = 1; char_index_i < char_index.count(); char_index_i++)
		{
			
			for(int ii = 0; ii < (*char_index_iterator).second.char_contents.size(); ii ++)
			{
				
				if( (*char_index_iterator).second.char_contents[ii] == (*iter_char) )
				{	
					// 装入SmsLabel过程
					// 由于一条短信对应一个SmsLabel，因此其中的second属性——位置信息不存在重复的情况，, mk这里忽略验重
					find_SmsLabel = SmsLabel.find( (*char_index_iterator).first );
					
					// 判断该字符是否已经存在于SmsLabel中
					if ( find_SmsLabel == SmsLabel.end() )// 不存在的情况
					{
						if ( (*char_index_iterator).second.char_contents[0] == (*iter_char) )
						{
							std::pair<int, vector<int> >  p( (*char_index_iterator).first,vector<int>(1,(-1)*i) );
							SmsLabel.insert(p);
						}
						else
						{
							std::pair<int, vector<int> >  p( (*char_index_iterator).first,vector<int>(1,i) );
							SmsLabel.insert(p);
						}
						
					}
					else
					{
						if ( (*char_index_iterator).second.char_contents[0] == (*iter_char) )
							(*find_SmsLabel).second.push_back((-1)*i);
						else
							(*find_SmsLabel).second.push_back(i);
					}
						

					// 连续跳出2层循环结构
					//jump_flag = true;
					break;
				}
			}
			/*
			if ( jump_flag)
				break;
				*/
		}
	}
	// 利用SmsLabel生成KeyWord过程
		// 首先，遍历SmsLabel，find_SmsLabel复用
	map<int, vector<int> >::iterator SmsLabel_iter_end = SmsLabel.end();
	for(find_SmsLabel = SmsLabel.begin(); find_SmsLabel != SmsLabel_iter_end; find_SmsLabel++)
	{
		// 利用SmsLabel的键值对应到char_index上，找到相关的关键词索引
		int temp_i = 0;
		map< int, std::vector<std::vector<int> > >::iterator iter_KeyWord;
		// SmsLabel对应的word_index规模由char_index的second属性中word_label的size决定
		//temp_i = char_index[ find_SmsLabel->first ].word_label.size(); 
		
		for(; temp_i < char_index[ find_SmsLabel->first ].word_label.size(); temp_i++) // 涉及多少关键词
		{
			// 判断KeyWord是否已经存在
			
				// temp 记录当前关键词序号，temp_i 也就是其对应的第几个数目（基于0）
			int temp = char_index[ find_SmsLabel->first ].word_label[temp_i];// 没找到的情况，temp应该赋值为多少
				// KeyWord查找 temp 序号的关键词是否已经存在
			iter_KeyWord = KeyWord.find( char_index[ find_SmsLabel->first].word_label[temp_i]);
			if ( iter_KeyWord == KeyWord.end() )
			{
				// 若不存在，利用SmsLabel中的first找到对应的关键词索引（key），同时word_index的对应索引的second(实际存储涉及关键字的序号)创建vector<vector<int>>
				//std::vector<int> temp_vector_int;
				// 创建条目
				KeyWord.insert( std::make_pair( temp, word_index[ char_index[ find_SmsLabel->first].word_label[temp_i] ].size() ) );// 这部分内容，单独存储int!!（过长表达式）
				// 填充条目内容，将SmsLabel中的second内容全部填充
				// 在word_index中找到应填充第几项
				int temp_number = 0;
				for(; temp_number < word_index[ char_index[ find_SmsLabel->first ].word_label[temp_i] ].size(); temp_number++ )
				{
					// 判断是否是对应条目
					if ( word_index[ char_index[ find_SmsLabel->first ].word_label[temp_i] ][temp_number] == find_SmsLabel->first )
						// 填充代码
					{
						// 注意拷贝方式
						KeyWord[ char_index[ find_SmsLabel->first].word_label[temp_i] ][temp_number] = find_SmsLabel->second;
						break;
					}
				}
			}
			else
			{
				// 已经存在

				// 利用iter_KeyWord
				int temp_number_1 = 0;
				for( ; temp_number_1 < word_index[iter_KeyWord->first].size(); temp_number_1++ )// 得到该关键词与多少关键字相关
				{
					if ( word_index[ iter_KeyWord->first][temp_number_1] == find_SmsLabel->first )
					{
						// 注意拷贝方式
						iter_KeyWord->second[ temp_number_1 ] = find_SmsLabel->second;
						break;
					}
				}
			// makepair的第二个属性：在由int转换为vector的意义，要注意！
			//KeyWord.insert(std::make_pair( char_index[ *find_SmsLabel->first ].word_label[temp_i], word_index[ char_index[ *find_SmsLabel->first ].word_label[temp_i] ].size()) );
			}
		}
	
	}
	
	return 1;
}
	
/*************************标注结果处理子模块***************************/
//记录当前关键词中全部关键字在短信中的位置
//map< string, vector<int> > SmsLabel;
//map< int,vector<string> > KeyWord;
int SmsClassifyExtraction::GenerateKeyPosInSms(vector< vector<int> >& KeyPosition,const vector<string>& SingleKey)
{
	vector<string>::const_iterator Keyit;
	map< string, vector<int> >::iterator Lableit;
//	for(Keyit=SingleKey.begin();Keyit!=SingleKey.end();Keyit++)
//	{
//		Lableit=SmsLabel.find(*Keyit);~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		if(Lableit!=SmsLabel.end())
//		{KeyPosition.push_back((*Lableit).second);}
//		else
//		{std::cout<<"不存在该关键字信息error"<<std::endl;}
//	}
	return 1;
}

//生成待处理的可能排列序列
int SmsClassifyExtraction::GenerateSequence(vector< vector<int> >& KeyPosition,vector< vector<int> >& Sequence)
{
	int num = KeyPosition.size() ; 
	vector<int> result(num , 0) ;
	while(true)
	{
		vector<int> temp;
		for(int i = 0 ; i <= num-1 ; ++i)
			temp.push_back(KeyPosition[i][result[i]]);
		Sequence.push_back(temp);
		int k = num - 1 ; 
		while(k>=0)
		{
			if(result[k] < KeyPosition[k].size()-1){
				++result[k] ; 
				break ;
			}else
			{
				result[k] = 0 ; 
				k-- ; 
			}
		}
		if(k<0)
			break ; 
	
	}
	return 1;
}

//检测序列是否等差
//输入：序列vector<int>
//输出：若为等差序列返回1，否则返回0；
int SmsClassifyExtraction::ArithmeticSequenceDetect(const vector<int>& Sequence)
{
	int temp;
	temp=Sequence[1]-Sequence[0];
	int i;
	for(i=1;i < Sequence.size();i++)
	{
		if(Sequence[i]-Sequence[i-1]!=temp)
		{return -1;}
	}
	return temp;
}

//单个关键词处理
//ProcessOneKeyword处理单个关键词，函数内部：先调用int GenerateKeyPosInSms(vector<vector<int>>& KeyPosition,vector<string> SingleKey)获得
//当前关键词中全部关键字在短信中的位置，再调用int GenerateSequence(vector<vector<int>> KeyPosition,vector< vector<int> >& Sequence)生成可能排列序列
//对每一种可能序列调用int ArithmeticSequenceDetect(vector<int> Sequence)判断该序列是否为等差数列
//若为等差数列，将结果写入map< string, vector<int>> ResultKeyWord

// CurrentKey标记
// keywordvalue标记
int SmsClassifyExtraction::ProcessOneKeyword( map< int,vector< vector<int> > >::iterator& it,float keywordvalue, const string& key_name)
{

	typedef multimap<int, KeySearch>::value_type	Val;
	vector< vector<int> > KeyPosition;
	vector< vector<int> > Sequence;
	
	GenerateSequence( (*it).second,Sequence);

	// 结果封装，以便传递，写入ResultKeyWord
		// 由于Sequence可能生成多个结果，因此可能生成多个KeySearch,最终生成多个ResultKeyWord
	for( int ii_temp = 0; ii_temp < Sequence.size(); ii_temp++)
	{	if (  !OrderSequenceDetect(Sequence[ii_temp]) )
		{
			// 非法信息，直接忽略
			continue;
		}
		KeySearch temp_result;
		temp_result.keyname = key_name;
		temp_result.keywordvalue = keywordvalue;
		temp_result.position = Sequence[ii_temp];
		ResultKeyWord.insert( Val( (*it).first, temp_result ) );
	}
	return 1;
}

//map< string, vector<int> > SmsLabel;
//map< int,vector<string> > KeyWord;
//std::map<int,float> wordvalue_index;
int SmsClassifyExtraction::LabledSmsAnlysis()
{
	
	std::map<int,float>::iterator valueit;
	map< int,vector< vector<int> > >::iterator it;
	for(it=KeyWord.begin();it!=KeyWord.end();it++)
	{
		valueit=wordvalue_index.find((*it).first);//获取关键词权值
		if(valueit!=wordvalue_index.end())
		{
			// 判断KeyWord结构是否全部填充
				// 否则视为这种关键词短信中并不存在
			bool check = false;
			for(int num_temp = 0; num_temp < (*it).second.size(); num_temp++)
			{
				if( !(*it).second[num_temp].size() )// 寻找null项目
				{
					check = true;
					break;
				}
			}
			if ( check )
				continue;
			else
			{
				string keyName = "";
				for(int j_temp = 0; j_temp < word_index[(*it).first].size(); j_temp++ )
				{
					keyName += char_index[ word_index[(*it).first][j_temp] ].char_contents[0];
				}
				ProcessOneKeyword( it,(*valueit).second, keyName );//关键词标注信息及关键词权值作为参数
			}
		}
		else
		{
			std::cout<<"获取关键词权值错误"<<std::endl;
			return 0;
		}
	}
	return 1;
}

int SmsClassifyExtraction::OrderSequenceDetect(const std::vector<int> &Sequence)
{
	int temp;
	temp=abs(Sequence[1])-abs(Sequence[0]);
	int others;
	int i;
	if(Sequence.size()>2)
	{
		for(i=2;i < Sequence.size();i++)
		{
			others=abs(Sequence[i])-abs(Sequence[i-1]);
			if((others>0&&temp<0)||(others<0&&temp>0))
			{return 0;}
		}
		return 1;
	}
	else if(Sequence.size()==2)
	{
		return 1;
	}
	else
	{return 0;}
}

int SmsClassifyExtraction::SmsProcess(const std::string &smsoriginal)
{
	SmsLabel.clear();
	KeyWord.clear();
	ResultKeyWord.clear();
	OriginalSmsLable(smsoriginal);
	LabledSmsAnlysis();
	return 1; 
}

/////////特征抽取类对外处理接口函数
int SmsClassifyExtraction::process(MessageStructure & msg)
{
	SmsProcess(msg.msg_content);
	msg.ResultKeyWord=ResultKeyWord;// 显示的特征抽取过程的结果输出
	return 0;
}
/*
int SmsClassifyExtraction::PrintResult(const std::string &smsoriginal)
{
	multimap<int,KeySearch>::iterator it;
	cout<<smsoriginal<<"	";
	for(it=ResultKeyWord.begin();it!=ResultKeyWord.end();it++)
	{
		cout<<(*it).first<<"  " << (*it).second.keyname<< "  ";
		vector<int>::iterator itv;
		for(itv=(*it).second.position.begin();itv!=(*it).second.position.end();itv++)
		{cout<<(*itv)<<" ";}
		cout<<"	"<<"		"<<(*it).second.keywordvalue<<"	";
	}
	cout<<endl;
	return 1;
}
*/

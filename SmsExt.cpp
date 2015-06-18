#include "SmsExt.h"
#include <cmath>

//���ݿ⽻����Ĺ��캯��
SmsClassifyExtraction::SmsClassifyExtraction()
{
	init();
}
//��������
SmsClassifyExtraction::~SmsClassifyExtraction()
{

}
//���ݿ�or���ڳ�ʼ���ؼ��ʿ�
int SmsClassifyExtraction::init()
{
	LoadPunc();//���ر����Ŵʵ�
	return 0;
}
/*
//���캯��
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



/***************************���ű�ע��ģ��******************************/
//���ű�ע��ģ��
void SmsClassifyExtraction::OriginalSmsLable(const std::string &smsoriginal)
{
	std::vector< std::string > characters;
	SmsCut(smsoriginal,characters);
	GetFinalKeyword(characters);
}
// �������ַֽ⣨����GB18030�汾��


//�������ַֽ�
int SmsClassifyExtraction::SmsCut(const std::string &source,std::vector<std::string> &characters)
{
	std::string::size_type i = 0;
	std::string::size_type end = source.length()-1;

	string str_temp = "";

	while( i <= end )
	{
		if( source[i] >= (char)0x81 && source[i] <= (char)0xfe )
		{
			// �����Ƿ������ֽ����ݣ����(���ֽ�)�ڶ��ֽ�0x30��0x39 && �������ֽڴ�0x81��0xFE && ���ĸ��ֽڴ�0x30��0x39
			if ( i+3 <= end)
			{
				if( source[i+1] >= (char)0x30 && source[i+1] <= (char)0x39 
					&& source[i+2] >= (char)0x81 && source[i+2] <= (char)0xfe
					&& source[i+3] >= (char)0x30 && source[i+3] <= (char)0x39 )
				{
					// �Ѿ�ȷ�������ֽ��ַ����ж��Ƿ�Ϊ�����ַ�					
					if( (source[i] == (char)0x81 ||  source[i] == (char)0x82)	// ���CJK����A
						||  (source[i] >= (char)0x95 && source[i] <= (char)0x98)) // ���CJK����B
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
						&& source[i+1] >= (char)0xa1 && source[i+1] <= (char)0xfe) // 6763������

						||( (source[i] >= (char)0x81 && source[i] <= (char)0xa0)// ���ֽ�
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) ) )// 6080������

						||( source[i] >= (char)0xaa && source[i] <= (char)0xfe
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xa0) ) ) ) // 8160������
					{
						// ���ķ���
						characters.push_back(source.substr(i,2));
					}	
						i+=2;
						continue;				
				}// ˫�ֽڿ��� ѭ������
			}
		}// 2 4�ֽڿ��� ѭ������
		// ����ǵ��ֽ��ַ���Ŀǰ���ڵ���������ַ���Ӧ����δ��� ���洢
		else if( source[i] >= (char)0x0 && source[i] <= (char)0x7f )
		{
			// Ӣ����ĸ�ϲ�
			str_temp = "";
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
					i++;
				}
				while( i <= end
					&& ((source[i] >= (char)0x61 && source[i] <= (char)0x7a)
					|| (source[i]>= (char)0x41 && source[i] <= (char)0x5a)) );
				
				characters.push_back( str_temp);
				continue;
			}
			// ���ִ洢
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

// SmsLabel�е�second���ԣ���Ȼ��Ϊvector<int>�����巢���仯
	// ���ټ�¼��Ӧword_index�е�λ�ã����Ǽ�¼��ǰ�ؼ��ֵĶ��ž���λ��
int SmsClassifyExtraction::GetFinalKeyword(std::vector<std::string> &characters)
{
	std::map< int,int > keywordInfo;

	// ��������ָʾ��
	std::map< int, char_index_contents >::iterator find_char_index;
	std::map< int, std::vector< int > >::iterator find_SmsLabel;

	int i=1;//label the posion of a character in a sms
			// ע�⣺����1������
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
					// װ��SmsLabel����
					// ����һ�����Ŷ�Ӧһ��SmsLabel��������е�second���ԡ���λ����Ϣ�������ظ��������, mk�����������
					find_SmsLabel = SmsLabel.find( (*char_index_iterator).first );
					
					// �жϸ��ַ��Ƿ��Ѿ�������SmsLabel��
					if ( find_SmsLabel == SmsLabel.end() )// �����ڵ����
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
						

					// ��������2��ѭ���ṹ
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
	// ����SmsLabel����KeyWord����
		// ���ȣ�����SmsLabel��find_SmsLabel����
	map<int, vector<int> >::iterator SmsLabel_iter_end = SmsLabel.end();
	for(find_SmsLabel = SmsLabel.begin(); find_SmsLabel != SmsLabel_iter_end; find_SmsLabel++)
	{
		// ����SmsLabel�ļ�ֵ��Ӧ��char_index�ϣ��ҵ���صĹؼ�������
		int temp_i = 0;
		map< int, std::vector<std::vector<int> > >::iterator iter_KeyWord;
		// SmsLabel��Ӧ��word_index��ģ��char_index��second������word_label��size����
		//temp_i = char_index[ find_SmsLabel->first ].word_label.size(); 
		
		for(; temp_i < char_index[ find_SmsLabel->first ].word_label.size(); temp_i++) // �漰���ٹؼ���
		{
			// �ж�KeyWord�Ƿ��Ѿ�����
			
				// temp ��¼��ǰ�ؼ�����ţ�temp_i Ҳ�������Ӧ�ĵڼ�����Ŀ������0��
			int temp = char_index[ find_SmsLabel->first ].word_label[temp_i];// û�ҵ��������tempӦ�ø�ֵΪ����
				// KeyWord���� temp ��ŵĹؼ����Ƿ��Ѿ�����
			iter_KeyWord = KeyWord.find( char_index[ find_SmsLabel->first].word_label[temp_i]);
			if ( iter_KeyWord == KeyWord.end() )
			{
				// �������ڣ�����SmsLabel�е�first�ҵ���Ӧ�Ĺؼ���������key����ͬʱword_index�Ķ�Ӧ������second(ʵ�ʴ洢�漰�ؼ��ֵ����)����vector<vector<int>>
				//std::vector<int> temp_vector_int;
				// ������Ŀ
				KeyWord.insert( std::make_pair( temp, word_index[ char_index[ find_SmsLabel->first].word_label[temp_i] ].size() ) );// �ⲿ�����ݣ������洢int!!���������ʽ��
				// �����Ŀ���ݣ���SmsLabel�е�second����ȫ�����
				// ��word_index���ҵ�Ӧ���ڼ���
				int temp_number = 0;
				for(; temp_number < word_index[ char_index[ find_SmsLabel->first ].word_label[temp_i] ].size(); temp_number++ )
				{
					// �ж��Ƿ��Ƕ�Ӧ��Ŀ
					if ( word_index[ char_index[ find_SmsLabel->first ].word_label[temp_i] ][temp_number] == find_SmsLabel->first )
						// ������
					{
						// ע�⿽����ʽ
						KeyWord[ char_index[ find_SmsLabel->first].word_label[temp_i] ][temp_number] = find_SmsLabel->second;
						break;
					}
				}
			}
			else
			{
				// �Ѿ�����

				// ����iter_KeyWord
				int temp_number_1 = 0;
				for( ; temp_number_1 < word_index[iter_KeyWord->first].size(); temp_number_1++ )// �õ��ùؼ�������ٹؼ������
				{
					if ( word_index[ iter_KeyWord->first][temp_number_1] == find_SmsLabel->first )
					{
						// ע�⿽����ʽ
						iter_KeyWord->second[ temp_number_1 ] = find_SmsLabel->second;
						break;
					}
				}
			// makepair�ĵڶ������ԣ�����intת��Ϊvector�����壬Ҫע�⣡
			//KeyWord.insert(std::make_pair( char_index[ *find_SmsLabel->first ].word_label[temp_i], word_index[ char_index[ *find_SmsLabel->first ].word_label[temp_i] ].size()) );
			}
		}
	
	}
	
	return 1;
}
	
/*************************��ע���������ģ��***************************/
//��¼��ǰ�ؼ�����ȫ���ؼ����ڶ����е�λ��
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
//		{std::cout<<"�����ڸùؼ�����Ϣerror"<<std::endl;}
//	}
	return 1;
}

//���ɴ�����Ŀ�����������
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

//��������Ƿ�Ȳ�
//���룺����vector<int>
//�������Ϊ�Ȳ����з���1�����򷵻�0��
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

//�����ؼ��ʴ���
//ProcessOneKeyword�������ؼ��ʣ������ڲ����ȵ���int GenerateKeyPosInSms(vector<vector<int>>& KeyPosition,vector<string> SingleKey)���
//��ǰ�ؼ�����ȫ���ؼ����ڶ����е�λ�ã��ٵ���int GenerateSequence(vector<vector<int>> KeyPosition,vector< vector<int> >& Sequence)���ɿ�����������
//��ÿһ�ֿ������е���int ArithmeticSequenceDetect(vector<int> Sequence)�жϸ������Ƿ�Ϊ�Ȳ�����
//��Ϊ�Ȳ����У������д��map< string, vector<int>> ResultKeyWord

// CurrentKey���
// keywordvalue���
int SmsClassifyExtraction::ProcessOneKeyword( map< int,vector< vector<int> > >::iterator& it,float keywordvalue, const string& key_name)
{

	typedef multimap<int, KeySearch>::value_type	Val;
	vector< vector<int> > KeyPosition;
	vector< vector<int> > Sequence;
	
	GenerateSequence( (*it).second,Sequence);

	// �����װ���Ա㴫�ݣ�д��ResultKeyWord
		// ����Sequence�������ɶ���������˿������ɶ��KeySearch,�������ɶ��ResultKeyWord
	for( int ii_temp = 0; ii_temp < Sequence.size(); ii_temp++)
	{	if (  !OrderSequenceDetect(Sequence[ii_temp]) )
		{
			// �Ƿ���Ϣ��ֱ�Ӻ���
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
		valueit=wordvalue_index.find((*it).first);//��ȡ�ؼ���Ȩֵ
		if(valueit!=wordvalue_index.end())
		{
			// �ж�KeyWord�ṹ�Ƿ�ȫ�����
				// ������Ϊ���ֹؼ��ʶ����в�������
			bool check = false;
			for(int num_temp = 0; num_temp < (*it).second.size(); num_temp++)
			{
				if( !(*it).second[num_temp].size() )// Ѱ��null��Ŀ
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
				ProcessOneKeyword( it,(*valueit).second, keyName );//�ؼ��ʱ�ע��Ϣ���ؼ���Ȩֵ��Ϊ����
			}
		}
		else
		{
			std::cout<<"��ȡ�ؼ���Ȩֵ����"<<std::endl;
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

/////////������ȡ����⴦��ӿں���
int SmsClassifyExtraction::process(MessageStructure & msg)
{
	SmsProcess(msg.msg_content);
	msg.ResultKeyWord=ResultKeyWord;// ��ʾ��������ȡ���̵Ľ�����
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

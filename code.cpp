# include "head.h"
# include <fstream>
# include <sstream>
//
# include <iostream>
using namespace std;
//

int operator<(myString a, myString b)
{
	for( int i = 0; i < a.content.length() && i < b.content.length(); i++ )
	{
		if( a.content[i] < b.content[i] )
			return 1;
		else if ( a.content[i] > b.content[i])
			return 0;
	}
	if (a.content.length() < b.content.length() )
		return 1;
	else if ( a.content.length() > b.content.length() )
		return 0;
	return 0;
}
int operator<(string a, string b)
{
	for( int i = 0; i < a.length() && i < b.length(); i++ )
	{
		if( a[i] < b[i] )
			return -1;
		else if ( a[i] > b[i])
			return 1;
	}
	if (a.length() < b.length() )
		return -1;
	else if ( a.length() > b.length() )
		return 1;
	return 0;
}

void message_classifier::keyStructure_generator(MessageStructure& msg)
{
	ItemInCB* t1 = this->codeBook;
	int* pt = new int[4];
	for(int i = 0; i < 4;i++)
	{
		pt[i] = -1;
	}
	int NO_CB = 0;
	for( ;t1 != NULL;t1=t1->next, NO_CB++)// Item Search
	{
		int No_of_Unit_in_Item = 0;
		string Item_Content = "";
		/*
		for( UnitInCB* temp_unit = t1->words[No_of_Unit_in_Item]; temp_unit != NULL; temp_unit =t1->words[No_of_Unit_in_Item] )
		{
			Item_Content += temp_unit->content;
			pt[No_of_Unit_in_Item] = temp_unit->substitute[0];
			No_of_Unit_in_Item++;
		}
		*/
		vector<int> i,j,m,n;
		while( true)
		{
			UnitInCB* pointer = t1->words[0];// fill in vector i
			{
				No_of_Unit_in_Item = 1;
				Item_Content += pointer->content;
				//if (pointer->hierachy[0] > 1)// hirachary of origin Unit, fill them in
				if (pointer->hierachy != NULL)// hirachary of origin Unit, fill them in				
					for(int a = 1; a < pointer->hierachy[0];a++)
						i.push_back(pointer->hierachy[a]);
				
				if( pointer->substitute != NULL)
				{
					UnitInCB* p = pointer->substitute[0];
					for(int num_of_sub = 0; pointer != NULL; num_of_sub++ )// hirachary of substitute Unit, fill them in
					{
						p = pointer->substitute[num_of_sub];
						if ( p== NULL)
							break;
						if ( p->hierachy != NULL)
							for(int a = 1; a < p->hierachy[0];a++)
								i.push_back(p->hierachy[a] * -1);
					}
				}
			}
			pointer = t1->words[1];// // same way, fill in vector j
			if ( pointer != NULL)
			{
				No_of_Unit_in_Item = 2;
				Item_Content += pointer->content;
				if (pointer->hierachy != NULL)// hirachary of origin Unit, fill them in
					for(int a = 1; a < pointer->hierachy[0];a++)
						j.push_back(pointer->hierachy[a]);
				
				if( pointer->substitute != NULL)
				{
					UnitInCB* p = pointer->substitute[0];

					for(int num_of_sub = 0; pointer != NULL; num_of_sub++ )// hirachary of substitute Unit, fill them in
					{
						p = pointer->substitute[num_of_sub];
						if (p==NULL)
							break;
						if ( p->hierachy != NULL)
							for(int a = 0; a < p->hierachy[0];a++)
								j.push_back(p->hierachy[a] * -1);
					}
				}
			}
			else
				break;
			pointer = t1->words[2];// // same way, fill in vector m
			if ( pointer != NULL)
			{
				No_of_Unit_in_Item = 3;
				Item_Content += pointer->content;
				if (pointer->hierachy != NULL)// hirachary of origin Unit, fill them in
					for(int a = 1; a < pointer->hierachy[0];a++)
						m.push_back(pointer->hierachy[a]);
				
				if( pointer->substitute != NULL)
				{
					UnitInCB* p = pointer->substitute[0];

					for(int num_of_sub = 0; pointer != NULL; num_of_sub++ )// hirachary of substitute Unit, fill them in
					{
						p = pointer->substitute[num_of_sub];
						if ( p!=NULL)
							break;
						if ( p->hierachy != NULL)
							for(int a = 0; a < p->hierachy[0];a++)
								m.push_back(p->hierachy[a] * -1);
					}
				}
			}
			else
				break;
			pointer = t1->words[3];// // same way, fill in vector n
			if ( pointer != NULL)
			{
				if (pointer->hierachy != NULL)// hirachary of origin Unit, fill them in
					for(int a = 1; a < pointer->hierachy[0];a++)
						n.push_back(pointer->hierachy[a]);
				else
					break;
				if( pointer->substitute != NULL)
				{
					No_of_Unit_in_Item = 4;
					Item_Content += pointer->content;
					UnitInCB* p = pointer->substitute[0];
					for(int num_of_sub = 0; pointer != NULL; num_of_sub++ )// hirachary of substitute Unit, fill them in
					{
						p = pointer->substitute[num_of_sub];
						if ( p==NULL)
							break;
						if ( p->hierachy != NULL)
							for(int a = 0; a < p->hierachy[0];a++)
								n.push_back(p->hierachy[a] * -1);
					}
				}
			}
			else 
				break;
			break;
		}
		// i j m n to become the sequence of keyword, No_of_Unit_in_Item
		if ( i.empty() )
			continue;
		switch( No_of_Unit_in_Item)
		{
		case 2:
			{
				if (j.empty())
					break;
				for(vector<int>::iterator it1 = i.begin(); it1 < i.end(); it1++)
					for(vector<int>::iterator it2 = j.begin(); it2 < j.end(); it2++)
					{
						KeySearch itemKS;
						itemKS.keyname = Item_Content;
						itemKS.keywordvalue = t1->weight;
						itemKS.position.push_back( *it1 );
						itemKS.position.push_back( *it2 );
						msg.ResultKeyWord.insert( multimap<int,KeySearch>::value_type(NO_CB,itemKS ) );
					}
					break;
			}
		case 3:
			{
				if ( j.empty() || m.empty() )
					break;
				for(vector<int>::iterator it1 = i.begin(); it1 < i.end(); it1++)
					for(vector<int>::iterator it2 = j.begin(); it2 < j.end(); it2++)
						for(vector<int>::iterator it3 = m.begin(); it3 < m.end(); it3++)
						{
							KeySearch itemKS;
							itemKS.keyname = Item_Content;
							itemKS.keywordvalue = t1->weight;
							itemKS.position.push_back( *it1 );
							itemKS.position.push_back( *it2 );
							itemKS.position.push_back( *it3 );
							msg.ResultKeyWord.insert( multimap<int,KeySearch>::value_type(NO_CB,itemKS ) );
						}
				break;
			}
		case 4:
			{
				if ( j.empty() || m.empty() || n.empty() )
					break;
				for(vector<int>::iterator it1 = i.begin(); it1 < i.end(); it1++)
					for(vector<int>::iterator it2 = j.begin(); it2 < j.end(); it2++)
						for(vector<int>::iterator it3 = m.begin(); it3 < m.end(); it3++)
							for(vector<int>::iterator it4 = n.begin(); it4 < n.end(); it4++)
							{
								KeySearch itemKS;
								itemKS.keyname = Item_Content;
								itemKS.keywordvalue = t1->weight;
								itemKS.position.push_back( *it1 );
								itemKS.position.push_back( *it2 );
								itemKS.position.push_back( *it3 );
								itemKS.position.push_back( *it4 );
								msg.ResultKeyWord.insert( multimap<int,KeySearch>::value_type(NO_CB,itemKS ) );
							}
				break;
			}
		default:
			break;
		}
		continue;
	}
}


UnitInCB* message_classifier::insert(UnitInCB * current)//xuye: maybe this function is wrong?
{
	UnitInCB* k = this->headOfUnit->next;
	UnitInCB* prev_of_k = this->headOfUnit;
	int flag = 1;
	while( k != NULL) 
	{
		flag = k->content < current->content;
		if ( flag == 0)
			return k;
		else if ( flag == 1)
			break;
		prev_of_k = k;
		k = k->next;
	}	
	prev_of_k->next = current;// 插入数据
	current->next = k;
	return current;
}

int** message_classifier::malloc2d(int r,int c)
{
	int ** t = new int*[r];
	for(int i = 0; i < r; i++)
	{
		t[i] = new int[c];
	}
	return t;
}

// 替换字加载工作
int message_classifier::substitute_normalizer(string path)
{
	string temp_str = "";
	ifstream subFile;
	subFile.open( path.c_str() );

	while( getline(subFile, temp_str) )
	{
		string char_in_store = "";
		int total_sub = 0;
		UnitInCB* origin = NULL;
		UnitInCB** p = new UnitInCB*[500]();//xuye: 10 is too short?
		int i = 0;
		for( ; i < temp_str.length(); i++)
		{
			if ( temp_str[i] == ' ')
			{
				origin = this->headOfUnit->next;
				while( origin != NULL)
				{
					if( origin->content == char_in_store )// pointer of origin found
						break;
					origin = origin->next;					
				}
				char_in_store = "";
				break;
			}
			char_in_store += temp_str[i];
		}
		if ( origin == NULL)
			continue;//return 1;
		for( i++; i < temp_str.length(); i++ )	
		{
			if (temp_str[i] == ' ')
			{
				UnitInCB* t = new UnitInCB();
				t->content = char_in_store;// char_in_store 入UnitInCB
				//UnitInCB* r = this->headOfUnit->next;// find the location and insert it
				//UnitInCB* prev_of_r = this->headOfUnit;
				p[total_sub++] = insert(t);// target inserted, and the location pointer returned to be used by codeBook	//xuye: if t->content has exsist already,there is a waste of memory			
                char_in_store = "";
				continue;
			}
			char_in_store += temp_str[i];
		}
		UnitInCB* t = new UnitInCB();
		t->content = char_in_store;// char_in_store 入UnitInCB
		//UnitInCB* r = this->headOfUnit->next;// find the location and insert it
		//UnitInCB* prev_of_r = this->headOfUnit;
		p[total_sub++] = insert(t);// target inserted, and the location pointer returned to be used by codeBook				
		char_in_store = "";
		origin->substitute = new UnitInCB*[total_sub+1]; // update the origin's substitute content
		int u = 0;
		for(; u < total_sub; u++)
		{
			//UnitInCB* fu = p[u];
			origin->substitute[u] = p[u];
		}
		origin->substitute[u] = NULL;
	}
	return 1;
}

// codeBook加载工作
int message_classifier::codeBook_normalizer(string pathOfKW)
{
	string temp_content = "";
	ifstream keywordFile;
	keywordFile.open( pathOfKW.c_str() );

	getline(keywordFile,temp_content);
	int numOfUnit = 0;
	istringstream ss(temp_content);
	ss >> temp_content;
	this->codeBook = new ItemInCB();
	ItemInCB * t1 = codeBook;
	ss >> t1->weight;
	int fuck = temp_content.length() / 2;
	t1->words = new UnitInCB*[fuck + 1];
	headOfUnit->next = new UnitInCB();
	UnitInCB * t2 = headOfUnit->next;
	t2->content = temp_content.substr(0,2);
	t1->words[numOfUnit++] = t2; 
	for(int i = 2; i < temp_content.length(); i += 2)
	{		
		//string toFind = temp_content.substr(i,2);
		UnitInCB* current = new UnitInCB();
		current->content = temp_content.substr(i,2);		
		t1->words[numOfUnit++] = insert(current);// 基于链表的插入排序，找到位置并返回// 从头开始找		
	}	
	t1->words[numOfUnit] = NULL;
	while( getline(keywordFile, temp_content) )// 后面循环
	{
		numOfUnit = 0;
		istringstream keyStream(temp_content);
		keyStream >> temp_content;// 先传递内容
		t1->next = new ItemInCB();
		t1 = t1->next;
		fuck = temp_content.length() / 2;
		t1->words = new UnitInCB*[fuck+1];
		keyStream >> t1->weight;
		//t1->weight = 1;
		for(int j =0; j < temp_content.length(); j += 2)
		{
			UnitInCB* current = new UnitInCB();
			current->content = temp_content.substr(j,2);
			t1->words[numOfUnit++] = insert(current);
		}
		t1->words[numOfUnit] = NULL;
	}
	keywordFile.close();



	temp_content = "";
	string keywordstr = "";
	keywordFile.open(pathOfKW.c_str());

	while(getline(keywordFile,temp_content))
	{
		//cout<<temp_content.size()<<"\t"<<temp_content<<endl;
		istringstream ist(temp_content);
		ist>>temp_content;
		for(int ii=0;ii<temp_content.size()/2;++ii){
			keywordstr+=temp_content.substr(ii*2,2)+' ';
			//cout<<keywordstr.size()<<" "<<temp_content.substr(ii*2,2)<<" "<<temp_content.size()<<endl;
		}
	}
	//cout<<keywordstr.size()<<endl;
	keywordFile.close();
	
    string strdf=origin_position+"character_all.txt";
    string strsdf=origin_position+"character.txt";
	ifstream dictFile;
    dictFile.open(strdf.c_str());
	ofstream smalldictFile;
    smalldictFile.open(strsdf.c_str());
	
	while(getline(dictFile,temp_content))
	{
		//cout<<temp_content.substr(0,2)<<endl;
		
		if( keywordstr.find(temp_content.substr(0,2))!= string::npos )
		{
			//cout<<keywordstr.find(temp_content.substr(0,2))<<endl;
			smalldictFile << temp_content << endl;
		}
	}
	dictFile.close();
	smalldictFile.close();







	return 1;
}

// 初始化函数
// codeBook 加载(全部相关数据)
int message_classifier::init(string pathOfCB)
{
	this->sta.preparation();
	this->headOfUnit = new UnitInCB();
	codeBook_normalizer(origin_position + pathOfCB);
	substitute_normalizer(origin_position + "character.txt");
	return 1;
}

// codeBook 打印函数和 headOfUnit打印函数
void message_classifier::print_all()
{
	UnitInCB * t = this->headOfUnit;// headOfUnit 打印
	while( t!= NULL)
	{
		
		int cnt;
		/*if ( t->hierachy )
		{
			cout << t->content << ":";
			cnt = t->hierachy[0];	
			for( int i = 1;i < cnt; i++)
				cout << t->hierachy[i] << " ";
			cout << "@@@";
		}*/

		if(t->substitute)
		{
			cout << t->content<<":";
			UnitInCB** p=t->substitute;
			while(*p!=NULL)
			{
				cout << (*p)->content<<",";
				p++;
			}
			cout << endl;
		}

		
		t = t->next;
	}
	cout << "@@@"<<endl;
	ItemInCB * s = this->codeBook;
	while( s!= NULL)
	{
		cout << s->words[0]->content<<":"<<s->weight << " ";
		s = s->next;
	}
	cout << endl;
}

int message_classifier::process(MessageStructure &msg)
{
	this->sta.dis_clear();
	this->sta.sentive_fre_clear();
	int location = 0;
	multimap<myString,int> characters;
	SmsCut_SortedInsert(msg.msg_content, characters, location);
	this->sta.process_identify(msg.msg_content);// 生成统计特征的过程
	UnitInCB* i = this->headOfUnit->next;// 生成层次特征的过程
	multimap<myString,int>::iterator j = characters.begin();
	multimap<myString,int>::iterator jjj = characters.begin();
    while (jjj != characters.end())
    {
        //cout << jjj->first.content << " " << jjj->second << endl;
        //cout << endl;
        jjj++;
    }
	for( ; i != NULL && j != characters.end(); )// about j's movement,a little complicated, because of repeating keys
	{
		int flag = i->content < j->first.content;
		if ( flag == 1 )
			j++;
		else if( flag == 0)
		{
			int total = 1;
			multimap<myString,int>::iterator z = j;
			z++;
			//j++;
			while( z!= characters.end() && z->first.content == i->content )//连续出现关键字才会进入循环
				total++,z++;
			i->hierachy = new int[total+1];
			i->hierachy[0] = total+1;
			for(int m = 1; m < total+1; m++)
			{
				i->hierachy[m] = (j++)->second;
			}				
		}
		else
			i = i->next;

	}
	// through coodBook
	keyStructure_generator(msg);
	return 1;
}

void message_classifier::targetSortedInsert(const string& soc, multimap<myString, int>& target, int& location)// based on array, insertion
{	
	myString temp;
	temp.content = soc;
	target.insert(pair<myString,int>(temp,location++));
	//target[temp]=location++;
	//target.push_back();

}

int message_classifier::SmsCut_SortedInsert(string& source,multimap<myString,int>& characters, int& location)
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
					this->sta.num_total++;
					// 已经确定是四字节字符，判断是否为中文字符					
					if( (source[i] == (char)0x81 ||  source[i] == (char)0x82)	// 针对CJK扩充A
						||  (source[i] >= (char)0x95 && source[i] <= (char)0x98)) // 针对CJK扩充B
					{
						this->sta.Chn.totalNum++;
						targetSortedInsert(source.substr(i,4),characters, location);
						//characters.push_back(source.substr(i,4));
					
					// 敏感词处理
						str_temp = source.substr(i,4);
						i += 4;
						if( this->sta.SentiveWord.find(str_temp) != this->sta.SentiveWord.end() )
						{
							this->sta.SentiveWord[str_temp].fre++;							
						}				
						continue;
					}
					else // 非中文字符
					{
						if( this->sta.PuncSet.find( source.substr(i,4) ) != this->sta.PuncSet.end() )
						{
							this->sta.punc.totalNum++;
							//characters.push_back( punc );
							i += 4;						
							continue;
						}
						else
						{
							this->sta.junk.totalNum++;// 这里认为，既不是中文字符，又不存在于标点符号文件中的字符为垃圾字符
							//characters.push_back(source.substr(i,4));
							i += 4;
							continue;
						}
					}
				}
			}
			if ( i+1 <= end )
			{
				if((source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
					|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) )
				{
					this->sta.num_total++;
					if ( (source[i] >= (char)0xb0 && source[i] <= (char)0xf7
						&& source[i+1] >= (char)0xa1 && source[i+1] <= (char)0xfe) // 6763个汉字

						||( (source[i] >= (char)0x81 && source[i] <= (char)0xa0)// 高字节
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xfe) ) )// 6080个汉字

						||( source[i] >= (char)0xaa && source[i] <= (char)0xfe
						&& ( (source[i+1] >= (char)0x40 && source[i+1] <= (char)0x7e)
						|| (source[i+1] >= (char)0x80 && source[i+1] <= (char)0xa0) ) ) ) // 8160个汉字
					{
						this->sta.Chn.totalNum++;
						// 中文符号
						targetSortedInsert(source.substr(i,2),characters, location);
						//characters.push_back(source.substr(i,2));
					// 敏感词处理
						str_temp = source.substr(i,2);
						i += 2;
						if( this->sta.SentiveWord.find(str_temp) != this->sta.SentiveWord.end() )
						{
							this->sta.SentiveWord[str_temp].fre++;							
						}
						continue;
					}
					else// 非中文字符
					{
						if( this->sta.PuncSet.find( source.substr(i,2) ) != this->sta.PuncSet.end() )
						{
							this->sta.punc.totalNum++;
							//characters.push_back( punc );
							i += 2;
							continue;
						}
						else
						{
							this->sta.junk.totalNum++;
							//characters.push_back(source.substr(i,4));
							i += 2;
							continue;
						}
					}
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
						str_temp += ( source[i] + (char)0x20 );
					else					
						str_temp += source[i];
					this->sta.En.totalNum++;
					i++;
					this->sta.num_total++;
				}
				while( i <= end
					&& ((source[i] >= (char)0x61 && source[i] <= (char)0x7a)
					|| (source[i]>= (char)0x41 && source[i] <= (char)0x5a)) );
				targetSortedInsert(str_temp,characters, location);
				//characters.push_back( str_temp);
				// 敏感词判断
				if( this->sta.SentiveWord.find(str_temp) != this->sta.SentiveWord.end() )
					this->sta.SentiveWord[str_temp].fre++;					
				continue;
			}
			// 数字存储
			else if( source[i] >= (char)0x30 && source[i] <= (char)0x39 )
			{				
				do
				{
					targetSortedInsert(source.substr(i,1),characters, location);
					//characters.push_back( source.substr(i,1) );
					str_temp += source[i];
					this->sta.num.totalNum++;
					i++;
					this->sta.num_total++;
				}
				while ( i<source.length() && source[i] >= (char)0x30 && source[i] <= (char)0x39);//必须先判断i是否越界
				continue;				
			}
			// 判断标点符号
			if( this->sta.PuncSet.find(source.substr(i,1)) != this->sta.PuncSet.end() )
			{
				this->sta.punc.totalNum++;
				//characters.push_back( punc );
				i++;
				this->sta.num_total++;
				continue;
			}
			else
			{
				this->sta.junk.totalNum++;
				//characters.push_back(source.substr(i,4));
				i ++;
				this->sta.num_total++;
				continue;
			}
			++i;
			this->sta.num_total++;
			continue;
		}
		i++;
	}
	// 计算比例部分
	this->sta.En.ratio = (double)this->sta.En.totalNum / this->sta.num_total;
	this->sta.Chn.ratio = (double)this->sta.Chn.totalNum / this->sta.num_total;
	this->sta.punc.ratio = (double)this->sta.punc.totalNum / this->sta.num_total;
	this->sta.junk.ratio = (double)this->sta.junk.totalNum / this->sta.num_total;
	this->sta.num.ratio = (double)this->sta.num.totalNum / this->sta.num_total;
	return 0;
	return 1;
}

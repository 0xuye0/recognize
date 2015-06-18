# include <sys/types.h>
# include <dirent.h>
# include "head.h"
# include "SmsCal.h"
# include "result.h"
# include "config.h"
# include "RFile.h"
# include "BacDel.h"
# include <string.h>

# include <iostream>
# include <fstream>
# include <time.h>
# include <vector>

# include <string>
# include <time.h>
#include <sstream>
using namespace std;



string smspath("");
string resultpath("");
string keyfile("");
string backpath("");
string rowsplit("");
string colsplit("");
float cycle(5);
int delcycle(5);
string origin_position("");
message_classifier test;
int process(vector<pair<bool,string> >& content)
{
	try
    {
		int number = content.size();
		result final;
		for(int ia = 0; ia < number; ia++)
		{
            if(content[ia].second=="")
                continue;
			MessageStructure msg;
			msg.msg_content = content[ia].second;
			test.process(msg);
			SmsClassifyCalculate test2;
			test2.process(msg);
			// value transfer
			final.result_transfer_sta(test);
			final.result_transfer_hir(msg);
			final.sentence();
			// change
			if ( final.identifyResult >0)
			{
				content[ia].first = true;
				multimap<int, KeySearch>::iterator it = msg.ResultKeyWord.begin();
				set<string> tt;
				while ( it != msg.ResultKeyWord.end())
				{
					tt.insert(it->second.keyname);
					it++;
				}
				set<string>::iterator ttit=tt.begin();
                content[ia].second.append(colsplit);
				while( ttit!=tt.end())
				{
					content[ia].second.append(*ttit+' ');
					ttit++;
				}
				tt.clear();
			}
			test.Unit_clear();
			final.clear();
			msg.clear();
		}
	}
	catch(int)
	{
		return -1;
	}
	return 1;
}

int main()
{
	Config conf;
	conf.Init("./config.txt");
	if(!conf.GetValue("smspath",smspath))
	{
		cout<<"no smspath found!"<<endl;
		return 1;
	}
	if(!conf.GetValue("resultpath",resultpath))
	{
		cout<<"no resultpath found!"<<endl;
		return 1;
	}
	if(!conf.GetValue("keyfile",keyfile))
	{
		cout<<"no keyfile found,use default keyword file!"<<endl;
		keyfile="keyword.txt";
	}
	if(!conf.GetValue("backpath",backpath))
	{
		cout<<"no backpath found!"<<endl;
		return 1;
	}
	if(!conf.GetValue("rowsplit",rowsplit))
	{
		// cout<<"no rowsplit found!"<<endl;
		rowsplit="\n";
	}
	if(!conf.GetValue("colsplit",colsplit))
	{
		cout<<"no colsplit found!"<<endl;
		colsplit = "|";
	}
	string scycle;
	if(!conf.GetValue("cycle",scycle))
	{
		cout<<"no cycle found!"<<endl;
		//return 0; no need to return , cycle is default 5
	}
	else
	{
		stringstream temp;
		temp<<scycle;
		temp>>cycle;
	}
	string sdelcycle;
	if(!conf.GetValue("delcycle",sdelcycle))
	{
		// cout<<"no delcycle found!"<<endl;
	}
	else
	{
		stringstream temp;
		temp<<sdelcycle;
		temp>>delcycle;
	}
    if(!conf.GetValue("datafile",origin_position))
    {
        cout<<"no datafile found!"<<endl;
        return 1;
    }
    else
    {
        origin_position+="/";
    }
	int delnum = 0;
	test.init(keyfile);
    cout << "init done" << endl;
	while(1)
	{
		BacDel bd;
		if(!bd.Init(backpath))
		{
			cout<<"backup init error!"<<endl;
			return 1;
		}
			// 数据准备
		set<string> s1;

	    DIR *dir;
	    dir=opendir(smspath.data());
	    struct dirent *d;
	    while((d=readdir(dir)))
	    {
	    	int size =strlen(d->d_name);
	    	if (strcmp((d->d_name + (size-4)),".txt")==0)
	    		s1.insert(smspath+"/"+d->d_name);
	    }
	    closedir(dir);

        if (s1.size() == 0)
        {
            cout << "no sms file found in the smspath!" << endl;
            return 1;
        }
		set<string>::iterator it=s1.begin();
		while(it != s1.end())
		{
			bd.Backup(*it);
			RFile rfile;
			if(!rfile.Init(rowsplit,colsplit,*it))
			{
				cout << "read file init error!" << endl;
				return 1;
			}
			string temp = "";
			vector<string> VecSendTime;
			vector<string> VecSendNum;
			vector<string> VecRecNum;
			vector<string> VecType;
			vector<string> VecRecTime;
			vector<pair<bool,string> > VecContent;
			while( rfile.GetLine() )
			{
				vector<string> temp;
				if(!rfile.Split(temp))
				{
					cout << "readfile cannot split sentence!" << endl;
					return 1;
				}
				VecSendTime.push_back(temp[0]);
				VecSendNum.push_back(temp[1]);
				VecRecNum.push_back(temp[2]);
				VecType.push_back(temp[3]);
				VecRecTime.push_back(temp[4]);
				VecContent.push_back(make_pair(false,temp[5]));
			}
			clock_t start,end;
			start = clock();
		
			// 调用
			int flag = process(VecContent);
			if (flag == -1)
            {
				cout << "未能成功运行!" << endl;
                return 1;
            }
			end = clock();
			cout<<"Run time: "<<(double)(end - start) / CLOCKS_PER_SEC<<"S"<<endl;	
			int count = 0;
			string output,output1;
			for(size_t z = 0; z < VecContent.size(); z++)
			{
				if (VecContent[z].first)
				{
					output1 += VecSendTime[z]+colsplit+VecSendNum[z]+colsplit+
                               VecRecNum[z]+colsplit+VecType[z]+colsplit+
                               VecRecTime[z]+colsplit+VecContent[z].second+rowsplit;
					continue;
				}
				else
				{
					output += VecSendTime[z]+colsplit+VecSendNum[z]+colsplit+
                              VecRecNum[z]+colsplit+VecType[z]+colsplit+
                              VecRecTime[z]+colsplit+VecContent[z].second+rowsplit;
				}
			}
            size_t found = (*it).find_last_of("/");
            string prefix = (*it).substr(found+1);
			string normal=prefix + ".normal";
			string abnormal=prefix + ".abnormal";
			if (resultpath!="")
			{
				normal=resultpath+"/"+prefix+".normal";
				abnormal=resultpath+"/"+prefix+".abnormal";
			}
			ofstream aa(normal.c_str());
			aa << output;
			ofstream bb(abnormal.c_str());
			bb << output1;
			aa.close();
			bb.close();
			bd.Delete(*it);
			it++;
		}
		sleep(int(cycle*60));
		delnum++;
		if(delnum==delcycle)
		{
			bd.DeleteAll();
			delnum = 0;
		}
	}
	return 0;
}

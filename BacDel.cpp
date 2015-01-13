#include"BacDel.h"
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<string>

using namespace std;

bool BacDel::Init(const string& path)
{
	BacPath=path;
	return true;
}
bool BacDel::Backup(const string& path)
{
	SourcePath=path;
	std::string backname;
	int pos=path.find_last_of("\\");
	if (pos>0)
	{
		std::string filename(path.substr(pos + 1) );
		backname=BacPath+"\\"+filename;
		//cout<<backname;
	}
	else
	{
		int lpos=path.find_last_of("/");
		if(lpos>0)
		{
			std::string filename(path.substr(lpos + 1) );
			backname=BacPath+"/"+filename;
		    // cout<<backname;
		}
		else
		{
			return 0;
			//cout<<"Error 0:Failname not exist"<<endl;
		}
	}
	ifstream in;
	ofstream out;
	// char*spath=(char*)SourcePath.data();
	
	in.open(SourcePath.c_str(),ios::in);//打开源文件
	if(in.fail())//打开源文件失败
	{
		//cout<<"Error 1: Fail to open the source file."<<endl;
		in.close();
		out.close();
		return 0;
	}
	out.open(backname.c_str(),ios::trunc);//创建目标文件 
	if(out.fail())//创建文件失败
	{
		//cout<<"Error 2: Fail to create the new file."<<endl;
		out.close();
		in.close();
		return 0;
	}
	else//复制文件
	{
		out<<in.rdbuf();
		out.close();
		in.close();
		BackedFiles.push_back(SourcePath);
		return 1;
	}
}
bool BacDel::Delete(const string& path)
{
	if(remove(path.c_str())==0)
	{
		return 1;
	}
	else
	{
		//printf("remove error");
		return 0;
	}
}
bool BacDel::DeleteAll()
{
	std::vector<std::string>::iterator it;
	for(it=BackedFiles.begin();it!=BackedFiles.end();it++)
	{
		std::string bfile=*it;
		const char* file=bfile.c_str();
		remove(file);				
	}
	return true;
}

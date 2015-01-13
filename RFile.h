#pragma once
#include <fstream>
#include <vector>
class RFile
{
public:
	RFile(void);
	~RFile(void);
	bool Init(const std::string& RowSplit1,const std::string& ColSplit1,const std::string& RFileName1);//{return true;}
	bool GetLine();//{return true;}
	bool Split(std::vector<std::string>& items);//{return true;}
private:
	std::ifstream Rfile;
	std::string RowSplit;
	std::string ColSplit;
	std::string content;
};


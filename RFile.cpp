#include "RFile.h"

using namespace std;
RFile::RFile(void)
{
}


RFile::~RFile(void)
{
}

void splitStr2Strs(string &str, const char *delimiter, vector<string> &res)
{
    size_t last_found = 0;
    size_t found = str.find(delimiter,last_found);
    res.clear();
    int count = 0;
    while(found != string::npos && count < 5)
    {
        count++;
        res.push_back(str.substr(last_found,found - last_found));
        last_found = found+1;
        found = str.find(delimiter,last_found);
    }
    size_t n = str.size() - 1;
    res.push_back(str.substr(last_found, n - last_found));
}

bool RFile::Init(const std::string& RowSplit1,const std::string& ColSplit1,const std::string& RFileName1)
{
    RowSplit = RowSplit1;
    ColSplit = ColSplit1;
    Rfile.open(RFileName1.c_str());
    return true;
}

bool RFile::GetLine()
{
    content.clear();
    char c;
    while(Rfile.get(c))
    {
        if (c == RowSplit.c_str()[0])
            break;
        content.append(1,c);
    }
    if (content.size() <= 0)
        return false;
    else
        return true;
}

bool RFile::Split(std::vector<std::string>& items)
{
    splitStr2Strs(content,ColSplit.c_str(),items);
    if(items.size() <= 0)
        return false;
    else
        return true;
}


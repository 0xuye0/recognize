#pragma once
#include <string>
#include <vector>
class BacDel
{
public:
	bool Init(const std::string& path);
	bool Backup(const std::string& path);
	bool Delete(const std::string& path);
	bool DeleteAll();
private:
	std::string SourcePath;
	std::string BacPath;
	std::vector<std::string> BackedFiles;
};


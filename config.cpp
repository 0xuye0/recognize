#include "config.h"

Config::Config(){
}

Config::~Config(){
}

bool Config::Init(const std::string& path){
	this->path = path;


	ifstream is (this->path.c_str());
	if (!is.good()){
		cout << "Cannot open config file"<<this->path<<endl;
		return false;
	}
	string tmp;
	while (getline(is, tmp)){
		tmp = trim(tmp);				//ȥ��ǰ��ո�
		int pos = tmp.find("#");			//#��ע��
		if (pos != std::string::npos){
			if (pos == 0) continue;			//���䱻ע��
			else{
				tmp = tmp.substr(0, pos);	//�������ע��
			}
		}
		int separator = tmp.find("=");
		if (separator != std::string::npos){
			if (separator == 0){
				cout << "error found :some configuration has no key" << endl;
				continue;
			}
			else if (separator == tmp.length() - 1){
				cout << "error found :some configuration has no value" << endl;
				continue;
			}
			string key;
			string value;
            string tmptmp = tmp.substr(0,separator);
			key = trim(tmptmp);
            tmptmp = tmp.substr(separator+1);
			value = trim(tmptmp);
			this->configs.insert(std::pair<string, string>(key, value));
		}
	}
	return true;
}


bool Config::GetValue(const std::string& key, std::string& value){
	if (this->configs.find(key) != configs.end()){
		value = this->configs.find(key)->second;
		return true;
	}
	else{
		return false;
	}

}


#ifndef VCGIO_H
#define VCGIO_H

#include <fstream>

using namespace std;


class vcgIOReader
{


private:
	


public:
	
	vcgIOReader();
	virtual ~vcgIOReader();
	//模板函数：将string类型变量转换为常用的数值类型
	template <class Type>
	Type stringToNum(const string& str);
	//读取xyz格式文件
	bool readFile(string path);
};

#endif

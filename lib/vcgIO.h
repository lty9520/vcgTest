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
	//ģ�庯������string���ͱ���ת��Ϊ���õ���ֵ����
	template <class Type>
	Type stringToNum(const string& str);
	//��ȡxyz��ʽ�ļ�
	bool readFile(string path);
};

#endif

#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh//export.h>
#include <vcg/complex/complex.h>
#include <wrap/ply/plylib.h>
#include <wrap/ply/plylib.cpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include<GLFW\glfw3.h>

#include "..\lib\vcgIO.h"


// ��������
class MyVertex; class MyEdge;
class MyFace;
typedef vcg::UsedTypes<
	vcg::Use<MyVertex>::AsVertexType,
	vcg::Use<MyEdge>  ::AsEdgeType,
	vcg::Use<MyFace>  ::AsFaceType >
	MyUsedTypes;
// ��������
class MyVertex : public vcg::Vertex<MyUsedTypes,
	vcg::vertex::Coord3f,
	vcg::vertex::Normal3f,
	vcg::vertex::BitFlags > { };
// ������
class MyEdge : public vcg::Edge<MyUsedTypes,
	vcg::edge::VertexRef,
	vcg::edge::EFAdj,
	vcg::edge::BitFlags > { };
// �����ͣ�������
class MyFace : public vcg::Face<MyUsedTypes,
	vcg::face::VertexRef,
	vcg::face::Normal3f,
	vcg::face::FFAdj,
	vcg::face::BitFlags > { };
// ��������
//����������ݣ����㡢�ߡ����������飨std::vector<>����
//ÿ������������ԣ��ռ����꣨3��float��ʾ�������㷨��������־λ��
//ÿ���߰������ԣ�����ָ�루ָ��ñߵ��������㣩���� - ���ڽ���Ϣ����־λ��
//ÿ����������������ԣ�����ָ�루ָ��������ε��������㣩���淨�������� - ���ڽ���Ϣ����־λ��
typedef vcg::tri::TriMesh<
	std::vector<MyVertex>,
	std::vector<MyEdge>,
	std::vector<MyFace> >
	GLMesh;//�Զ�����������

using namespace std;
using namespace vcg;


vcgIOReader::vcgIOReader()
{
	
}

vcgIOReader::~vcgIOReader()
{
	
}

//ģ�庯������string���ͱ���ת��Ϊ���õ���ֵ����
//************************************
// Method:    stringToNum
// FullName:  vcgIOReader::stringToNum
// Access:    public 
// Returns:   Type
// Qualifier:
// Parameter: const string & str
//************************************
template <class Type>
Type vcgIOReader::stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

bool vcgIOReader::readFile(string path)
{

	ifstream fin(path, std::ios::in);
	char line[1024] = { 0 };
	string x_s = "";
	string y_s = "";
	string z_s = "";
	vector<double> x;
	vector<double> y;
	vector<double> z;
	

	clock_t start_time = clock();
	cout << "Reading file ......";
	if (fin)
	{
		while (fin.getline(line, sizeof(line)))
		{
			std::stringstream word(line);
			word >> x_s;
			word >> y_s;
			word >> z_s;
			x.push_back(stringToNum<double>(x_s));
			y.push_back(stringToNum<double>(y_s));
			z.push_back(stringToNum<double>(z_s));
		}
		fin.clear();
		fin.close();
	}
	else
	{
		cout << "Open failed!" << endl;
		cout << "File not exist" << path << endl;
	}

	cout << "Use: " << double(clock() - start_time) / CLOCKS_PER_SEC << "s" << endl;
	
	return true;
}

int main()
{
	vcgIOReader vcgio;

	string file_path = "rabbit.txt";
	//if (vcgio.readFile(file_path))
	//	cout << "file read finished!" << endl;
	//else
	//	cout << "file read false!" << endl;
	

	// Mesh �ļ�һ�����ٰ�������������Ϣ�������԰���������Ϣ(������)�����㷨������������ɫ������ɫ��
	// �淨��������������ȵ����ԣ��� mask �Ķ�����λ����ǻ���ƶ�ȡ��д���� Mesh �ļ�����Щ����
	// �� vcg::tri::io::Mask����ȡ PLY ��Ҫ�����ļ� "vcglib/wrap/ply/plylib.cpp"�������
	// ͷ�ļ�������#include "wrap/io_trimesh/import.h" #include "wrap/io_trimesh/export.h"
	GLMesh m; int mask;
	//// ��ȡ PLY �ļ�������鷵��ֵ������ mask Ϊ��ѡ��mask �Ƿ��ز�������������Щ����
	//if (vcg::tri::io::ImporterPLY<GLMesh>::Open(m, "file_to_open.ply", mask)
	//	!= vcg::ply::E_NOERROR) {
	//	std::cout << "Load PLY file ERROR\n";
	//}
	//// some modification to m and mask ...
	//// ���� PLY �ļ���mask ��������������� m ����Щ���Ա�д�뵽�ļ�
	//vcg::tri::io::ExporterPLY<GLMesh>::Save(m, "file_to_save.ply", mask);

	// ��ȡ��д�� OBJ �ļ��Ĵ��룬mask ����ͬ��
	//if (vcg::tri::io::ImporterOBJ<GLMesh>::Open(m, "3-ming-nei-bujian(NS).obj", mask)
	//	!= vcg::tri::io::ImporterOBJ<GLMesh>::E_NOERROR) 
	//{
	//	std::cout << "Load OBJ file ERROR\n";
	//}
	// some modification to m and mask ...
	//vcg::tri::io::ExporterOBJ<GLMesh>::Save(m, "file_to_save.obj", mask);
	// ��ȡ��д�������ļ����������ļ���չ���Զ�ƥ���ļ���ʽ ---------------------------------------
	int oerr = vcg::tri::io::Importer<GLMesh>::Open(m, "3-waiyan-zhutou.obj", mask);
	if (oerr != 0){
		std::cout << "Load mesh file ERROR: "
			<< vcg::tri::io::Importer<GLMesh>::ErrorMsg(oerr) << '\n';
	}
	//// some modification to m and mask ...
	//int serr = vcg::tri::io::Exporter<GLMesh>::Save(m, "file_to_save.3ds", mask);
	//if (serr != 0){
	//	std::cout << "Save mesh file ERROR: "
	//		<< vcg::tri::io::Exporter<GLMesh>::ErrorMsg(oerr) << '\n';
	//}

	vcg::tri::UpdateNormal<GLMesh>::PerVertexNormalized(m); // ���㶥�㷨����������λ��
	//vcg::tri::UpdateNormal<GLMesh>::PerFaceNormalized(m); // �����淨����������λ��

	//GLMesh::VertexType& v = m.vert[50]; // �� i �����㣬���� v.isD()==false����δ��־Ϊ��ɾ��
	//cout << v.N().X() << endl;


	/*glewExperimental = GL_TRUE;
	if (glewInit()!=GLEW_OK)
	{
	cout << "failed to initalize GLEW" << endl;
	return -1;
	}*/

	

	system("pause");

	return 0;
}
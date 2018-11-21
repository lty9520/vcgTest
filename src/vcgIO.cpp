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


// 类型声明
class MyVertex; class MyEdge;
class MyFace;
typedef vcg::UsedTypes<
	vcg::Use<MyVertex>::AsVertexType,
	vcg::Use<MyEdge>  ::AsEdgeType,
	vcg::Use<MyFace>  ::AsFaceType >
	MyUsedTypes;
// 顶点类型
class MyVertex : public vcg::Vertex<MyUsedTypes,
	vcg::vertex::Coord3f,
	vcg::vertex::Normal3f,
	vcg::vertex::BitFlags > { };
// 边类型
class MyEdge : public vcg::Edge<MyUsedTypes,
	vcg::edge::VertexRef,
	vcg::edge::EFAdj,
	vcg::edge::BitFlags > { };
// 面类型，三角形
class MyFace : public vcg::Face<MyUsedTypes,
	vcg::face::VertexRef,
	vcg::face::Normal3f,
	vcg::face::FFAdj,
	vcg::face::BitFlags > { };
// 网格类型
//网格包含数据：顶点、边、三角形数组（std::vector<>）；
//每个顶点包含属性：空间坐标（3个float表示）、顶点法向量、标志位；
//每个边包含属性：顶点指针（指向该边的两个顶点）、边 - 面邻接信息、标志位；
//每个三角形面包含属性：顶点指针（指向该三角形的三个顶点）、面法向量、面 - 面邻接信息、标志位。
typedef vcg::tri::TriMesh<
	std::vector<MyVertex>,
	std::vector<MyEdge>,
	std::vector<MyFace> >
	GLMesh;//自定义网格类型

using namespace std;
using namespace vcg;


vcgIOReader::vcgIOReader()
{
	
}

vcgIOReader::~vcgIOReader()
{
	
}

//模板函数：将string类型变量转换为常用的数值类型
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
	

	// Mesh 文件一般至少包含顶点数组信息，还可以包含连接信息(三角形)、顶点法向量、顶点颜色、面颜色、
	// 面法向量、纹理坐标等等属性，用 mask 的二进制位来标记或控制读取或写入了 Mesh 文件的哪些属性
	// 见 vcg::tri::io::Mask，读取 PLY 需要包含文件 "vcglib/wrap/ply/plylib.cpp"（见这里）
	// 头文件包含：#include "wrap/io_trimesh/import.h" #include "wrap/io_trimesh/export.h"
	GLMesh m; int mask;
	//// 读取 PLY 文件，并检查返回值，参数 mask 为可选，mask 是返回参数：读入了哪些属性
	//if (vcg::tri::io::ImporterPLY<GLMesh>::Open(m, "file_to_open.ply", mask)
	//	!= vcg::ply::E_NOERROR) {
	//	std::cout << "Load PLY file ERROR\n";
	//}
	//// some modification to m and mask ...
	//// 保存 PLY 文件，mask 是输入参数，控制 m 的哪些属性被写入到文件
	//vcg::tri::io::ExporterPLY<GLMesh>::Save(m, "file_to_save.ply", mask);

	// 读取或写入 OBJ 文件的代码，mask 作用同上
	//if (vcg::tri::io::ImporterOBJ<GLMesh>::Open(m, "3-ming-nei-bujian(NS).obj", mask)
	//	!= vcg::tri::io::ImporterOBJ<GLMesh>::E_NOERROR) 
	//{
	//	std::cout << "Load OBJ file ERROR\n";
	//}
	// some modification to m and mask ...
	//vcg::tri::io::ExporterOBJ<GLMesh>::Save(m, "file_to_save.obj", mask);
	// 读取、写入网格文件，将根据文件扩展名自动匹配文件格式 ---------------------------------------
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

	vcg::tri::UpdateNormal<GLMesh>::PerVertexNormalized(m); // 计算顶点法向量，并单位化
	//vcg::tri::UpdateNormal<GLMesh>::PerFaceNormalized(m); // 计算面法向量，并单位化

	//GLMesh::VertexType& v = m.vert[50]; // 第 i 个顶点，假设 v.isD()==false，即未标志为已删除
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
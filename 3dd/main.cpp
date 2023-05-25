#include "threeDdetectSystem.h"
#include <QtWidgets/QApplication>

//#include "NodeHeader.h"
#include "XmlProject.h"
#include <algorithm>

#include <functional>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置连接器选项

class testFunc
{
public:
	testFunc()
	{
		std::function<bool(int, int)> fu = std::bind(&testFunc::add, this, std::placeholders::_1, std::placeholders::_1);

	}
	~testFunc()
	{

	}

	bool add(int a, int b)
	{
		m = a + b;
		return true;
	}
	
public:
	int m = 0;
};

class Tst
{
public:
	Tst() {}
	~Tst() {}

	void setFunc(std::function<bool(int a, int b)> func) {
		m_func = func;
		m_func(5, 10);
	}

public:
	std::function<bool(int a, int b)> m_func;
};

void testptr(int * ptr)
{
	//ptr = new int;
	std::string str1="Yes";
	std::string str2;
	memcpy_s(&str2, sizeof(str2), &str1, sizeof(str1));
}
void testProcess()
{
	//
	testFunc _func;
	Tst _tT;
	std::function<bool(int, int)> func = std::bind(&testFunc::add, &_func, std::placeholders::_1, std::placeholders::_2);
	_tT.setFunc(std::bind(&testFunc::add, &_func, std::placeholders::_1, std::placeholders::_2));
	int _n = _func.m;

	//

	std::map<int, std::shared_ptr<int>> mp;
	int n = 7;

	mp.insert(std::make_pair( 5, std::shared_ptr<int>(&n)));

	std::shared_ptr<int> p = mp[5];
	std::shared_ptr<int> p2 = mp[3];
	if (!p2)
	{
		n = 9;
	}
	uint16_t key = getKeyById(127, 125);
	int projectId = getProjectId(key);
	int NodeId = getNodeId(key);
	
//	std::shared_ptr<XmlProject> project(new XmlProject(1));
	//std::shared_ptr<DDataNode> dataSource(new DDataNode(project));
	//dataSource->setReadFilepath("");//文件输入路径
	//project->addNode(dataSource);
}


int main(int argc, char *argv[])
{
	int res = 0;
	//int n = 0x1;
	//int m = 0x000000020;
	//int *ptr = nullptr;
	//testptr(ptr);
	//0x0020;
	//testLogic();
    QApplication a(argc, argv);
	//testProcess();
	threeDdetectSystem w;
	OperationalLayer::getOperationInstance().setGUICallBack(threeDdetectSystem::layerCallFun, (void*)&w);
	w.resize(QSize(1600, 900));
	w.setWindowTitle(QObject::STRING_TITLE_SYSTEM);
    w.show();

     res= a.exec();
	return res;
}

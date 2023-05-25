/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。 
//
// 文件功能描述：
//
// 
// 创建者：tangao 
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once

#include "GraphNode.h"

class GTextNode : public GraphNode
{
public:
	GTextNode(std::shared_ptr< XmlProject> project);
	~GTextNode();
public:
	bool setParameter(std::shared_ptr<SuperNodeStruct> Gtext);
	bool getParameter(G_Text &Gtext);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	bool run();
	void getBound(double bounds[6]);
	void setBound(double* bounds);
	virtual bool addToRender() ; // yzw 0718
	virtual bool removeFromRender() ;

private:
	std::string string_To_UTF8(const std::string& str);
	void updateDisplay(std::shared_ptr<SuperNodeStruct> Gtext);
private:
	G_Text m_Gtext;
	vtkSmartPointer<vtkDynamic2DLabelMapper> m_textLabelMapper;
	vtkSmartPointer<vtkPolyData> m_textPointsPolyData;
	vtkSmartPointer<vtkActor2D> m_actor_2d;
	vtkSmartPointer<vtkNamedColors> colors;
	vtkSmartPointer<vtkVectorText> atext;
	vtkSmartPointer<vtkPolyDataMapper> textMapper;
	vtkSmartPointer<vtkBillboardTextActor3D> textActor;//vtkFollower
};

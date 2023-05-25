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

class GBoundingBoxNode : public GraphNode
{
public:
	GBoundingBoxNode(std::shared_ptr< XmlProject> project);
	~GBoundingBoxNode();
public:
	bool setParameter(std::shared_ptr<SuperNodeStruct> boundingBox);
	bool getParameter(G_BoundingBox &boundingbox);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	bool run();
	bool addToRender();	// yzw 0610
	bool removeFromRender(); // yzw 0610
	void getBound(double bounds[6]);
	void setBound(double* bounds);

private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> boundingbox);
private:
	G_BoundingBox m_boundingbox;
	vtkSmartPointer<vtkActor> m_actor;
	vtkSmartPointer<vtkActor2D> m_actor_2d;
};

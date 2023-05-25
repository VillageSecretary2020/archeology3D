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
class GCommentNode : public GraphNode
{
public:
	GCommentNode(std::shared_ptr< XmlProject> project);
	~GCommentNode();

public:
	static void getDefault(SuperNodeStruct*);	
	bool setParameter(std::shared_ptr<SuperNodeStruct> comment);
	bool getParameter(G_Comment & comment);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	bool run();
	bool addToRender();
	bool removeFromRender();
	void getBound(double bounds[6]);
	void setBound(double* bounds);

private:
	std::string string_To_UTF8(const std::string& str);
	void updateDisplay(std::shared_ptr<SuperNodeStruct> comment);
private:
	vtkSmartPointer<vtkTextActor> m_textActor;
private:
	G_Comment m_comment;
};

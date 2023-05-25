#pragma once

#include "GraphNode.h"

class GDxfNode : public GraphNode
{
public:
	GDxfNode(std::shared_ptr<XmlProject> project);
	~GDxfNode();

	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }

	bool run();
	bool addToRender();	 
	bool removeFromRender() ;
	void makeNextNodeRun() {};
	void getBound(double bounds[6]) {}
	void setBound(double* bounds) {}
	void setPictureFile(std::string);

	void setPicPos(double*);

	static void getDefault(SuperNodeStruct*);		// 取默认参数
	std::shared_ptr<SuperNodeStruct> getParameter();
	bool setParameter(std::shared_ptr<SuperNodeStruct> highLowZoneMap);
//	virtual void setInputData(std::vector<pointOriginal> &data, int dataIndex = 0);
	virtual void setDataRange(double* dataRange);
private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> highlowmap);
	double first_pos[3];
	double bounds[6];
private:
	G_Picture m_picture;
	vtkSmartPointer<vtkGDALVectorReader> dxfReader;

	vtkAlgorithmOutput* outPort;
	vtkSmartPointer<vtkActor> boxActor;
	vtkSmartPointer<vtkActor> dxfActor;

};


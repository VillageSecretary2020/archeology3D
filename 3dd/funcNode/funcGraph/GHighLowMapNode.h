#pragma once

#include "GraphNode.h"

class GHighLowMapNode: public GraphNode
{
public:
	GHighLowMapNode(std::shared_ptr<XmlProject> project);
	~GHighLowMapNode();

	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }

	bool run();
	bool addToRender();	 
	bool removeFromRender() ;
	void makeNextNodeRun() {};
	void getBound(double bounds[6]) {}
	void setBound(double* bounds) {}

	static void getDefault(SuperNodeStruct*);
	std::shared_ptr<SuperNodeStruct> getParameter();
	bool setParameter(std::shared_ptr<SuperNodeStruct> highLowZoneMap);
private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> highlowmap);

private:
	vtkSmartPointer<vtkActor> m_actor;
	vtkSmartPointer<vtkImageData> m_imageData;
	//vtkSmartPointer<vtkLookupTable> m_lut;
	vtkSmartPointer<vtkDataSetMapper> m_mapper;
	G_HighLowZoneMap m_highLowPicture;
};


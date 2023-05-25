#pragma once

#include "GraphNode.h"
#include <vtkGDALRasterReader.h>

class GGeotiffNode : public GraphNode
{
public:
	GGeotiffNode(std::shared_ptr<XmlProject> project);
	~GGeotiffNode();

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

	virtual void setNewPos(double*);
	void setPicPos(double*);
	void setBoxPos();	// setOffset

	static void getDefault(SuperNodeStruct*);		// 取默认参数
	std::shared_ptr<SuperNodeStruct> getParameter();
	bool setParameter(std::shared_ptr<SuperNodeStruct> highLowZoneMap);
//	virtual void setInputData(std::vector<pointOriginal> &data, int dataIndex = 0);
	virtual void setDataRange(double* dataRange);
private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> highlowmap);
	double first_pos[3];
	double bounds[6];
	int raster_dim[2];
	double raster_pos[8];
	double tif_scale[3] = { 1.0, 1.0, 1.0 };//
private:
	G_Geotiff m_picture;
	vtkSmartPointer<vtkTIFFReader> tifReader ;
	vtkSmartPointer<vtkGDALRasterReader> rasterReader;

	vtkAlgorithmOutput* outPort;
	vtkSmartPointer<vtkActor> boxActor;
	vtkSmartPointer<vtkImageActor> picExtActor;

};


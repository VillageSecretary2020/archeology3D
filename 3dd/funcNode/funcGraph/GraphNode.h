/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
//
// 文件功能描述：
// GraphNode.h
//
// 创建者：tangAo
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once

#include "SurperNode.h"

#include <vtkAutoInit.h>
//#include <QVTKOpenGLStereoWidget.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkStructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkProperty.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkSphereSource.h>
#include <vtkScalarBarRepresentation.h>
#include <vtkLookupTable.h>
#include <vtkContourFilter.h>
#include <vtkExtractVOI.h>
#include <vtkCubeSource.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkScalarBarWidget.h>
#include <vtkLabeledDataMapper.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkRenderWindow.h>
#include <vtkClipVolume.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkAxesActor.h>
//#include <vtkUnicodeString.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>
#include <vtkVectorText.h>
#include <vtkFollower.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkImageReslice.h>
#include <vtkImageActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkMatrix4x4.h>
#include <vtkImageMapper3D.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyLine.h>
#include <vtkMassProperties.h>
#include <vtkMarchingCubes.h>
#include <vtkImageMapToColors.h>
#include <vtkTransform.h>
#include <vtkTriangleFilter.h>
#include <vtkDynamic2DLabelMapper.h>
#include <vtkAxesActor.h>
#include <vtkPointData.h>
#include <vtkPlane.h>
#include <vtkLODProp3D.h>
#include <vtkGlyph2D.h>
#include <vtkGlyph3D.h>
#include <vtkArrowSource.h>
#include <vtkCutter.h>
#include <vtkOutlineSource.h>
#include <vtkNamedColors.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtkCubeAxesActor.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkProbeFilter.h>
#include <vtkPointWidget.h>
#include <vtkJPEGReader.h>
#include <vtkTIFFReader.h>
#include <vtkGDALVectorReader.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkDelaunay3D.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkExtractUnstructuredGrid.h>
#include "vtkPlaneSource.h"
#include "vtkConeSource.h"
#include <vtkQuadric.h>
#include <vtkFillHolesFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkLinearSubdivisionFilter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkSmoothPolyDataFilter.h>

// vtkLookupTable 颜色插值数
#define LT_COLOR_NUM		240
class vPointWidget;
class vtkNodeCallback;

class GraphNode : public SurperNode
{
public:
	GraphNode(std::shared_ptr< XmlProject> project);
	virtual ~GraphNode();

	virtual std::shared_ptr<SuperNodeStruct> getParameter();
	virtual void setInputData(std::vector<pointFinal> &data);
	virtual bool getOnputData(std::vector<pointFinal> &data);
	virtual void setInputData(std::vector<pointOriginal> &data, int dataIndex = 0);
	virtual void setOriginData(POINT_FINAL_VEC *data) {
		origin_data = data;
	}
	virtual void setDataRange(double* dataRange);
	virtual void setResolution(int * slt);
	virtual void setLimits(double * limits);
	int setLut( int index, double range[2], Child_labelMark&, bool enLabelMark= true);
	virtual void setColorBarOn(bool checked);	// yzw 0810
	void setColorBar(Child_ColorBar &m_colorBar);
	virtual void updateDisplayColorBar(Child_ColorBar&,Child_ColorBar&);

	virtual bool set_progress(int range, int pos, const char*);
	virtual bool run() = 0;
	virtual bool nextNodeRun(int8_t nodeId);
	virtual bool saveData(std::string path);
	virtual void getBound(double bounds[6]) = 0;
	virtual void setBound(double* bounds) = 0;
	virtual bool addToRender() = 0; // yzw 0718
	virtual bool removeFromRender() = 0;
	virtual void setChecked(bool checked);
	virtual void setFocused(bool checked);
	virtual void updateDisplay(std::shared_ptr<SuperNodeStruct>) = 0;
	virtual vtkSmartPointer<vtkProp> getActor() = 0;

public:
	// 切面相关
	double origin_cmd[3];
	virtual void setNewPos(double*);
	vtkSmartPointer<vtkProbeFilter> probe;
	vtkSmartPointer<vtkNodeCallback> nodeCallback;
	vtkSmartPointer<vtkSphereSource> sphereSource;
	vtkSmartPointer<vtkPolyData> inputPolyData;
	vtkSmartPointer<vtkPolyData> point;
	vtkSmartPointer<vtkSphereSource> sphere;
	vtkSmartPointer<vtkGlyph3D> glyph;
	vtkSmartPointer<vtkPolyDataMapper> glyphMapper;
	vtkSmartPointer<vtkActor> glyphActor;
	vtkSmartPointer<vPointWidget> pointWidget = nullptr;
	bool dragger_en = false;
	void draggerOn(bool s = true);
	vtkSmartPointer<vtkActor> tri_point[3];
	int tri_point_n = 0;
	vtkSmartPointer<vtkPlane> m_cutterPlane;
protected:
	 vtkSmartPointer<vtkLookupTable> m_lut;
	//void beginRun();
	std::vector<pointFinal> m_data;
	POINT_FINAL_VEC *origin_data;
	double m_dataRange[6];
	vtkSmartPointer<vtkScalarBarActor> m_scalarBar; // 0810 
	int	m_ranged = 0;
};

class vPointWidget :public vtkPointWidget
{
public:
	vPointWidget();
	~vPointWidget();
	static vPointWidget* New();
	GraphNode* node;
};

// This does the actual work: updates the probe.
// Callback for the interaction
class vtkNodeCallback : public vtkCommand
{
public:
	vtkNodeCallback() = default;
	GraphNode* pNode;

	static vtkNodeCallback* New()
	{
		return new vtkNodeCallback;
	}
	virtual void Execute(vtkObject* caller, unsigned long eId, void* call_data)
	{
		std::string className=caller->GetClassName();
		if(className== "vtkPointWidget"){
			vPointWidget* pointWidget = reinterpret_cast<vPointWidget*>(caller);
			GraphNode* pn = pNode;// pointWidget->node;
			//pointWidget->GetPolyData(this->PolyData);
			//double position[3];
			double* position = pn->origin_cmd;
			pointWidget->GetPosition(position);
			pn->setNewPos(position);
			//std::ostringstream text;
			//text << "cursor: " << std::fixed << std::setprecision(4) << position[0]
			//	<< ", " << position[1] << ", " << position[2];
			//printf("%s\n", text.str().c_str());
			//std::cout << text.str().c_str()<< std::endl;
			//std::cout.flush();
			//if (PositionActor)
			//	this->PositionActor->SetInput(text.str().c_str());

			//this->CursorActor->VisibilityOn();
		}
		else if(eId== vtkCommand::ProgressEvent)// (className.find( "PolyDataMapper")) 
		{
			double pg = *(double*)call_data;
			int r = pg * 10000;
			//pNode->set_progress(10000, r, nullptr);
		}
	}

	vtkPolyData* PolyData = nullptr;
	//vtkActor* CursorActor = nullptr;
	//vtkTextActor* PositionActor = nullptr;
};

// VTK可移动三维坐标轴 vtkMovableAxesWidget
// https://blog.csdn.net/weixin_38416696/article/details/124987253?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-124987253-blog-95938345.pc_relevant_3mothn_strategy_recovery&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-124987253-blog-95938345.pc_relevant_3mothn_strategy_recovery&utm_relevant_index=1

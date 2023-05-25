#include "GHighLowMapNode.h"
#include "XmlProject.h"
#include <fstream>
#include <vector>
#include "NodeParaters.h"

#include <vtkImageShrink3D.h>
#include <vtkSimpleElevationFilter.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkWarpScalar.h>
#include <vtkElevationFilter.h>
#include <vtkCastToConcrete.h>
#include <vtkPolyDataNormals.h>
#include <vtkDEMReader.h>
#include <vtkTexture.h>
#include <vtkJPEGReader.h>
#include <vtkTransformTextureCoords.h>
#include <vtkTextureMapToPlane.h>
//#include <vtkPBRPrefilterTexture.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkExtractVOI.h>
#include "vtkPlaneSource.h"
#include "vtkFitToHeightMapFilter.h"
#include "vtkProbeFilter.h"
#include <vtkGlyph3D.h>
#include <vtkMaskPoints.h>
#include <vtkArrowSource.h>


GHighLowMapNode::GHighLowMapNode(std::shared_ptr<XmlProject> project):GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_HIGHANDLOWDIAGRAM_TYPE;
	m_actor = vtkSmartPointer<vtkActor>::New();
	m_imageData = vtkSmartPointer<vtkImageData>::New();
	//m_lut = vtkSmartPointer<vtkLookupTable>::New();
	m_mapper = vtkSmartPointer<vtkDataSetMapper>::New();

}


GHighLowMapNode::~GHighLowMapNode()
{
}

std::shared_ptr<TiXmlNode> GHighLowMapNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_highLowPicture.toXml(pParameters);
	pElemNode->InsertEndChild(*pParameters);
	return pElemNode;
}

bool GHighLowMapNode::readParameterFromXmlNode(TiXmlNode* node)
{

#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	TiXmlElement* pParaters = ((TiXmlElement*)node)->FirstChildElement("Parameters");
	if (!pParaters)
		return SetProjectError
		m_highLowPicture.fromXml(pParaters);
	return true;
}

std::shared_ptr<SuperNodeStruct> GHighLowMapNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_HighLowZoneMap> data = std::make_shared<G_HighLowZoneMap>();
	//memcpy_s(mesh.get(), sizeof(C_Mesh), &m_mesh, sizeof(C_Mesh));
	*data = m_highLowPicture;
	SurperNode::getParameter(data);
	return data;
}

/* 
	std::string m_putIn;//输入网格点阵
	//xxx   输入覆盖叠加图？//这个没明白啥意思，你看下补充下
	uint16_t m_putIndex;//输入分量
	CutPlaneDirection m_direction;//摆放方向  三维数据输入特有
	int m_setionId;//切片id  三维数据输入特有
	float m_scaleFactor;//比例因子
	std::string      textureFile;        //纹理照片
	Child_Render_HighLowMap m_renderInfo;//高低区域图渲染信息
	Child_Map_Overlapped m_mapOverlapped;//图像叠加
	Vector_Overlay m_verctorOverlay;//矢量图覆盖
	Child_ColorBar m_colorBar; // 标刻度尺*/
bool GHighLowMapNode::setParameter(std::shared_ptr<SuperNodeStruct> param) {
	std::shared_ptr<G_HighLowZoneMap> highLowZoneMap = std::dynamic_pointer_cast<G_HighLowZoneMap>(param);
	SurperNode::setParameter(param);
	m_highLowPicture = *highLowZoneMap;
	m_highLowPicture.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return true;
}

void GHighLowMapNode::getDefault(SuperNodeStruct* st)
{

}

bool GHighLowMapNode::run() {
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return false;
	if (m_excuted)
		return true;
#if 0
	
#endif
	auto dem = vtkSmartPointer<vtkDEMReader>::New();
	vtkImageData* feildData;

	dem->SetFileName(m_highLowPicture.demFile.c_str());
	dem->Update();

	feildData = dem->GetOutput();

	auto ImageDataGeometry = vtkSmartPointer<vtkImageDataGeometryFilter>::New();
	ImageDataGeometry->SetInputData(feildData);
	ImageDataGeometry->Update();

	double Scale = 0;
	double Diagonal = sqrt((feildData->GetDimensions()[0] - 1) * feildData->GetSpacing()[0] * (feildData->GetDimensions()[0] - 1) * feildData->GetSpacing()[0] +
		(feildData->GetDimensions()[1] - 1) * feildData->GetSpacing()[1] * (feildData->GetDimensions()[1] - 1) * feildData->GetSpacing()[1] +
		(feildData->GetDimensions()[2] - 1) * feildData->GetSpacing()[2] * (feildData->GetDimensions()[2] - 1) * feildData->GetSpacing()[2]) / 2.0;

	Scale = m_highLowPicture.m_scaleFactor / (feildData->GetScalarRange()[1] - feildData->GetScalarRange()[0]) * Diagonal;

	auto warpScalar = vtkSmartPointer<vtkWarpScalar>::New();
	warpScalar->SetInputConnection(ImageDataGeometry->GetOutputPort());
	warpScalar->UseNormalOn();
	warpScalar->SetNormal(0, 0, 1);
	warpScalar->SetScaleFactor(Scale);
	warpScalar->Update();

	//auto lut = vtkSmartPointer<vtkLookupTable>::New();
	m_lut->SetHueRange(0.6, 0);
	m_lut->SetSaturationRange(1.0, 0);
	m_lut->SetValueRange(0.5, 1.0);

	auto texturemap = vtkSmartPointer<vtkTextureMapToPlane>::New();
	texturemap->SetInputConnection(warpScalar->GetOutputPort());
	texturemap->Update();

	auto transformTexture = vtkSmartPointer<vtkTransformTextureCoords>::New();
	transformTexture->SetInputConnection(texturemap->GetOutputPort());
	transformTexture->SetScale(1, 1, 1);
	transformTexture->Update();

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(transformTexture->GetOutputPort());
	//mapper->SetLookupTable(lut);
	mapper->SetScalarRange(feildData->GetScalarRange());

	auto texReader = vtkSmartPointer<vtkJPEGReader>::New();
	texReader->SetFileName(m_highLowPicture.textureFile.c_str());//纹理图片
	texReader->Update();

	auto texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(texReader->GetOutputPort());
	texture->SetRepeat(false);
	if (m_highLowPicture.m_mapOverlapped.m_suitedType == Child_Suited_type::Stretching_On_Surface)
	{
		texture->SetEdgeClamp(true);
	}
	if (m_highLowPicture.m_mapOverlapped.m_suitedType == Child_Suited_type::Clip_To_Surface)
	{
		texture->SetEdgeClamp(false);
	}

	if (m_highLowPicture.m_mapOverlapped.m_modulation == Child_Color_Modulation::BlendSurfaceandOverlayColors)
	{
		texture->SetColorMode(VTK_COLOR_MODE_DEFAULT);
	}
	if (m_highLowPicture.m_mapOverlapped.m_modulation == Child_Color_Modulation::OverlayColorOnly)
	{
		texture->SetColorMode(VTK_COLOR_MODE_MAP_SCALARS);
	}
	if (m_highLowPicture.m_mapOverlapped.m_modulation == Child_Color_Modulation::ColorMapOnly)
	{
		texture->SetColorMode(VTK_COLOR_MODE_DIRECT_SCALARS);
	}
	texture->InterpolateOff();

	m_actor = vtkSmartPointer<vtkActor>::New();
	m_actor->SetMapper(mapper);
	if (m_highLowPicture.m_mapOverlapped.m_showCover)
	{
		m_actor->SetTexture(texture);
	}
	if (m_highLowPicture.m_renderInfo.m_drawStyle == child_draw_style::Line_shape)
	{
		m_actor->GetProperty()->SetRepresentationToWireframe();
	}
	if (m_highLowPicture.m_renderInfo.m_drawStyle == child_draw_style::Scatter_shape)
	{
		m_actor->GetProperty()->SetRepresentationToPoints();
	}
	if (m_highLowPicture.m_renderInfo.m_drawStyle == child_draw_style::Shadow_shape)
	{
		m_actor->GetProperty()->SetRepresentationToSurface();
	}
	m_actor->GetProperty()->SetOpacity(m_highLowPicture.m_renderInfo.m_opacity);


	if(m_checked){
		auto renderer = m_project.lock()->getRender();
		renderer->AddActor(m_actor);
		renderer->Render();
	}
	m_excuted = true;
	//makeNextNodeRun();
	return true;
}

bool GHighLowMapNode::addToRender()	// yzw 0712
{
	if (m_excuted)
		m_project.lock()->getRender()->AddActor(m_actor);
	return m_excuted;
}

bool GHighLowMapNode::removeFromRender() {
	if(m_excuted)
		m_project.lock()->getRender()->RemoveActor(m_actor);
	return m_excuted;
}

void GHighLowMapNode::updateDisplay(std::shared_ptr<SuperNodeStruct> highlowmap)
{

}

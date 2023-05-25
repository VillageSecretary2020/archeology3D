// GGeotiffNode.cpp

#include <QApplication>
#include <QCursor>

#include "GGeotiffNode.h"
#include "XmlProject.h"
#include <fstream>
#include <vector>
#include "NodeParaters.h"
#include "geotiff_property_widget.h"

#include <vtkImageShrink3D.h>
#include <vtkSimpleElevationFilter.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkWarpScalar.h>
#include <vtkElevationFilter.h>
#include <vtkCastToConcrete.h>
#include <vtkPolyDataNormals.h>
#include <vtkDEMReader.h>
#include <vtkTexture.h>
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

GGeotiffNode::GGeotiffNode(std::shared_ptr<XmlProject> project):GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_GEOTIFF_TYPE;
	tifReader = vtkSmartPointer<vtkTIFFReader>::New();
	rasterReader = vtkGDALRasterReader::New();
	boxActor =		vtkSmartPointer<vtkActor>::New();
	picExtActor =		vtkSmartPointer<vtkImageActor>::New();
#if 0
	probe=vtkSmartPointer<vtkProbeFilter>::New() ;
	nodeCallback=vtkSmartPointer<vtkNodeCallback>::New() ;
	sphereSource=vtkSmartPointer<vtkSphereSource>::New(); ;
	point= vtkSmartPointer<vtkPolyData>::New() ;
	glyph=vtkSmartPointer<vtkGlyph3D>::New() ;
	glyphMapper=vtkSmartPointer<vtkPolyDataMapper> ::New();
	glyphActor = vtkSmartPointer<vtkActor>::New();
#endif
}

GGeotiffNode::~GGeotiffNode()
{
}

std::shared_ptr<TiXmlNode> GGeotiffNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_picture.toXml(pParameters);
	pElemNode->InsertEndChild(*pParameters);
	return pElemNode;
}

bool GGeotiffNode::readParameterFromXmlNode(TiXmlNode* node)
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
		m_picture.fromXml(pParaters);
	return true;
}

// 探测器上鼠标当前的坐标
void GGeotiffNode::setNewPos(double* pos)
{
#if 0
	if (tri_point_n)
		tri_point[tri_point_n-1]->SetPosition(pos);
	if (tri_point_n==1)
		boxActor->SetPosition(pos);
#endif
	if (widget) {
		int* dim= rasterReader->GetRasterDimensions();
		dim[0] = pos[0];
		dim[1] = -pos[1];	// 因为y轴坐标系已翻转
		const double *ppos= rasterReader->GetGeoCornerPoints();
		double cur_pos[2] = { ppos[4], ppos[5] };
		geotiff_property_widget* pw = (geotiff_property_widget*)widget;
		pw->curPos(cur_pos);
	}
}

void GGeotiffNode::setPicPos(double* pos)
{
#if 0
#else
		picExtActor->SetUserTransform(nullptr);
		double delta[3] = { m_dataRange[0] - raster_pos[0],m_dataRange[1] - raster_pos[1], 0.0 };
		delta[0] /= tif_scale[0];
		delta[1] /= tif_scale[1];
	if(!m_picture.apply){
		double cur_pos[3] = { m_dataRange[0] - delta[0], m_dataRange[1]- delta[1], 0.0}; // 坐标匹配时在坐标位置显示
		if (m_picture.adopt > 0){ // 非坐标匹配时在{0.0, 0.0, 0.0}显示
			cur_pos[0] = 0.0;		// 坐标探测器pointWidget在{0.0, 0.0, 0.0}，picExtActor也在此位置
			cur_pos[1] = 0.0;
		}
		picExtActor->SetPosition(cur_pos);
		return;
	}

	double cur_pos[3] = {0.0, 0.0, 0.0 };
	double box_pos[3];
		box_pos[0] = (bounds[1]- bounds[0]) * m_picture.xyz_offset[0];	// dims[0]
		box_pos[1] = (bounds[3]- bounds[2]) * m_picture.xyz_offset[1]; // dims[1]
		box_pos[2] = (m_dataRange[5] - m_dataRange[2])* m_picture.xyz_offset[2] ;// (*1.0 / SLIDER_OFFSET_MID - 1)*pScale

		if (m_picture.adopt == 0) {
			cur_pos[0] = m_dataRange[0];
			cur_pos[1] = m_dataRange[1];
		}
		delta[0] += box_pos[0];
		delta[1] += box_pos[1];
		cur_pos[0] -= delta[0];
		cur_pos[1] -= delta[1];
		picExtActor->SetPosition(cur_pos);
		double center[3] = {
			m_dataRange[0],
			m_dataRange[1],
			0.0
		};
		if (m_picture.adopt > 0) { // 非坐标匹配
			center[0] = delta[0];	// 
			center[1] = delta[1];
		}
#if 1
		//double scale[3] = { 1.0/ m_picture.xyz_scale[0], 1.0/ m_picture.xyz_scale[1],  1.0 / m_picture.xyz_scale[2] };
		double scale[3] = {tif_scale[0]/ m_picture.xyz_scale[0],tif_scale[1]/ m_picture.xyz_scale[1],  1.0 / m_picture.xyz_scale[2] };
		vtkSmartPointer<vtkTransform> myTrans = vtkTransform::New();
		myTrans->Translate(center);
		myTrans->Scale(scale);
		myTrans->RotateZ(-m_picture.xyz_offset[3]);
		myTrans->Translate(-center[0] , -center[1] , 0 );
		picExtActor->SetUserTransform(myTrans);
#endif
#endif
}

// setOffset
void GGeotiffNode::setBoxPos()
{
	if(m_picture.apply){
		boxActor->VisibilityOff();
		return;
	}

	double cur_pos[3] = {0.0, 0.0, 0.0 };
	double delta[3] = {0.0, 0.0, 0.0 };
	if (m_picture.adopt > 0) { // 非坐标匹配，移到{0,0,0}处显示
		cur_pos[0] = -m_dataRange[0];
		cur_pos[1] = -m_dataRange[1];
		delta[0] = m_dataRange[0]-raster_pos[0];
		delta[1] = m_dataRange[1]-raster_pos[1];
		delta[0] /= tif_scale[0];
		delta[1] /= tif_scale[1];
	}
	double box_pos[3];	//偏移量
	box_pos[0] = (bounds[1] - bounds[0]) * m_picture.xyz_offset[0];// *tif_scale[0]; // dims[0]
	box_pos[1] = (bounds[3] - bounds[2]) * m_picture.xyz_offset[1];// *tif_scale[1]; // dims[1]
		box_pos[2] = (*(m_dataRange + 5) - *(m_dataRange + 2))* m_picture.xyz_offset[2] ; //(*1.0 / SLIDER_OFFSET_MID - 1)  *pScale

		delta[0] += box_pos[0];
		delta[1] += box_pos[1];
		cur_pos[0] += delta[0];
		cur_pos[1] += delta[1];
		boxActor->SetPosition(cur_pos);	// 偏移
#if 1
		double center[3] = {
			m_dataRange[0] + delta[0],
			m_dataRange[1] + delta[1],
			0.0
		};
		if (m_picture.adopt > 0) { // 非坐标匹配
			center[0] = delta[0];	// 
			center[1] = delta[1];
		}
		vtkSmartPointer<vtkTransform> myTrans = vtkTransform::New();
		myTrans->Translate(center);
		myTrans->Scale(m_picture.xyz_scale[0]/tif_scale[0], m_picture.xyz_scale[1] / tif_scale[1], m_picture.xyz_scale[2] );
		myTrans->RotateZ(m_picture.xyz_offset[3] );
		myTrans->Translate(-center[0], -center[1], 0);
		boxActor->SetUserTransform(myTrans);
#endif
		boxActor->VisibilityOn();
}

void GGeotiffNode::setPictureFile(std::string fn)
{
	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	 inputPolyData= vtkSmartPointer<vtkPolyData>::New();
	 vtkImageData* imgData;

	int dims[3];
	char ext[10];
	_splitpath(fn.data(), nullptr, nullptr, nullptr, ext);
	auto compareStr = [&](std::string str1, std::string str2) {
		if (str1.length() != str2.length())
			return false;
		return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
			return tolower(a) == tolower(b); });
	};
		double ds[3];
		const char* fname =  fn.c_str(); // "d:/devel/tongling.tif";// 
	//if (compareStr(ext, ".tif"))
	{
		int cr= rasterReader->CanReadFile(fname);
		rasterReader->SetFileName(fname);
		rasterReader->Update();
		int* rDim= rasterReader->GetRasterDimensions();
		memcpy_s(raster_dim, sizeof(raster_dim), rDim, sizeof(raster_dim));
		const double *ppos= rasterReader->GetGeoCornerPoints();
		memcpy_s(raster_pos, sizeof(raster_pos), ppos, sizeof(raster_pos));
		tif_scale[0] = (raster_pos[4] - raster_pos[0]) / raster_dim[0];	//每像素的距离间隔
		tif_scale[1] = (raster_pos[1] - raster_pos[5]) / raster_dim[1];
	}{
		double scale[3] = { 1.0, -1.0, 1.0 };	// y轴坐标系翻转以适应VTK
		tifReader->SetFileName(fname);
		tifReader->SetDataSpacing(scale);
		tifReader->SpacingSpecifiedFlagOn();
		tifReader->Update();
		tifReader->GetDataSpacing(ds);// tif默认不为1.0 ？
		imgData = tifReader->GetOutput();
		imgData->GetDimensions(dims);
		outPort = tifReader->GetOutputPort();
		bounds[0] = 0; bounds[1] = dims[0];
		bounds[2] = 0; bounds[3] = dims[1];
		bounds[4] = 0; bounds[5] = dims[2];
		}
#if 1
	//imgData->GetDimensions(dims);
	vtkIdType tn = imgData->GetNumberOfPoints();
	for (vtkIdType id = 0; id < tn; id++) {
		points->InsertNextPoint(imgData->GetPoint(id));
		cells->InsertNextCell(1, &id);
	}
	inputPolyData->SetPoints(points);
	inputPolyData->SetVerts(cells);
	;
	//probe->SetInputData(point);
	//probe->SetSourceData(inputPolyData);
#endif
}

//void GPictureNode::setInputData(std::vector<pointOriginal>& data, int dataIndex)
	//GraphNode::setInputData(data, dataIndex);
void GGeotiffNode::setDataRange(double* dataRange)
{
	m_excuted = false;
	memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	setPictureFile(m_picture.picFilePath);
	QApplication::restoreOverrideCursor();
#if 0
	sphereSource->Update();

	glyph->SetInputConnection(probe->GetOutputPort());
	glyph->SetSourceConnection(sphereSource->GetOutputPort());//cone 
	glyph->SetVectorModeToUseVector();
	glyph->SetScaleModeToDataScalingOff();
	glyph->SetScaleFactor( 0.4);

	glyphMapper->SetInputConnection(glyph->GetOutputPort());

	glyphActor->SetMapper(glyphMapper);
	glyphActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	glyphActor->VisibilityOn();

	sphere->SetRadius(4);
	sphere->Update();
	vtkPolyData* polydata1 = sphere->GetOutput();
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputData(polydata1);
	tri_point[0]->SetMapper(mapper); tri_point[0]->VisibilityOff();
	tri_point[1]->SetMapper(mapper); tri_point[1]->VisibilityOff();
	tri_point[2]->SetMapper(mapper); tri_point[2]->VisibilityOff();
#endif
	picExtActor->GetPosition(first_pos);	
	vtkSmartPointer<vtkOutlineSource> m_outline = vtkSmartPointer<vtkOutlineSource>::New();
	m_outline->SetBounds(dataRange[0], dataRange[3], dataRange[1], dataRange[4], dataRange[2], dataRange[5]);
	m_outline->Update();
	vtkSmartPointer<vtkPolyDataMapper> m_outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_outlineMapper->SetInputConnection(m_outline->GetOutputPort());
	//m_actor = vtkSmartPointer<vtkActor>::New();
	boxActor->SetMapper(m_outlineMapper);
	boxActor->GetProperty()->SetLineWidth(1.0);
	boxActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
}

std::shared_ptr<SuperNodeStruct> GGeotiffNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_Geotiff> data = std::make_shared<G_Geotiff>();
	*data = m_picture;
	SurperNode::getParameter(data);
	return data;
}

//class MouseInteractorHighLightActor;
bool GGeotiffNode::setParameter(std::shared_ptr<SuperNodeStruct> param) {
	std::shared_ptr<G_Geotiff> pic = std::dynamic_pointer_cast<G_Geotiff>(param);
	SurperNode::setParameter(param);
	m_picture = *pic;
	if(param->m_changetype == G_Picture_MN_CT+ 20){
		param->m_changetype = 0;
		m_picture.adopt = 0;
		//return false;
	}else	if(param->m_changetype == G_Picture_MN_CT+ 21){
		param->m_changetype = 0;
		m_picture.adopt = 1;
		//return false;
	}
#if 0
	else if(param->m_changetype == G_Picture_MN_CT + 22){
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point_n = 0;
		return false;
	}
	else if(param->m_changetype == G_Picture_MN_CT + 23){
		tri_point[0]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		pointWidget->SetPosition(tri_point[0]->GetPosition());
		tri_point_n = 1;
		return false;
	}
	else if(param->m_changetype == G_Picture_MN_CT + 24){
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[1]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		pointWidget->SetPosition(tri_point[1]->GetPosition());
		tri_point_n = 2;
		return false;
	}
	else if(param->m_changetype ==G_Picture_MN_CT + 25){
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[2]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		pointWidget->SetPosition(tri_point[2]->GetPosition());
		tri_point_n = 3;
		return false;
	}
#endif
	else if (param->m_changetype == G_Picture_MN_CT + 40) {
		param->m_changetype = 0;
		draggerOn(false);
		return false;
	}
	else	if (param->m_changetype == G_Picture_MN_CT + 41) {
		param->m_changetype = 0;
		draggerOn();
		return false;
	}
	else if (param->m_changetype == G_Picture_MN_CT + 42) {
		param->m_changetype = 0;
		picExtActor->SetOpacity(m_picture.xy_range[5] / 1000.0);
		return false;
	}
	else	if(param->m_changetype == G_Picture_MN_CT+ 0x80){
		setPicPos(first_pos);
		setBoxPos();
		if (m_picture.apply) {
			draggerOn(false);
		}
		return false;
	}
	else	if(param->m_changetype == G_Picture_MN_CT+ 0xf0){
		setPictureFile(pic->picFilePath);
		//return false;
	}
	m_picture.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return true;
}

void GGeotiffNode::getDefault(SuperNodeStruct* st)
{
	G_Geotiff* pst = (G_Geotiff*)st;
	pst->adopt = 0;
	pst->xy_range[0] = 0;
	pst->xy_range[1] = 999;
	pst->xy_range[2] = 0;
	pst->xy_range[3] = 999;
	pst->xy_range[5] = 800;	// opacity
	pst->apply = false;
	pst->xyz_offset[0] = 0.0;
	pst->xyz_offset[1] = 0.00;
	pst->xyz_offset[2] = 0.0;
	pst->xyz_offset[3] = 0.0;//zRotate
	pst->xyz_scale[0] = 1.0;
	pst->xyz_scale[1] = 1.0;
	pst->xyz_scale[2] = 1.0;
}

bool GGeotiffNode::run() {
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return false;
	if (m_excuted)
		return true;
		{
			int range[4];
			if(m_picture.xy_range[0]< m_picture.xy_range[1]){
				range[0] = m_picture.xy_range[0];		range[1] = m_picture.xy_range[1];
			}else{
				range[1] = m_picture.xy_range[0];		range[0] = m_picture.xy_range[1];
			}
			if(m_picture.xy_range[2]< m_picture.xy_range[3]){
				range[2] = m_picture.xy_range[2];		range[3] = m_picture.xy_range[3];
			}else{
				range[3] = m_picture.xy_range[2];		range[2] = m_picture.xy_range[3];
			}
			vtkSmartPointer<vtkExtractVOI> pImgExtVOI = vtkSmartPointer<vtkExtractVOI>::New();
			pImgExtVOI->SetInputConnection(outPort);// (texReader->GetOutputPort());
			pImgExtVOI->SetVOI(
				bounds[0]+(bounds[1] - bounds[0])*range[0]/1000, bounds[0]+(bounds[1] - bounds[0])*range[1]/1000,
				bounds[2]+(bounds[3] - bounds[2])*range[2]/1000, bounds[2]+(bounds[3] - bounds[2])*range[3]/1000,
				//dims[2]/3, dims[2]*2/3
				bounds[4], bounds[5]
			);
			pImgExtVOI->Update();
			picExtActor->SetInputData(pImgExtVOI->GetOutput());
			picExtActor->SetOpacity(m_picture.xy_range[5]/1000.0);
			if(dragger_en){
				draggerOn();
			}

			setPicPos(first_pos);
			setBoxPos();
		}
	
	if(m_checked){
		auto renderer = m_project.lock()->getRender();
		//renderer->AddActor(picActor);
		renderer->AddActor(picExtActor);
		//renderer->AddActor(tri_point[0]);
		//renderer->AddActor(tri_point[1]);
		//renderer->AddActor(tri_point[2]);
		renderer->AddActor(boxActor);
		//renderer->Render();
	}
	m_excuted = true;
	//makeNextNodeRun();
	return true;
}

bool GGeotiffNode::addToRender()	// yzw 0712
{
	if (m_excuted){
		auto renderer = m_project.lock()->getRender();
		//renderer->AddActor(picActor);
		renderer->AddActor(picExtActor);
		//renderer->AddActor(tri_point[0]);
		//renderer->AddActor(tri_point[1]);
		//renderer->AddActor(tri_point[2]);
		renderer->AddActor(boxActor);
	}
	return m_excuted;
}

bool GGeotiffNode::removeFromRender() {
	if(m_excuted){
		auto renderer = m_project.lock()->getRender();
		//renderer->RemoveActor(picActor);
		renderer->RemoveActor(picExtActor);
		//renderer->RemoveActor(tri_point[0]);
		//renderer->RemoveActor(tri_point[1]);
		renderer->RemoveActor(tri_point[2]);
		renderer->RemoveActor(boxActor);
	}
	return m_excuted;
}

void GGeotiffNode::updateDisplay(std::shared_ptr<SuperNodeStruct> highlowmap)
{

}

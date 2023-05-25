// GPictureNode.cpp

#include <QApplication>
#include <QCursor>

#include "GPictureNode.h"
#include "XmlProject.h"
#include <fstream>
#include <vector>
#include "NodeParaters.h"
#include "picture_property_widget.h"

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
#include "vtkFitToHeightMapFilter.h"
#include "vtkProbeFilter.h"
#include <vtkGlyph3D.h>
#include <vtkMaskPoints.h>
#include <vtkArrowSource.h>

GPictureNode::GPictureNode(std::shared_ptr<XmlProject> project):GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_PICTURE_TYPE;
	jpegReader = vtkSmartPointer<vtkJPEGReader>::New();
	tifReader = vtkSmartPointer<vtkTIFFReader>::New();
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

GPictureNode::~GPictureNode()
{
}

std::shared_ptr<TiXmlNode> GPictureNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_picture.toXml(pParameters);
	pElemNode->InsertEndChild(*pParameters);
	return pElemNode;
}

bool GPictureNode::readParameterFromXmlNode(TiXmlNode* node)
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

void GPictureNode::setPicPos(double* npos)
{
#if 0
#else
		double cur_pos[3], box_pos[3];
		box_pos[0] = (bounds[1]- bounds[0]) * m_picture.xyz_offset[0];	// dims[0]
		box_pos[1] = (bounds[3]- bounds[2]) * m_picture.xyz_offset[1]; // dims[1]
		box_pos[2] = (*(m_dataRange + 5) - *(m_dataRange + 2))* m_picture.xyz_offset[2] /m_picture.xyz_scale[2];// (*1.0 / SLIDER_OFFSET_MID - 1)*pScale

		picExtActor->SetPosition(m_dataRange[0] - box_pos[0], m_dataRange[1] - box_pos[1], m_dataRange[2] - box_pos[2]);
		if (!m_picture.apply) {
			picExtActor->SetUserTransform(nullptr);
			return;
		}
		double scale[3] = {1.0/ m_picture.xyz_scale[0], 1.0 / m_picture.xyz_scale[1],  1.0 / m_picture.xyz_scale[2] };
		vtkSmartPointer < vtkTransform> myTrans = vtkSmartPointer < vtkTransform>::New();
		myTrans->Translate(m_dataRange[0] , m_dataRange[1] , m_dataRange[2] );
		myTrans->Scale(scale);
		myTrans->RotateZ(-m_picture.xyz_offset[3]);
		myTrans->Translate(-m_dataRange[0] , -m_dataRange[1] , -m_dataRange[2] );
		picExtActor->SetUserTransform(myTrans);
		picExtActor->GetPosition(cur_pos);

		boxActor->VisibilityOff();
#endif
}
#if 0
void GPictureNode::setOffset()
{
		double cur_pos[3], box_pos[3];
		box_pos[0] = (bounds[1]- bounds[0]) * m_picture.xyz_offset[0]; // dims[0]
		box_pos[1] = (bounds[3]- bounds[2]) * m_picture.xyz_offset[1]; // dims[1]
		box_pos[2] = (*(m_dataRange + 5) - *(m_dataRange + 2))* m_picture.xyz_offset[2] * m_picture.xyz_scale[2]; //(*1.0 / SLIDER_OFFSET_MID - 1)  *pScale
		boxActor->VisibilityOn();
		boxActor->GetPosition(cur_pos);
		vtkTransform *myTrans = vtkTransform::New();
		myTrans->Translate(box_pos[0] -cur_pos[0], box_pos[1] -cur_pos[1], box_pos[2] -cur_pos[2]);
		myTrans->Scale(m_picture.xyz_scale);
		myTrans->RotateZ(m_picture.xyz_offset[3] );
		boxActor->SetUserTransform(myTrans);

		picExtActor->SetUserTransform(nullptr);
}
#else
void GPictureNode::setBoxPos()
{
	if (m_picture.apply) {
		boxActor->VisibilityOff();
		return;
	}

	vtkSmartPointer < vtkTransform> myTrans = vtkSmartPointer < vtkTransform>::New();
	myTrans->Translate(m_dataRange[0], m_dataRange[1], m_dataRange[2]);
	myTrans->Scale(m_picture.xyz_scale);
	myTrans->RotateZ(m_picture.xyz_offset[3]);
	myTrans->Translate(-m_dataRange[0], -m_dataRange[1], -m_dataRange[2]);
	boxActor->SetUserTransform(myTrans);
	boxActor->VisibilityOn();

}
#endif

void GPictureNode::setPictureFile(std::string fn)
{
	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	 inputPolyData= vtkSmartPointer<vtkPolyData>::New();

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
	if (compareStr(ext, ".jpg") || compareStr(ext, ".jpeg"))
	{
		jpegReader->SetFileName(fn.c_str());
		//jpegReader->SetDataSpacing(scale);
		//jpegReader->SpacingSpecifiedFlagOn();
		jpegReader->Update();
		jpegReader->GetDataSpacing(ds);	// jpeg默认为1.0
		jpegReader->GetOutput()->GetDimensions(dims);
		outPort = jpegReader->GetOutputPort();
		bounds[0] = 0; bounds[1] = dims[0];
		bounds[2] = 0; bounds[3] = dims[1];
		bounds[4] = 0; bounds[5] = dims[2];
	}
	else if (compareStr(ext, ".tif"))
	{
		double scale[3] = { 1.0, -1.0, 1.0 };//
		tifReader->SetFileName(fn.c_str());
		tifReader->SetDataSpacing(scale);
		tifReader->SpacingSpecifiedFlagOn();
		tifReader->Update();
		tifReader->GetDataSpacing(ds);// tif默认不为1.0？
		tifReader->GetOutput()->GetDimensions(dims);
		outPort = tifReader->GetOutputPort();
		bounds[0] = 0; bounds[1] = dims[0];
		bounds[2] = 0; bounds[3] = dims[1];
		bounds[4] = 0; bounds[5] = dims[2];
	}
}

//void GPictureNode::setInputData(std::vector<pointOriginal>& data, int dataIndex)
	//GraphNode::setInputData(data, dataIndex);
void GPictureNode::setDataRange(double* dataRange)
{
	m_excuted = false;
	memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	setPictureFile(m_picture.picFilePath);
	QApplication::restoreOverrideCursor();
	sphere->SetRadius(4);
	sphere->Update();
	vtkPolyData* polydata1 = sphere->GetOutput();
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputData(polydata1);
	//tri_point[0]->SetMapper(mapper); tri_point[0]->VisibilityOff();
	//tri_point[1]->SetMapper(mapper); tri_point[1]->VisibilityOff();
	//tri_point[2]->SetMapper(mapper); tri_point[2]->VisibilityOff();

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

std::shared_ptr<SuperNodeStruct> GPictureNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_Picture> data = std::make_shared<G_Picture>();
	*data = m_picture;
	SurperNode::getParameter(data);
	return data;
}

//class MouseInteractorHighLightActor;
bool GPictureNode::setParameter(std::shared_ptr<SuperNodeStruct> param) {
	std::shared_ptr<G_Picture> pic = std::dynamic_pointer_cast<G_Picture>(param);
	SurperNode::setParameter(param);
	m_picture = *pic;
#if 0
	if(param->m_changetype == G_Picture_MN_CT+ 20){
		param->m_changetype = 0;
		glyphActor->VisibilityOff();
		draggerOn(false);
		glyphActor->VisibilityOff();
		tri_point[0]->VisibilityOff();
		tri_point[1]->VisibilityOff();
		tri_point[2]->VisibilityOff();
		boxActor->VisibilityOff();
		return false;
	}else	if(param->m_changetype == G_Picture_MN_CT+ 21){
		param->m_changetype = 0;
		glyphActor->VisibilityOn();
		tri_point[0]->VisibilityOn();
		tri_point[1]->VisibilityOn();
		tri_point[2]->VisibilityOn();
		boxActor->VisibilityOn();
		if (tri_point_n)
			tri_point[tri_point_n - 1]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		draggerOn(); 
		return false;
	}
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
	else	if(param->m_changetype == G_Picture_MN_CT+ 0xa0){
		setPicPos(first_pos);
		return false;
	}else	
#endif
	if(param->m_changetype == G_Picture_MN_CT+ 0x80){
		setPicPos(first_pos);
		setBoxPos();
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

void GPictureNode::getDefault(SuperNodeStruct* st)
{
	G_Picture* pst = (G_Picture*)st;
	pst->xy_range[0] = 0;
	pst->xy_range[1] = 999;
	pst->xy_range[2] = 0;
	pst->xy_range[3] = 999;
	pst->apply = false;
	pst->xyz_offset[0] = 0.0;
	pst->xyz_offset[1] = 0.00;
	pst->xyz_offset[2] = 0.0;
	pst->xyz_offset[3] = 0.0;//zRotate
	pst->xyz_scale[0] = 1.0;
	pst->xyz_scale[1] = 1.0;
	pst->xyz_scale[2] = 1.0;
}

bool GPictureNode::run() {
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
			picExtActor->SetOpacity(0.8);
			//if(dragger_en){
			//	//pointWidget->GetPosition(origin_cmd);
			//	//setNewPos(origin_cmd);
			//	draggerOn();
			//}
			//if (m_picture.apply)
				setPicPos(first_pos);
			//else
				setBoxPos();//setOffset();
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

bool GPictureNode::addToRender()	// yzw 0712
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

bool GPictureNode::removeFromRender() {
	if(m_excuted){
		auto renderer = m_project.lock()->getRender();
		//renderer->RemoveActor(picActor);
		renderer->RemoveActor(picExtActor);
		//renderer->RemoveActor(tri_point[0]);
		//renderer->RemoveActor(tri_point[1]);
		//renderer->RemoveActor(tri_point[2]);
		renderer->RemoveActor(boxActor);
	}
	return m_excuted;
}

void GPictureNode::updateDisplay(std::shared_ptr<SuperNodeStruct> highlowmap)
{

}

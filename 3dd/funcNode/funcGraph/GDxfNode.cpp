// GPictureNode.cpp

#include <QApplication>
#include <QCursor>

#include "GDxfNode.h"
#include "XmlProject.h"
#include <fstream>
#include <vector>
#include "NodeParaters.h"
//#include "Interpolation.h"
//#include "istyle.h"
#include "dxf_property_widget.h"

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
//#include "vtkPlaneSource.h"
#include "vtkFitToHeightMapFilter.h"
#include "vtkProbeFilter.h"
#include <vtkGlyph3D.h>
#include <vtkMaskPoints.h>
#include <vtkArrowSource.h>
#include <vtkCompositeDataIterator.h>

GDxfNode::GDxfNode(std::shared_ptr<XmlProject> project):GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_DXF_TYPE;
	dxfReader = vtkGDALVectorReader::New();
	dxfActor =		vtkSmartPointer<vtkActor>::New();
}

GDxfNode::~GDxfNode()
{
}

std::shared_ptr<TiXmlNode> GDxfNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_picture.toXml(pParameters);
	pElemNode->InsertEndChild(*pParameters);
	return pElemNode;
}

bool GDxfNode::readParameterFromXmlNode(TiXmlNode* node)
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

void GDxfNode::setPicPos(double* npos)
{
		if (!m_picture.apply) {
			dxfActor->SetPosition(m_dataRange[0] , m_dataRange[1] , m_dataRange[2] );
			dxfActor->SetUserTransform(nullptr);
			return;
		}
		double scale_x=(m_picture.xyz_scale[0])/(bounds[1]- bounds[0]);//-m_picture.xyz_offset[0]
		double scale[3] = {
			 scale_x,
			( m_picture.xyz_scale[1]) /(bounds[3] - bounds[2]) ,//m_picture.xyz_offset[1] -
			0.1};//scale_x
		double cur_pos[3] = {
			m_picture.xyz_offset[0] -bounds[0],
			m_picture.xyz_offset[1] -m_picture.xyz_scale[1] -bounds[2],
			m_picture.xyz_offset[2]};
		dxfActor->SetPosition(cur_pos );
		double center[3] = {
			m_picture.xyz_offset[0] ,
			m_picture.xyz_offset[1] -m_picture.xyz_scale[1] ,
			m_picture.xyz_offset[2]};
		vtkSmartPointer < vtkTransform> myTrans = vtkSmartPointer < vtkTransform>::New();
		myTrans->Translate(center);
		myTrans->Scale(scale);
		//myTrans->RotateZ(-m_picture.xyz_offset[3]);
		myTrans->Translate(-center[0] , -center[1] , -center[2] );
		dxfActor->SetUserTransform(myTrans);
		dxfActor->GetPosition(cur_pos);
}

void GDxfNode::setPictureFile(std::string fn)
{
	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	 inputPolyData= vtkSmartPointer<vtkPolyData>::New();

	char ext[10];
	_splitpath(fn.data(), nullptr, nullptr, nullptr, ext);
	auto compareStr = [&](std::string str1, std::string str2) {
		if (str1.length() != str2.length())
			return false;
		return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
			return tolower(a) == tolower(b); });
	};
	if (compareStr(ext, ".dxf"))
	{
		double sRange[2] = { 0.0, 9.0 }; int ic = 0;
		dxfReader->SetFileName(fn.c_str());
		dxfReader->Update();
		dxfReader->GetOutput()->GetBounds(bounds);
		if (widget) {
			dxf_property_widget* pw = (dxf_property_widget*)widget;
			pw->set_xlength_ywidth(bounds[1] - bounds[0], bounds[3] - bounds[2]);
		}
		outPort = dxfReader->GetOutputPort();
		vtkSmartPointer<vtkCompositePolyDataMapper> dtmMapper = vtkCompositePolyDataMapper::New();
		dtmMapper->SetInputConnection(dxfReader->GetOutputPort());
		dxfActor->SetMapper(dtmMapper);
		dxfActor->GetProperty()->SetColor(0, 0, 0);
	}
}

void GDxfNode::setDataRange(double* dataRange)
{
	m_excuted = false;
	memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	setPictureFile(m_picture.picFilePath);
	QApplication::restoreOverrideCursor();

	dxfActor->GetPosition(first_pos);	
}

std::shared_ptr<SuperNodeStruct> GDxfNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_Picture> data = std::make_shared<G_Picture>();
	*data = m_picture;
	SurperNode::getParameter(data);
	return data;
}

//class MouseInteractorHighLightActor;
bool GDxfNode::setParameter(std::shared_ptr<SuperNodeStruct> param) {
	std::shared_ptr<G_Picture> pic = std::dynamic_pointer_cast<G_Picture>(param);
	SurperNode::setParameter(param);
	m_picture = *pic;

	if(param->m_changetype == G_Picture_MN_CT+ 0x80){
		setPicPos(first_pos);
		//setBoxPos();
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

void GDxfNode::getDefault(SuperNodeStruct* st)
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

bool GDxfNode::run() {
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return false;
	if (m_excuted)
		return true;
		{
			//if (m_picture.apply)
				setPicPos(first_pos);
			//else
				//setBoxPos();//setOffset();
		}
	
	if(m_checked){
		auto renderer = m_project.lock()->getRender();
		renderer->AddActor(dxfActor);
	}
	m_excuted = true;
	//makeNextNodeRun();
	return true;
}

bool GDxfNode::addToRender()
{
	if (m_excuted){
		auto renderer = m_project.lock()->getRender();
		renderer->AddActor(dxfActor);
	}
	return m_excuted;
}

bool GDxfNode::removeFromRender() {
	if(m_excuted){
		auto renderer = m_project.lock()->getRender();
		renderer->RemoveActor(dxfActor);
	}
	return m_excuted;
}

void GDxfNode::updateDisplay(std::shared_ptr<SuperNodeStruct> highlowmap)
{

}

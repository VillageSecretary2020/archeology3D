// GBoundingBoxNode.cpp

#include "border_property_widget.h"
#include "GBoundingBoxNode.h"
#include "XmlProject.h"

#include "vtkLabeledDataMapper.h"

std::string f2fstr(float f, int digits)
{
		return QString::number(f, 'f', digits).toStdString()	;
}

GBoundingBoxNode::GBoundingBoxNode(std::shared_ptr<XmlProject> project):GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_BOUNDINGBOX_TYPE;
	m_nodeCanAppendNodeTypeList = {NodeType::NODE_G_AXIS_TYPE};
	//m_dataRange[0] = m_dataRange[1] = m_dataRange[2] = m_dataRange[3] = m_dataRange[4]
	//	= m_dataRange[5] = NAN;
	m_actor = vtkSmartPointer<vtkActor>::New();
	m_actor_2d = vtkSmartPointer<vtkActor2D>::New();

}

GBoundingBoxNode::~GBoundingBoxNode()
{
}

bool GBoundingBoxNode::setParameter(std::shared_ptr<SuperNodeStruct> boundingBox)
{
	if (!m_project.lock())
		return false;
	std::shared_ptr<G_BoundingBox> _boundingBox = std::dynamic_pointer_cast<G_BoundingBox>(boundingBox);

	//if (m_excuted)
	{
		//updateDisplay(boundingBox);
	}
	//else
	{
		SurperNode::setParameter(boundingBox);
		_boundingBox->setProjectId(m_boundingbox.getProjectId());
		//memcpy_s(&m_boundingbox, sizeof(G_BoundingBox), &boundingBox, sizeof(G_BoundingBox));
		m_boundingbox = *_boundingBox;
		m_boundingbox.setProjectId(m_project.lock()->getProjectId());
		removeFromRender();

		if (m_excuted)	{// yzw 0712 如果已经调用run了，
			m_excuted = false;
			run();					// 则（是更新参数）再调
		}
	}
	
	return true;
}

bool GBoundingBoxNode::getParameter(G_BoundingBox & boundingbox)
{
	if ( !m_project.lock())	// RETURN_ERROR == m_nodeIndex ||
		return false;
	//memcpy_s(&boundingbox, sizeof(G_BoundingBox), &m_boundingbox, sizeof(G_BoundingBox));
	boundingbox = m_boundingbox;
	return true;
}

std::shared_ptr<SuperNodeStruct> GBoundingBoxNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_BoundingBox> ptr = std::make_shared<G_BoundingBox>();
	*ptr = m_boundingbox;
	SurperNode::getParameter(ptr);
	return ptr;
}

bool GBoundingBoxNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return SetProjectError;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError;
	TiXmlElement* pElement = (TiXmlElement*)(node);
	if (!pElement)
		return SetProjectError;
	TiXmlElement* pParameter = pElement->FirstChildElement("Parameters");
	if (!pParameter)
		return SetProjectError;
	m_boundingbox.fromXml(pParameter);

	return true;
}

bool GBoundingBoxNode::run()
{
	if (m_excuted)
		return true;
	if(!m_ranged){
		m_ranged = 1;
	m_dataRange[0] = m_dataRange[3] = m_data[0].x;
	m_dataRange[1] = m_dataRange[4] = m_data[0].y;
	m_dataRange[2] = m_dataRange[5] = m_data[0].z;
	for (auto var : m_data)
	{
		if (var.x > m_dataRange[3])
		{
			m_dataRange[3] = var.x;
		}
		if (var.x < m_dataRange[0])
		{
			m_dataRange[0] = var.x;
		}
		if (var.y > m_dataRange[4])
		{
			m_dataRange[4] = var.y;
		}
		if (var.y < m_dataRange[1])
		{
			m_dataRange[1] = var.y;
		}
		if (var.z > m_dataRange[5])
		{
			m_dataRange[5] = var.z;
		}
		if (var.z < m_dataRange[2])
		{
			m_dataRange[2] = var.z;
		}
	}}
	m_boundingbox.m_leftBottom.m_x = *(m_dataRange);
	m_boundingbox.m_leftBottom.m_y = *(m_dataRange + 1);
	m_boundingbox.m_leftBottom.m_z = *(m_dataRange + 2);
	m_boundingbox.m_rightTop.m_x = *(m_dataRange + 3);
	m_boundingbox.m_rightTop.m_y = *(m_dataRange + 4);
	m_boundingbox.m_rightTop.m_z = *(m_dataRange + 5);
	border_property_widget* ptr_border_widget = (border_property_widget*)this->m_boundingbox.widget;
	ptr_border_widget->update_peak_from_node(m_boundingbox.m_leftBottom, m_boundingbox.m_rightTop);
	vtkSmartPointer<vtkOutlineSource> m_outline = vtkSmartPointer<vtkOutlineSource>::New();
	m_outline->SetBounds(m_boundingbox.m_leftBottom.m_x, m_boundingbox.m_rightTop.m_x, m_boundingbox.m_leftBottom.m_y, m_boundingbox.m_rightTop.m_y, m_boundingbox.m_leftBottom.m_z, m_boundingbox.m_rightTop.m_z);
	m_outline->Update();
	vtkSmartPointer<vtkPolyDataMapper> m_outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_outlineMapper->SetInputConnection(m_outline->GetOutputPort());
	//m_actor = vtkSmartPointer<vtkActor>::New();
	m_actor->SetMapper(m_outlineMapper);
	m_actor->GetProperty()->SetLineWidth(m_boundingbox.m_render.m_lineWidth);
	m_actor->GetProperty()->SetColor(m_boundingbox.m_render.m_rgb.m_r / 255.0, m_boundingbox.m_render.m_rgb.m_g / 255.0, m_boundingbox.m_render.m_rgb.m_b / 255.0);

	auto digi= m_boundingbox.m_labelMark.m_decimalDigits;
	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	auto pos = vtkSmartPointer<vtkStringArray>::New();
	auto	idtype = points->InsertNextPoint(m_boundingbox.m_leftBottom.m_x, m_boundingbox.m_leftBottom.m_y, m_boundingbox.m_leftBottom.m_z);
	cells->InsertNextCell(1, &idtype);
	pos->InsertNextValue(
		f2fstr(m_boundingbox.m_leftBottom.m_x,  digi)+ ", "+
		f2fstr(m_boundingbox.m_leftBottom.m_y,  digi) + ", "+
		f2fstr(m_boundingbox.m_leftBottom.m_z,  digi) );

			idtype = points->InsertNextPoint(m_boundingbox.m_rightTop.m_x, m_boundingbox.m_rightTop.m_y, m_boundingbox.m_rightTop.m_z);
	cells->InsertNextCell(1, &idtype);
	pos->InsertNextValue(
		f2fstr(m_boundingbox.m_rightTop.m_x, digi)+ ", " +
		f2fstr(m_boundingbox.m_rightTop.m_y, digi) + ", " +
		f2fstr(m_boundingbox.m_rightTop.m_z, digi));
	pos->SetName("pos");

	auto outlinePoints = vtkSmartPointer<vtkPolyData>::New();
	outlinePoints->SetPoints(points);
	outlinePoints->SetVerts(cells);
	outlinePoints->GetPointData()->AddArray(pos);

	vtkSmartPointer<vtkLabeledDataMapper> m_pointLabelMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
	m_pointLabelMapper->SetInputData(outlinePoints);
	m_pointLabelMapper->SetLabelModeToLabelFieldData();
	m_pointLabelMapper->SetFieldDataName("pos");
	m_pointLabelMapper->GetLabelTextProperty()->SetFontSize(m_boundingbox.m_labelMark.m_fontSize);
	m_pointLabelMapper->GetLabelTextProperty()->SetFontFamily(VTK_FONT_FILE);

	//m_actor_2d = vtkSmartPointer<vtkActor2D>::New();
	m_actor_2d->SetMapper(m_pointLabelMapper);

	if(m_checked){
		m_project.lock()->getRender()->AddActor(m_actor);
		if (m_boundingbox.m_labelMark.m_customOn)
		{// 0610
			m_project.lock()->getRender()->AddActor2D(m_actor_2d);
		}
	}
	m_excuted = true;
	return true;
}

bool GBoundingBoxNode::addToRender()	 // yzw 0610
{
	if (m_excuted)
	{
		m_project.lock()->getRender()->AddActor(m_actor);
		m_project.lock()->getRender()->AddActor(m_actor_2d);
	 }
	return m_excuted;
}

bool GBoundingBoxNode::removeFromRender()	 // yzw 0610
{
	if (m_excuted)
	{
		m_project.lock()->getRender()->RemoveActor(m_actor);
		m_project.lock()->getRender()->RemoveActor(m_actor_2d);
		return true;
	 }
	 return m_excuted;
}

void GBoundingBoxNode::getBound(double bounds[6])
{
}

void GBoundingBoxNode::setBound(double * bounds)
{
}

void GBoundingBoxNode::updateDisplay(std::shared_ptr<SuperNodeStruct> boundingbox)
{
}

std::shared_ptr<TiXmlNode> GBoundingBoxNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlNode> pElemNode = std::dynamic_pointer_cast<TiXmlNode>(pNode);
	std::shared_ptr<TiXmlElement> pParameter(new TiXmlElement("Parameters"));
	m_boundingbox.toXml(pParameter);
	pElemNode->InsertEndChild(*pParameter);
	return pElemNode;
}

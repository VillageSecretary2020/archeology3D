#include "GOrthogonal_factor.h"
#include "XmlProject.h"
#include "orthogonal_property_widget.h"

GOrthonalFactorNode::GOrthonalFactorNode(std::shared_ptr<XmlProject> project) :GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_ORTHOGONALFACTOR_TYPE;
	m_imageData = vtkSmartPointer<vtkImageData>::New();
	m_voi = vtkSmartPointer<vtkExtractVOI>::New();
	m_lut = vtkSmartPointer<vtkLookupTable>::New();
	m_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	m_orthoimage = vtkSmartPointer<vtkActor>::New();
}

GOrthonalFactorNode::~GOrthonalFactorNode()
{
}

void GOrthonalFactorNode::setResolution(int * slt)
{
	m_excuted = false;
	memcpy_s(m_resolution, sizeof(int) * 3, slt, sizeof(int) * 3);
	if (widget) {
		orthogonal_property_widget*pw = (orthogonal_property_widget*)widget;
		pw->setResolution(m_resolution);
	}
}

bool GOrthonalFactorNode::setParameter(std::shared_ptr<SuperNodeStruct> orthogonalfactor)
{
	//if(!m_project.lock() || RETURN_ERROR == m_nodeIndex)
	if(!m_project.lock())
		return false;
	std::shared_ptr<G_Orthogonal_Factor> _orthogonal = std::dynamic_pointer_cast<G_Orthogonal_Factor>(orthogonalfactor);
	//if (m_excuted)
	{/*此处可添加测试代码*/
		//updateDisplay(orthogonalfactor);
	}
	//else
	{
		SurperNode::setParameter(orthogonalfactor);
		_orthogonal->setProjectId(m_orthogonalFactor.getProjectId());
		//memcpy_s(&m_orthogonalFactor, sizeof(G_Orthogonal_Factor), _orthogonal.get(), sizeof(G_Orthogonal_Factor));
		m_orthogonalFactor = *_orthogonal;
		m_orthogonalFactor.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	}
	return true;
}

bool GOrthonalFactorNode::getParameter(std::shared_ptr<SuperNodeStruct> data)
{
	if (!m_project.lock())	// RETURN_ERROR == m_nodeIndex ||
		return false;
	*data = m_orthogonalFactor;
	return true;
}

std::shared_ptr<SuperNodeStruct> GOrthonalFactorNode::getParameter()
{
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return std::shared_ptr<SuperNodeStruct>();

	std::shared_ptr<G_Orthogonal_Factor> data= std::make_shared<G_Orthogonal_Factor>();
	*data = m_orthogonalFactor;
	SurperNode::getParameter(data);
	return data;
}

std::shared_ptr<TiXmlNode> GOrthonalFactorNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pNodeElement = std::dynamic_pointer_cast<TiXmlElement>(pNode);

	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_orthogonalFactor.toXml(pParameters);
#if 0
	pParameters->SetAttribute("putIn", m_orthogonalFactor.m_putIn.data());
	pParameters->SetAttribute("putIndex", m_orthogonalFactor.m_putIndex);
	pParameters->SetAttribute("direction", (int)m_orthogonalFactor.m_direction);
	pParameters->SetAttribute("setId", m_orthogonalFactor.m_setId);
	pParameters->SetAttribute("colorMethod", (int)m_orthogonalFactor.m_colorMehod);
	pParameters->SetAttribute("gradientMap", m_orthogonalFactor.m_gradientMap);
	std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
	pColorBar->SetAttribute("displayOn", m_orthogonalFactor.m_colorBar.m_displayOn);
	pColorBar->SetAttribute("direction", (int)m_orthogonalFactor.m_colorBar.m_direction);
	pColorBar->SetAttribute("xpos", m_orthogonalFactor.m_colorBar.m_xPos);
	pColorBar->SetAttribute("ypos", m_orthogonalFactor.m_colorBar.m_yPos);
	pColorBar->SetAttribute("width", m_orthogonalFactor.m_colorBar.m_width);
	pColorBar->SetAttribute("height", m_orthogonalFactor.m_colorBar.m_height);
	std::shared_ptr<TiXmlElement> pText = std::make_shared<TiXmlElement>("Text");
	pText->SetAttribute("text", m_orthogonalFactor.m_colorBar.m_title.m_title.data());
	pText->SetAttribute("fontSize", m_orthogonalFactor.m_colorBar.m_title.m_fontSize);
	pColorBar->InsertEndChild(*pText);
	std::shared_ptr<TiXmlElement> pLabelMark = std::make_shared<TiXmlElement>("LabelMark");
	pLabelMark->SetAttribute("num", m_orthogonalFactor.m_colorBar.m_label.m_num);
	pLabelMark->SetAttribute("fondSize", m_orthogonalFactor.m_colorBar.m_label.m_fontSize);
	pLabelMark->SetAttribute("customOn", m_orthogonalFactor.m_colorBar.m_label.m_customOn);
	pLabelMark->SetAttribute("type", m_orthogonalFactor.m_colorBar.m_label.m_type);
	pLabelMark->SetAttribute("decimalDigits", m_orthogonalFactor.m_colorBar.m_label.m_decimalDigits);
	pLabelMark->SetAttribute("preText", m_orthogonalFactor.m_colorBar.m_label.m_preText.data());
	pLabelMark->SetAttribute("lastText", m_orthogonalFactor.m_colorBar.m_label.m_lastText.data());
	pColorBar->InsertEndChild(*pLabelMark);
	pParameters->InsertEndChild(*pColorBar);
#endif
	pNodeElement->InsertEndChild(*pParameters);
	return pNodeElement;
}

bool GOrthonalFactorNode::readParameterFromXmlNode(TiXmlNode * node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError
	m_orthogonalFactor.m_name = m_nodeName;
	m_orthogonalFactor.m_nodeIndex = m_nodeIndex;

	TiXmlElement* pParameters = ((TiXmlElement*)node)->FirstChildElement("Parameters");
	if (!pParameters)
		return SetProjectError
		m_orthogonalFactor.fromXml(pParameters);
#if 0
	m_orthogonalFactor.m_putIn = pParameters->Attribute("putIn");
	if (TIXML_SUCCESS != pParameters->QueryUnsignedAttribute("putIndex", (unsigned int*)&m_orthogonalFactor.m_putIndex))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryIntAttribute("direction", (int*)&m_orthogonalFactor.m_direction))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryIntAttribute("setId", &m_orthogonalFactor.m_setId))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryIntAttribute("colorMethod", (int*)&m_orthogonalFactor.m_colorMehod))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_orthogonalFactor.m_gradientMap))
		return SetProjectError
	TiXmlElement* pColorBar = pParameters->FirstChildElement("ColorBar");
	if (!pColorBar)
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryBoolAttribute("displayOn", &m_orthogonalFactor.m_colorBar.m_displayOn))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryIntAttribute("direction", (int*)&m_orthogonalFactor.m_colorBar.m_direction))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryFloatAttribute("xpos", &m_orthogonalFactor.m_colorBar.m_xPos))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryFloatAttribute("ypos", &m_orthogonalFactor.m_colorBar.m_yPos))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryFloatAttribute("width", &m_orthogonalFactor.m_colorBar.m_width))
		return SetProjectError
	if (TIXML_SUCCESS != pParameters->QueryFloatAttribute("height", &m_orthogonalFactor.m_colorBar.m_height))
		return SetProjectError
	TiXmlElement* pText = pColorBar->FirstChildElement("Text");
	if (!pText)
		return SetProjectError
	m_orthogonalFactor.m_colorBar.m_title.m_title = pText->Attribute("text");
	if (TIXML_SUCCESS != pText->QueryUnsignedAttribute("fontSize", (unsigned int*)&m_orthogonalFactor.m_colorBar.m_title.m_fontSize))
		return SetProjectError
	TiXmlElement* pLabelMark = pText->NextSiblingElement("LabelMark");
	if (!pLabelMark)
		return SetProjectError
	if (TIXML_SUCCESS != pLabelMark->QueryIntAttribute("num", &m_orthogonalFactor.m_colorBar.m_label.m_num))
		return SetProjectError
	if (TIXML_SUCCESS != pLabelMark->QueryUnsignedAttribute("fondSize", (unsigned int*)&m_orthogonalFactor.m_colorBar.m_label.m_fontSize))
		return SetProjectError
	if (TIXML_SUCCESS != pLabelMark->QueryBoolAttribute("customOn", &m_orthogonalFactor.m_colorBar.m_label.m_customOn))
		return SetProjectError
	if (TIXML_SUCCESS != pLabelMark->QueryUnsignedAttribute("type", (unsigned int*)&m_orthogonalFactor.m_colorBar.m_label.m_type))
		return SetProjectError
	if (TIXML_SUCCESS != pLabelMark->QueryUnsignedAttribute("decimalDigits", (unsigned int*)&m_orthogonalFactor.m_colorBar.m_label.m_decimalDigits))
		return SetProjectError
	m_orthogonalFactor.m_colorBar.m_label.m_preText = pLabelMark->Attribute("preText");
	m_orthogonalFactor.m_colorBar.m_label.m_lastText = pLabelMark->Attribute("lastText");
#endif
	return true;
}

void GOrthonalFactorNode::getDefault(SuperNodeStruct* st)
{
	G_Orthogonal_Factor* pst = (G_Orthogonal_Factor*)st;
	pst->m_direction = CutPlaneDirection::D_XZ;
	pst->m_setId = 1;
	pst->m_colorBar.getDefault();
	pst->m_colorBar.m_title.m_title = "Orthonal";
}

bool GOrthonalFactorNode::run()
{
	if (m_excuted)
		return true;
	if(!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return false;
	//m_orthogonalFactor.m_direction = CutPlaneDirection::D_XZ;
	//m_orthogonalFactor.m_setId = 15;

	double spacing[3] = { 0, 0, 0 };

	spacing[0] = (m_limits[3] - m_limits[0]) / (m_resolution[0]);
	spacing[1] = (m_limits[4] - m_limits[1]) / (m_resolution[1]);
	spacing[2] = (m_limits[5] - m_limits[2]) / (m_resolution[2]);

	m_imageData->SetDimensions(m_resolution[0] , m_resolution[1] , m_resolution[2] );
	m_imageData->SetSpacing(spacing[0], spacing[1], spacing[2]);
	m_imageData->SetOrigin(m_limits[0], m_limits[1], m_limits[2]);
	m_imageData->AllocateScalars(VTK_DOUBLE, 1);

	for (int i = 0; i < m_resolution[0]; i++)
	{
		//double i_f = spacing[0] * i + m_limits[0];
		for (int j = 0; j < m_resolution[1]; j++)
		{
			//double j_f = spacing[1] * j + m_limits[1];
			for (int k = 0; k < m_resolution[2]; k++)
			{
				//double k_f = spacing[2] * k + m_limits[2];
				//*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * 50 * 50 + j * 50 + k].scalar;
				*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * m_resolution[1] * m_resolution[2] + j * m_resolution[2] + k].scalar;
			}
		}
	}

	m_voi->SetInputData(m_imageData);
	switch (m_orthogonalFactor.m_direction)
	{
	case CutPlaneDirection::D_XY:
	{
		m_voi->SetVOI(0,
			m_imageData->GetDimensions()[0]-1,
			0,
			m_imageData->GetDimensions()[1]-1,
			m_orthogonalFactor.m_setId - 1,
			m_orthogonalFactor.m_setId - 1);
	}
	break;
	case CutPlaneDirection::D_XZ:
	{
		m_voi->SetVOI(0,
			m_imageData->GetDimensions()[0] - 1,
			m_orthogonalFactor.m_setId - 1,
			m_orthogonalFactor.m_setId - 1,
			0,
			m_imageData->GetDimensions()[2] - 1);
	}
	break;
	case CutPlaneDirection::D_YZ:
	{
		m_voi->SetVOI(m_orthogonalFactor.m_setId - 1,
			m_orthogonalFactor.m_setId - 1,
			0,
			m_imageData->GetDimensions()[1] - 1,
			0,
			m_imageData->GetDimensions()[2] - 1);
	}
	break;
	default:
		break;
	}
	m_voi->Update();

	//m_lut->SetHueRange(0.0, 0.6);
	//m_lut->SetAlphaRange(1.0, 1.0);
	//m_lut->SetValueRange(1.0, 1.0);
	//m_lut->SetSaturationRange(1.0, 1.0);
	//m_lut->SetNumberOfTableValues(50);
	//m_lut->SetRange(m_imageData->GetScalarRange());
	//m_lut->Build();
	int cr = setLut(m_orthogonalFactor.m_gradientMap, m_imageData->GetScalarRange(), m_orthogonalFactor.m_colorBar.m_label);

	m_mapper->SetInputConnection(m_voi->GetOutputPort());
	m_mapper->SetLookupTable(m_lut);
	m_mapper->SetScalarModeToUsePointData();
	//m_mapper->UseLookupTableScalarRangeOn();
	m_mapper->SetScalarRange(m_imageData->GetScalarRange());//

	m_scalarBar->SetLookupTable(m_mapper->GetLookupTable());
	setColorBar(m_orthogonalFactor.m_colorBar);

	m_orthoimage->SetMapper(m_mapper); 
	if(m_checked){
		auto renderer = m_project.lock()->getRender();
		renderer->AddActor(m_orthoimage);
		if (m_orthogonalFactor.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);

	//makeNextNodeRun();
	}
	m_excuted = true;
	return true;
}

void GOrthonalFactorNode::makeNextNodeRun()
{
	for (auto nId : m_nodeNextNodeList)
	{
		std::shared_ptr<SurperNode> node = m_project.lock()->getSpecifiedNode(nId);
		switch (node->getNodeType())
		{
		case NodeType::NODE_G_AXIS_TYPE:
		{

		}
		break;
		case NodeType::NODE_G_BOUNDINGBOX_TYPE:
		{

		}
		break;
		default:
			break;
		}
	}
}

bool GOrthonalFactorNode::addToRender()	 // yzw 0610
{
	if (m_excuted){
		m_project.lock()->getRender()->AddActor(m_orthoimage);
		if (m_orthogonalFactor.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
	}return m_excuted;
}

bool GOrthonalFactorNode::removeFromRender() // yzw 0610
{
	if (m_excuted){
		m_project.lock()->getRender()->RemoveActor(m_orthoimage);
		m_project.lock()->getRender()->RemoveActor(m_scalarBar);
	}return m_excuted;
}

void GOrthonalFactorNode::updateDisplay(std::shared_ptr<SuperNodeStruct> orthogonalFactor)
{
	std::shared_ptr<G_Orthogonal_Factor> _orthogonal = std::dynamic_pointer_cast<G_Orthogonal_Factor>(orthogonalFactor);
	if (!_orthogonal)
		return;
	switch (_orthogonal->m_changetype)
	{
	case 0:
	{
		
	}
	break;
	case 1:
	{
		double spacing[3] = { 0, 0, 0 };
		spacing[0] = (m_limits[3] - m_limits[0]) / (m_resolution[0]);
		spacing[1] = (m_limits[4] - m_limits[1]) / (m_resolution[1]);
		spacing[2] = (m_limits[5] - m_limits[2]) / (m_resolution[2]);

		m_orthogonalFactor.m_nodeIndex = _orthogonal->m_putIndex;
		m_imageData->SetDimensions(m_resolution[0] + 1, m_resolution[1] + 1, m_resolution[2] + 1);
		m_imageData->SetSpacing(spacing[0], spacing[1], spacing[2]);
		m_imageData->SetOrigin(m_limits[0], m_limits[1], m_limits[2]);
		m_imageData->AllocateScalars(VTK_DOUBLE, 1);

		for (int i = 0; i <= m_resolution[0]; i++)
		{
			double i_f = spacing[0] * i + m_limits[0];
			for (int j = 0; j <= m_resolution[1]; j++)
			{
				double j_f = spacing[1] * j + m_limits[1];
				for (int k = 0; k <= m_resolution[2]; k++)
				{
					double k_f = spacing[2] * k + m_limits[2];
					*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * 50 * 50 + j * 50 + k].scalar;
				}
			}
		}
		m_voi->SetInputData(m_imageData);
		switch (m_orthogonalFactor.m_direction)
		{
		case CutPlaneDirection::D_XY:
		{
			m_voi->SetVOI(0,
				m_imageData->GetDimensions()[0]-1,
				0,
				m_imageData->GetDimensions()[1]-1,
				m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1);
		}
		break;
		case CutPlaneDirection::D_XZ:
		{
			m_voi->SetVOI(0, m_imageData->GetDimensions()[0]-1, m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1, 0, m_imageData->GetDimensions()[2]-1);
		}
		break;                             
		case CutPlaneDirection::D_YZ:
		{
			m_voi->SetVOI(m_orthogonalFactor.m_setId - 1, m_orthogonalFactor.m_setId - 1,
				0, m_imageData->GetDimensions()[1] - 1, 0, m_imageData->GetDimensions()[2] - 1);
		}
		break;
		default:
			break;
		}
		m_voi->Update();
		m_lut->SetRange(m_imageData->GetScalarRange());
		m_lut->Build();
		m_mapper->Update();

	}
	break;
	case 2:
	{
		m_orthogonalFactor.m_direction = _orthogonal->m_direction;
		switch (m_orthogonalFactor.m_direction)
		{
		case CutPlaneDirection::D_XY:
		{
			m_voi->SetVOI(0,
				m_imageData->GetDimensions()[0] - 1,
				0,
				m_imageData->GetDimensions()[1] - 1,
				m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1);
		}
		break;
		case CutPlaneDirection::D_XZ:
		{
			m_voi->SetVOI(0,
				m_imageData->GetDimensions()[0] - 1,
				m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1,
				0,
				m_imageData->GetDimensions()[2] - 1);
		}
		break;
		case CutPlaneDirection::D_YZ:
		{
			m_voi->SetVOI(m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1,
				0,
				m_imageData->GetDimensions()[1] - 1,
				0,
				m_imageData->GetDimensions()[2] - 1);
		}
		break;
		default:
			break;
		}
		m_voi->Update();
	}
	break;
	case 3:
	{
		m_orthogonalFactor.m_setId = _orthogonal->m_setId;
		switch (m_orthogonalFactor.m_direction)
		{
		case CutPlaneDirection::D_XY:
		{
			m_voi->SetVOI(0,
				m_imageData->GetDimensions()[0] - 1,
				0,
				m_imageData->GetDimensions()[1] - 1,
				m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1);
			
		}
		break;
		case CutPlaneDirection::D_XZ:
		{
			m_voi->SetVOI(0,
				m_imageData->GetDimensions()[0] - 1,
				m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1,
				0,
				m_imageData->GetDimensions()[2] - 1);
			
		}
		break;
		case CutPlaneDirection::D_YZ:
		{
			m_voi->SetVOI(m_orthogonalFactor.m_setId - 1,
				m_orthogonalFactor.m_setId - 1,
				0,
				m_imageData->GetDimensions()[1] - 1,
				0,
				m_imageData->GetDimensions()[2] - 1);
			
		}
		break;
		default:
			break;
		}
		m_voi->Update();
	}
	break;
	case 4:
	{

	}
	break;
	case 5:
	{
		updateDisplayColorBar(m_orthogonalFactor.m_colorBar, _orthogonal->m_colorBar);
	}
	break;
	default:
		break;
	}
	m_project.lock()->getRender()->Render();
}

#include "GTextNode.h"
#include "XmlProject.h"

GTextNode::GTextNode(std::shared_ptr<XmlProject> project) :GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_TEXT_TYPE;
	m_nodeCanAppendNodeTypeList = { 
		NodeType::NODE_G_AXIS_TYPE,
		NodeType::NODE_G_BOUNDINGBOX_TYPE,
		//NodeType::NODE_G_CLIPBOARD_TYPE 
	};
	m_textLabelMapper = vtkSmartPointer<vtkDynamic2DLabelMapper>::New();
	m_actor_2d = vtkSmartPointer<vtkActor2D>::New();
	colors=vtkSmartPointer<vtkNamedColors> ::New();
	atext = vtkSmartPointer<vtkVectorText> ::New();
	textMapper = vtkSmartPointer<vtkPolyDataMapper> ::New();
	textActor = vtkSmartPointer<vtkBillboardTextActor3D>::New();//vtkFollower
}

GTextNode::~GTextNode()
{
}

bool GTextNode::setParameter(std::shared_ptr<SuperNodeStruct> Gtext)
{
	//std::cout << "输出text: " << text->m_text << std::endl;
	//if(!m_project.lock() || RETURN_ERROR == m_nodeIndex)
	if(!m_project.lock())
		return false;
	//if (m_excuted)
	{
	//	updateDisplay(Gtext);
	}
	std::shared_ptr<G_Text> text = std::dynamic_pointer_cast<G_Text>(Gtext);
	if (!text)
		return false;

	//memcpy_s(&m_Gtext, sizeof(G_Text), Gtext.get(), sizeof(G_Text));
	m_Gtext = *text;
	m_Gtext.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	//if (m_excuted) 
	{
		m_excuted = false;
		return run();
	}
	return true;
}

bool GTextNode::getParameter(G_Text & Gtext)
{
	if(!m_project.lock())// || RETURN_ERROR == m_nodeIndex)
		return false;
	//memcpy_s(&Gtext, sizeof(G_Text), &m_Gtext, sizeof(G_Text));
	Gtext = m_Gtext;
	return true;
}

std::shared_ptr<SuperNodeStruct> GTextNode::getParameter()
{
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return std::shared_ptr<SuperNodeStruct>();

	std::shared_ptr<G_Text> data = std::make_shared<G_Text>();
	*data = m_Gtext;
	SurperNode::getParameter(data);
	return data;
}

std::shared_ptr<TiXmlNode> GTextNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameter(new TiXmlElement("Parameters"));
		m_Gtext.toXml(pParameter);

	pElemNode->InsertEndChild(*pParameter);

	return pElemNode;
}

bool GTextNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	m_Gtext.m_name = m_nodeName;
	m_Gtext.m_nodeIndex = m_nodeIndex;

	TiXmlElement* pParameters = ((TiXmlElement*)node)->FirstChildElement("Parameters");
	if (!pParameters)
		return SetProjectError

	m_Gtext.fromXml(pParameters);

	return true;
}

bool GTextNode::run()
{
	if (m_excuted)
		return true;
	if (!m_project.lock())
	{
		return false;
	}

#if 0
	vtkUnicodeString str = vtkUnicodeString::from_utf8(string_To_UTF8(m_Gtext.m_text));
	auto point = vtkSmartPointer<vtkPoints>::New();
	auto cell = vtkSmartPointer<vtkCellArray>::New();
	auto text_str = vtkSmartPointer<vtkStringArray>::New();
	auto idtype = point->InsertNextPoint(m_Gtext.m_pos.m_x, m_Gtext.m_pos.m_y, m_Gtext.m_pos.m_z);
	cell->InsertNextCell(1, &idtype);
	text_str->InsertNextValue(str.utf8_str());
	text_str->SetName("text");


	auto textPoints = vtkSmartPointer<vtkPolyData>::New();
	textPoints->SetPoints(point);
	textPoints->SetVerts(cell);
	textPoints->GetPointData()->AddArray(text_str);
	m_textLabelMapper->SetInputData(textPoints);
	m_textLabelMapper->SetLabelModeToLabelFieldData();
	m_textLabelMapper->SetFieldDataName("text");
	m_textLabelMapper->GetLabelTextProperty()->SetFontSize(m_Gtext.m_fontSize);
	m_textLabelMapper->GetLabelTextProperty()->SetFontFamily(VTK_FONT_FILE);
	//m_textLabelMapper->GetLabelTextProperty()->SetFontFile("C:/Windows/Fonts/simfang.ttf");
	m_textLabelMapper->GetLabelTextProperty()->SetColor(m_Gtext.m_rgb.m_r / 255.0, m_Gtext.m_rgb.m_g / 255.0, m_Gtext.m_rgb.m_b / 255.0);
	m_textLabelMapper->SetLabelFormat("%s"); //("_first%s_last");

	m_actor_2d = vtkSmartPointer<vtkActor2D>::New();
	m_actor_2d->SetMapper(m_textLabelMapper);
#endif
	vtkTextProperty*pTexProper = vtkTextProperty::New(); //声明文本属性
	pTexProper->SetColor(m_Gtext.m_rgb.m_r / 255.0, m_Gtext.m_rgb.m_g / 255.0, m_Gtext.m_rgb.m_b / 255.0);
	pTexProper->SetFontSize(m_Gtext.m_fontSize);
	pTexProper->SetFontFamily(VTK_FONT_FILE);
	pTexProper->SetFontFile("C:/Windows/Fonts/simfang.ttf"); // simsun.ttc
	//pTexProper->SetFontFamily(0);
	//pTexProper->SetJustification(1);
	//pTexProper->SetBold(1);
	//pTexProper->SetItalic(1);
	//pTexProper->SetShadow(1);

	atext->SetText(m_Gtext.m_text.data());//"Origin"
	textMapper->SetInputConnection(atext->GetOutputPort());
	textActor->SetTextProperty(pTexProper);
//	textActor->SetMapper(textMapper);
//	textActor->SetScale(0.2, 0.2, 0.2);	//AddPosition
//	textActor->SetPosition(m_Gtext.m_pos.m_x, m_Gtext.m_pos.m_y, m_Gtext.m_pos.m_z);
//	textActor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());
	textActor->SetInput(m_Gtext.m_text.data());
	textActor->SetTextProperty(pTexProper);
	textActor->SetPosition(m_Gtext.m_pos.m_x, m_Gtext.m_pos.m_y, m_Gtext.m_pos.m_z); //设置位置
	textActor->SetScale(0.7, 0.7, 0.7); //设置文字大小
	textActor->RotateX(90.0);//沿X轴旋转90度

	if(m_checked){
		auto ren = m_project.lock()->getRender();
		//ren->AddActor(m_actor_2d);
		ren->AddActor(textActor);
		//textActor->SetCamera(ren->GetActiveCamera());
	//m_project.lock()->getRender()->Render();
	}
	m_excuted = true;
	return false;
}

bool GTextNode::addToRender()
{
	if (m_excuted) {
		auto ren = m_project.lock()->getRender();
		//ren->AddActor(m_actor_2d);
		ren->AddActor(textActor);
		//textActor->SetCamera(ren->GetActiveCamera());
	}
	return m_excuted;
}

bool GTextNode::removeFromRender()
{
	if (m_excuted) {
		auto ren = m_project.lock()->getRender();
		//ren->RemoveActor(m_actor_2d);
		ren->RemoveActor(textActor);
	}
	return m_excuted;
}

void GTextNode::getBound(double bounds[6])
{
}

void GTextNode::setBound(double * bounds)
{
}


std::string GTextNode::string_To_UTF8(const std::string & str)
{
#if 0
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;
#endif
	std::string retStr(str);
	return retStr;
}

void GTextNode::updateDisplay(std::shared_ptr<SuperNodeStruct> Gtext)
{
	std::shared_ptr<G_Text> text = std::dynamic_pointer_cast<G_Text>(Gtext);
	if (!text)
		return;
	
		vtkUnicodeString str = vtkUnicodeString::from_utf8(string_To_UTF8(m_Gtext.m_text));

		auto point = vtkSmartPointer<vtkPoints>::New();
		auto cell = vtkSmartPointer<vtkCellArray>::New();
		auto text_str = vtkSmartPointer<vtkStringArray>::New();
		auto idtype = point->InsertNextPoint(m_Gtext.m_pos.m_x, m_Gtext.m_pos.m_y, m_Gtext.m_pos.m_z);
		cell->InsertNextCell(1, &idtype);
		text_str->InsertNextValue(str.utf8_str());
		text_str->SetName("text");
		auto textPoints = vtkSmartPointer<vtkPolyData>::New();
		textPoints->SetPoints(point);
		textPoints->SetVerts(cell);
		textPoints->GetPointData()->AddArray(text_str);

		m_textLabelMapper->SetInputData(textPoints);
		m_textLabelMapper->SetLabelModeToLabelFieldData();
		m_textLabelMapper->SetFieldDataName("text");
		m_textLabelMapper->GetLabelTextProperty()->SetFontSize(text->m_fontSize);
		m_textLabelMapper->GetLabelTextProperty()->SetFontFamily(VTK_FONT_FILE);
		//m_textLabelMapper->GetLabelTextProperty()->SetFontFile("C:/Windows/Fonts/simfang.ttf");
		m_textLabelMapper->GetLabelTextProperty()->SetColor(text->m_rgb.m_r / 255.0, text->m_rgb.m_g / 255.0, text->m_rgb.m_b / 255.0);
		m_textLabelMapper->SetLabelFormat("_first%s_last");
		m_textLabelMapper->Update();
		m_project.lock()->getRender()->Render();
}

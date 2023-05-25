#include "GCommentNode.h"
#include "XmlProject.h"

GCommentNode::GCommentNode(std::shared_ptr<XmlProject> project) : GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_COMMENT_TYPE;
	m_nodeCanAppendNodeTypeList = {
		NodeType::NODE_G_AXIS_TYPE,
		NodeType::NODE_G_BOUNDINGBOX_TYPE,
		//NodeType::NODE_G_CLIPBOARD_TYPE
	};
	m_textActor = vtkSmartPointer<vtkTextActor>::New();
}

GCommentNode::~GCommentNode()
{
}

bool GCommentNode::setParameter(std::shared_ptr<SuperNodeStruct> st)
{
	if(!m_project.lock())
		return false;
	SurperNode::setParameter(st);
	std::shared_ptr<G_Comment> _st = std::dynamic_pointer_cast<G_Comment>(st);
	m_comment= *_st;
	m_comment.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	//if (m_excuted)
	{
		m_excuted = false;
		return run();
	}
	return true;
}

bool GCommentNode::getParameter(G_Comment & comment)
{
	if (!m_project.lock()) // RETURN_ERROR == m_nodeIndex || 
		return false;
	//memcpy_s(&comment, sizeof(G_Comment), &m_comment, sizeof(G_Comment));
	comment = m_comment;
	return true;
}

std::shared_ptr<SuperNodeStruct> GCommentNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_Comment> data = std::make_shared<G_Comment>();
	//memcpy_s(mesh.get(), sizeof(C_Mesh), &m_mesh, sizeof(C_Mesh));
	*data = m_comment;
	SurperNode::getParameter(data);
	return data;
}

bool GCommentNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return SetProjectError;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError;
	m_comment.m_name = m_nodeName;
	m_comment.m_nodeIndex = m_nodeIndex;
	TiXmlElement* pElementNode = (TiXmlElement*)(node);
	if (!pElementNode)
		return SetProjectError;
	TiXmlElement* pParameter = (pElementNode->FirstChildElement("Parameters"));
	if (!pParameter)
		return SetProjectError;
	m_comment.fromXml(pParameter);

	return true;
}

void GCommentNode::getDefault(SuperNodeStruct* st) {
	G_Comment* pst = (G_Comment*)st;
	pst->m_fontSize = 20;
	pst->m_text = "comment";
	pst->m_align = LEFT;
	pst->m_XPos = 100;
	pst->m_YPos = 600;
}

bool GCommentNode::run()
{
	//if (m_excuted)
	//	return true;
	vtkUnicodeString str = vtkUnicodeString::from_utf8(
		m_comment.m_text
	);
	m_textActor->SetInput(str.utf8_str());
	m_textActor->SetDisplayPosition(m_comment.m_XPos, m_comment.m_YPos);
	if (Child_Text_Align::CENTER == m_comment.m_align)
	{
		m_textActor->GetTextProperty()->SetJustificationToCentered();
	}
	else if (Child_Text_Align::LEFT == m_comment.m_align)
	{
		m_textActor->GetTextProperty()->SetJustificationToLeft();
	}
	else
	{
		m_textActor->GetTextProperty()->SetJustificationToRight();
	}
	
	m_textActor->GetTextProperty()->SetFontSize(m_comment.m_fontSize);
	m_textActor->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
	m_textActor->GetTextProperty()->SetFontFile("C:/Windows/Fonts/simfang.ttf");
	m_textActor->GetTextProperty()->SetColor(m_comment.m_rgb.m_r / 255.0, m_comment.m_rgb.m_g / 255.0, m_comment.m_rgb.m_b / 255.0);

	if(m_checked){
		m_project.lock()->getRender()->AddActor(m_textActor);
		//makeNextNodeRun();
		//m_project.lock()->getRender()->ResetCamera();
		m_project.lock()->getRender()->Render();
	}
	m_excuted = true;
	return true;
}

bool GCommentNode::addToRender()	// yzw 0608
{
	if (m_excuted)
		m_project.lock()->getRender()->AddActor(m_textActor);
	return m_excuted;
}

bool GCommentNode::removeFromRender()	// yzw 0606
{
	if (m_excuted)
		m_project.lock()->getRender()->RemoveActor(m_textActor);
	return m_excuted;
}

void GCommentNode::getBound(double bounds[6])
{
}

void GCommentNode::setBound(double * bounds)
{
}

std::string GCommentNode::string_To_UTF8(const std::string & str)
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

void GCommentNode::updateDisplay(std::shared_ptr<SuperNodeStruct> comment)
{
	std::shared_ptr<G_Comment> node = std::dynamic_pointer_cast<G_Comment>(comment);
	switch (node->m_changetype)
	{
	case 1:
	{
		vtkUnicodeString str = vtkUnicodeString::from_utf8(string_To_UTF8(node->m_text));
		m_textActor->SetInput(str.utf8_str());
		m_comment.m_text = node->m_text;
		m_project.lock()->getRender()->Render();
	}
	break;
	case 2://修改字体
	{

	}
	break;
	case 3:
	{
		m_textActor->GetTextProperty()->SetFontSize(node->m_fontSize);
		m_comment.m_fontSize = node->m_fontSize;
		m_project.lock()->getRender()->Render();
	}
	break;
	case 4:
	{
		if (Child_Text_Align::CENTER == node->m_align)
		{
			m_textActor->GetTextProperty()->SetJustificationToCentered();
		}
		else if (Child_Text_Align::LEFT == node->m_align)
		{
			m_textActor->GetTextProperty()->SetJustificationToLeft();
		}
		else
		{
			m_textActor->GetTextProperty()->SetJustificationToRight();
		}
		m_comment.m_align = node->m_align;
		m_project.lock()->getRender()->Render();
	}
	break;
	case 5:
	{
		m_textActor->GetTextProperty()->SetColor(node->m_rgb.m_r / 255.0, node->m_rgb.m_g / 255.0, node->m_rgb.m_b / 255.0);
		memcpy_s(&m_comment.m_rgb, sizeof(Child_colorRGB), &node->m_rgb, sizeof(Child_colorRGB));
		m_project.lock()->getRender()->Render();
	}
	break;
	case 6:
	{
		m_textActor->SetDisplayPosition(node->m_XPos*1000, node->m_YPos*1000);
		m_comment.m_XPos = node->m_XPos;
		m_comment.m_YPos = node->m_YPos;
		m_project.lock()->getRender()->Render();
	}
	break;
	default:
		break;
	}
}

std::shared_ptr<TiXmlNode> GCommentNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlNode> pElemNode = std::dynamic_pointer_cast<TiXmlNode>(pNode);
	std::shared_ptr<TiXmlElement> pParameter(new TiXmlElement("Parameters"));
	m_comment.toXml(pParameter);
#if 0
	pParameter->SetAttribute("text", m_comment.m_text.data());
	pParameter->SetAttribute("fontType", m_comment.m_fontType);
	pParameter->SetAttribute("fontSize", m_comment.m_fontSize);
	pParameter->SetAttribute("xpos", m_comment.m_XPos);
	pParameter->SetAttribute("ypos", m_comment.m_YPos);
	pParameter->SetAttribute("align", (int)m_comment.m_align);
	std::shared_ptr<TiXmlElement> pRgb = std::make_shared<TiXmlElement>("ColorRgb");
	pRgb->SetAttribute("color_r", m_comment.m_rgb.m_r);
	pRgb->SetAttribute("color_g", m_comment.m_rgb.m_g);
	pRgb->SetAttribute("color_b", m_comment.m_rgb.m_b);
	pParameter->InsertEndChild(*pRgb);
#endif
	pElemNode->InsertEndChild(*pParameter);
	return pNode;
}

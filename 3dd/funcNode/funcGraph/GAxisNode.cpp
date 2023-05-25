// GAxisNode.cpp

#include "GAxisNode.h"
#include "XmlProject.h"
#include <vtkVertex.h>

GAxis::GAxis(std::shared_ptr< XmlProject> project) : GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_AXIS_TYPE;
	m_nodeCanAppendNodeTypeList = { NodeType::NODE_G_BOUNDINGBOX_TYPE };
	m_textActor = vtkSmartPointer<vtkActor2D>::New();
	m_cubeAxesActor 
		= vtkSmartPointer<vtkCubeAxesActor>::New();
	xArrowActor = vtkSmartPointer<vtkActor>::New();
	yArrowActor = vtkSmartPointer<vtkActor>::New();
	zArrowActor = vtkSmartPointer<vtkActor>::New();
}

GAxis::~GAxis()
{
}

void GAxis::getDefault(SuperNodeStruct* st)
{
	G_Axis* pst = (G_Axis*)st;	//	默认参数
	G_Axis m_axis;
	m_axis.m_display_gridLine = false;
	m_axis.m_LineRgb = { 255, 0, 0 };
	m_axis.m_lineWidth = 1;
	m_axis.m_font = 1;
	m_axis.m_labelMarkSize = 20;
	m_axis.m_showArrows = true; //未做
	m_axis.m_axisDiameter = 5;
	m_axis.m_textSize = 16;
	m_axis.m_xAxis = { true,  u8"X 轴", 0, 0, {  255, 0,  0},{ true, -50, 50, 2, "", 4 } };
	m_axis.m_yAxis = { true,  u8"Y 轴", 0, 0, { 0, 255, 0 }, { true, -50, 50, 2, "", 4 } };
	m_axis.m_zAxis = { true,  u8"Z 轴", 0, 0,  { 0, 0, 255 }, { true, -50, 50, 2, "", 4 } };
	*pst = m_axis;
}

bool GAxis::setParameter(std::shared_ptr<SuperNodeStruct> st)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if (!m_project.lock())
		return false;
	//memcpy_s(&m_axis, sizeof(G_Axis), &axis, sizeof(G_Axis));
	std::shared_ptr<G_Axis> _st = std::dynamic_pointer_cast<G_Axis>(st);
	SurperNode::setParameter(st);
	m_axis = *_st;
	m_axis.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return true;
}

bool GAxis::getParameter(G_Axis & axis)
{
	if ( !m_project.lock())	// RETURN_ERROR == m_nodeIndex ||
		return false;
	//memcpy_s(&axis, sizeof(G_Axis), &m_axis, sizeof(G_Axis));
	axis = m_axis;
	return true;
}

std::shared_ptr<SuperNodeStruct> GAxis::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_Axis> ptr = std::make_shared<G_Axis>();
	*ptr = m_axis;
	SurperNode::getParameter(ptr);
	return ptr;
}

std::shared_ptr<TiXmlNode> GAxis::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameter(new TiXmlElement("Parameters"));
	m_axis.toXml(pParameter);
#if 0
	pParameter->SetAttribute("putIn", m_axis.m_putIn.data());
	pParameter->SetAttribute("showGridLine", m_axis.m_display_gridLine);
	pParameter->SetAttribute("lineWidth", m_axis.m_lineWidth);
	pParameter->SetAttribute("font", m_axis.m_font);
	pParameter->SetAttribute("showArrows", (int)m_axis.m_showArrows);
	pParameter->SetAttribute("axisDiameter", m_axis.m_axisDiameter);
	pParameter->SetAttribute("labelMarkSize", m_axis.m_labelMarkSize);
	pParameter->SetAttribute("textSize", m_axis.m_textSize);
	std::shared_ptr<TiXmlElement> pRgb(new TiXmlElement("rgb"));
	pRgb->SetAttribute("r", m_axis.m_LineRgb.m_r);
	pRgb->SetAttribute("g", m_axis.m_LineRgb.m_g);
	pRgb->SetAttribute("b", m_axis.m_LineRgb.m_b);
	pParameter->InsertEndChild(*pRgb);
	std::shared_ptr<TiXmlElement> pXAxis(new TiXmlElement("xAxis"));
	pXAxis->SetAttribute("showAxis", (int)m_axis.m_xAxis.m_showAxis);
	pXAxis->SetAttribute("TextTitle", m_axis.m_xAxis.m_textTitle.data());
	pXAxis->SetAttribute("minValue", m_axis.m_xAxis.m_minValue);
	pXAxis->SetAttribute("maxValue", m_axis.m_xAxis.m_maxValue);
	std::shared_ptr<TiXmlElement> pColorRgb(new TiXmlElement("colorRGB"));
	pColorRgb->SetAttribute("r", m_axis.m_xAxis.m_rgb.m_r);
	pColorRgb->SetAttribute("g", m_axis.m_xAxis.m_rgb.m_g);
	pColorRgb->SetAttribute("b", m_axis.m_xAxis.m_rgb.m_b);

	pXAxis->InsertEndChild(*pColorRgb);
	std::shared_ptr<TiXmlElement> pLabelAxis = std::make_shared<TiXmlElement>("labelMarkAxis");
	pLabelAxis->SetAttribute("showLabelMark", m_axis.m_xAxis.m_labelmark.m_showLabelMark);
	pLabelAxis->SetAttribute("minValue", m_axis.m_xAxis.m_labelmark.m_minValue);
	pLabelAxis->SetAttribute("maxValue", m_axis.m_xAxis.m_labelmark.m_maxValue);
	pLabelAxis->SetAttribute("intervalValue", m_axis.m_xAxis.m_labelmark.m_intervalValue);
	pLabelAxis->SetAttribute("format", m_axis.m_xAxis.m_labelmark.m_format.data());
	pLabelAxis->SetAttribute("significat_digits", m_axis.m_xAxis.m_labelmark.m_significant_digits);
	pXAxis->InsertEndChild(*pLabelAxis);
	pParameter->InsertEndChild(*pXAxis);
	std::shared_ptr<TiXmlElement> pYAxis(new TiXmlElement("yAxis"));
	pYAxis->SetAttribute("showAxis", (int)m_axis.m_yAxis.m_showAxis);
	pYAxis->SetAttribute("TextTitle", m_axis.m_yAxis.m_textTitle.data());
	pYAxis->SetAttribute("minValue", m_axis.m_yAxis.m_minValue);
	pYAxis->SetAttribute("maxValue", m_axis.m_yAxis.m_maxValue);
	std::shared_ptr<TiXmlElement> pColorRgbY(new TiXmlElement("colorRGB"));
	pColorRgbY->SetAttribute("r", m_axis.m_xAxis.m_rgb.m_r);
	pColorRgbY->SetAttribute("g", m_axis.m_xAxis.m_rgb.m_g);
	pColorRgbY->SetAttribute("b", m_axis.m_xAxis.m_rgb.m_b);
	pYAxis->InsertEndChild(*pColorRgbY);
	std::shared_ptr<TiXmlElement> pLabelAxisY = std::make_shared<TiXmlElement>("labelMarkAxis");
	pLabelAxisY->SetAttribute("showLabelMark", m_axis.m_yAxis.m_labelmark.m_showLabelMark);
	pLabelAxisY->SetAttribute("minValue", m_axis.m_yAxis.m_labelmark.m_minValue);
	pLabelAxisY->SetAttribute("maxValue", m_axis.m_yAxis.m_labelmark.m_maxValue);
	pLabelAxisY->SetAttribute("intervalValue", m_axis.m_yAxis.m_labelmark.m_intervalValue);
	pLabelAxisY->SetAttribute("format", m_axis.m_yAxis.m_labelmark.m_format.data());
	pLabelAxisY->SetAttribute("significat_digits", m_axis.m_yAxis.m_labelmark.m_significant_digits);
	pYAxis->InsertEndChild(*pLabelAxisY);
	pParameter->InsertEndChild(*pYAxis);
	std::shared_ptr<TiXmlElement> pZAxis(new TiXmlElement("zAxis"));
	pZAxis->SetAttribute("showAxis", (int)m_axis.m_zAxis.m_showAxis);
	pZAxis->SetAttribute("TextTitle", m_axis.m_zAxis.m_textTitle.data());
	pZAxis->SetAttribute("minValue", m_axis.m_zAxis.m_minValue);
	pZAxis->SetAttribute("maxValue", m_axis.m_zAxis.m_maxValue);
	std::shared_ptr<TiXmlElement> pColorRgbZ(new TiXmlElement("colorRGB"));
	pColorRgbZ->SetAttribute("r", m_axis.m_xAxis.m_rgb.m_r);
	pColorRgbZ->SetAttribute("g", m_axis.m_xAxis.m_rgb.m_g);
	pColorRgbZ->SetAttribute("b", m_axis.m_xAxis.m_rgb.m_b);
	pZAxis->InsertEndChild(*pColorRgbZ);
	std::shared_ptr<TiXmlElement> pLabelAxisZ = std::make_shared<TiXmlElement>("labelMarkAxis");
	pLabelAxisZ->SetAttribute("showLabelMark", m_axis.m_zAxis.m_labelmark.m_showLabelMark);
	pLabelAxisZ->SetAttribute("minValue", m_axis.m_zAxis.m_labelmark.m_minValue);
	pLabelAxisZ->SetAttribute("maxValue", m_axis.m_zAxis.m_labelmark.m_maxValue);
	pLabelAxisZ->SetAttribute("intervalValue", m_axis.m_zAxis.m_labelmark.m_intervalValue);
	pLabelAxisZ->SetAttribute("format", m_axis.m_zAxis.m_labelmark.m_format.data());
	pLabelAxisZ->SetAttribute("significat_digits", m_axis.m_zAxis.m_labelmark.m_significant_digits);
	pZAxis->InsertEndChild(*pLabelAxisZ);
	pParameter->InsertEndChild(*pZAxis);
#endif
	pElemNode->InsertEndChild(*pParameter);
	return pElemNode;
}

bool GAxis::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
		return SetProjectError;
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError;
	TiXmlElement* pElement = (TiXmlElement*)(node);
	TiXmlElement* pParameter = (pElement->FirstChildElement("Parameters"));
	if (!pParameter)
		return SetProjectError;
	m_axis.fromXml(pParameter);
#if 0
	m_axis.m_putIn = pParameter->Attribute("putIn");
	if (TIXML_SUCCESS != pParameter->QueryBoolAttribute("showGridLine", &m_axis.m_display_gridLine))
		return SetProjectError;
	if (TIXML_SUCCESS != pParameter->QueryUnsignedAttribute("lineWidth", (unsigned int*)&m_axis.m_lineWidth))
		return SetProjectError;
	if (TIXML_SUCCESS != pParameter->QueryUnsignedAttribute("font", (unsigned int*)&m_axis.m_font))
		return SetProjectError;
	if (TIXML_SUCCESS != pParameter->QueryBoolAttribute("showArrows", &m_axis.m_showArrows))
		return SetProjectError;
	if (TIXML_SUCCESS != pParameter->QueryFloatAttribute("axisDiameter", &m_axis.m_axisDiameter))
		return SetProjectError;
	if (TIXML_SUCCESS != pParameter->QueryUnsignedAttribute("labelMarkSize", (unsigned int*)&m_axis.m_labelMarkSize))
		return SetProjectError;
	if (TIXML_SUCCESS != pParameter->QueryUnsignedAttribute("textSize", (unsigned int*)&m_axis.m_textSize))
		return SetProjectError;
	TiXmlElement* pRgb = (pParameter->FirstChildElement("rgb"));
	if (!pRgb)
		return SetProjectError;
	if(TIXML_SUCCESS != pRgb->QueryUnsignedAttribute("r", (unsigned int*)&m_axis.m_LineRgb.m_r))
		return SetProjectError;
	if(TIXML_SUCCESS != pRgb->QueryUnsignedAttribute("g", (unsigned int*)&m_axis.m_LineRgb.m_g))
		return SetProjectError;
	if(TIXML_SUCCESS != pRgb->QueryUnsignedAttribute("b", (unsigned int*)&m_axis.m_LineRgb.m_b))
		return SetProjectError;
	TiXmlElement* pXaixs = (pRgb->NextSiblingElement("xAxis"));
	if(!pXaixs)
		return SetProjectError;
	if(TIXML_SUCCESS != pXaixs->QueryBoolAttribute("showAxis", &m_axis.m_xAxis.m_showAxis))
		return SetProjectError;
	m_axis.m_xAxis.m_textTitle = pXaixs->Attribute("TextTitle");
	if(m_axis.m_xAxis.m_textTitle.empty())
		return SetProjectError;
	if (TIXML_SUCCESS != pXaixs->QueryIntAttribute("minValue", &m_axis.m_xAxis.m_minValue))
		return SetProjectError;
	if(TIXML_SUCCESS != pXaixs->QueryIntAttribute("maxValue", &m_axis.m_xAxis.m_maxValue))
		return SetProjectError;
	TiXmlElement* colorRGBX = (pXaixs->NextSiblingElement("colorRGB"));
	if (!colorRGBX)
		return SetProjectError;
	if (TIXML_SUCCESS != colorRGBX->QueryUnsignedAttribute("r", (unsigned int *)&m_axis.m_xAxis.m_rgb.m_r))
		return SetProjectError;
	if (TIXML_SUCCESS != colorRGBX->QueryUnsignedAttribute("g", (unsigned int *)&m_axis.m_xAxis.m_rgb.m_g))
		return SetProjectError;
	if (TIXML_SUCCESS != colorRGBX->QueryUnsignedAttribute("b", (unsigned int *)&m_axis.m_xAxis.m_rgb.m_b))
		return SetProjectError;

	TiXmlElement* pLabelAxis = (colorRGBX->NextSiblingElement("labelMarkAxis"));
	if (!pLabelAxis)
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis->QueryBoolAttribute("showLabelMark", &m_axis.m_xAxis.m_labelmark.m_showLabelMark))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis->QueryIntAttribute("minValue", &m_axis.m_xAxis.m_labelmark.m_minValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis->QueryIntAttribute("maxValue", &m_axis.m_xAxis.m_labelmark.m_maxValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis->QueryIntAttribute("intervalValue", &m_axis.m_xAxis.m_labelmark.m_intervalValue))
		return SetProjectError;
	m_axis.m_xAxis.m_labelmark.m_format = pLabelAxis->Attribute("format");
	if (m_axis.m_xAxis.m_labelmark.m_format.empty())
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis->QueryUnsignedAttribute("significat_digits", (unsigned int*)&m_axis.m_xAxis.m_labelmark.m_significant_digits))
		return SetProjectError;
	//
	TiXmlElement* pYaixs = (pRgb->NextSiblingElement("yAxis"));
	if (!pYaixs)
		return SetProjectError;
	if (TIXML_SUCCESS != pYaixs->QueryBoolAttribute("showAxis", &m_axis.m_yAxis.m_showAxis))
		return SetProjectError;
	m_axis.m_yAxis.m_textTitle = pYaixs->Attribute("TextTitle");
	if (m_axis.m_yAxis.m_textTitle.empty())
		return SetProjectError;
	if (TIXML_SUCCESS != pYaixs->QueryIntAttribute("minValue", &m_axis.m_yAxis.m_minValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pYaixs->QueryIntAttribute("maxValue", &m_axis.m_yAxis.m_maxValue))
		return SetProjectError;
	TiXmlElement* pColorbar_y = (pYaixs->NextSiblingElement("colorRGB"));
	if (!pColorbar_y)
		return SetProjectError;
	if (TIXML_SUCCESS != pColorbar_y->QueryUnsignedAttribute("r", (unsigned int *)&m_axis.m_yAxis.m_rgb.m_r))
		return SetProjectError;
	if (TIXML_SUCCESS != pColorbar_y->QueryUnsignedAttribute("g", (unsigned int *)&m_axis.m_yAxis.m_rgb.m_g))
		return SetProjectError;
	if (TIXML_SUCCESS != pColorbar_y->QueryUnsignedAttribute("b", (unsigned int *)&m_axis.m_yAxis.m_rgb.m_b))
		return SetProjectError;

	TiXmlElement* pLabelAxis_y = (pColorbar_y->NextSiblingElement("labelMarkAxis"));
	if (!pLabelAxis_y)
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_y->QueryBoolAttribute("showLabelMark", &m_axis.m_yAxis.m_labelmark.m_showLabelMark))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_y->QueryIntAttribute("minValue", &m_axis.m_yAxis.m_labelmark.m_minValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_y->QueryIntAttribute("maxValue", &m_axis.m_yAxis.m_labelmark.m_maxValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_y->QueryIntAttribute("intervalValue", &m_axis.m_yAxis.m_labelmark.m_intervalValue))
		return SetProjectError;
	m_axis.m_yAxis.m_labelmark.m_format = pLabelAxis_y->Attribute("format");
	if (m_axis.m_yAxis.m_labelmark.m_format.empty())
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_y->QueryUnsignedAttribute("significat_digits", (unsigned int*)&m_axis.m_yAxis.m_labelmark.m_significant_digits))
		return SetProjectError;

	//
	TiXmlElement* pZaixs = (pRgb->NextSiblingElement("zAxis"));
	if (!pZaixs)
		return SetProjectError;
	if (TIXML_SUCCESS != pZaixs->QueryBoolAttribute("showAxis", &m_axis.m_zAxis.m_showAxis))
		return SetProjectError;
	m_axis.m_zAxis.m_textTitle = pZaixs->Attribute("TextTitle");
	if (m_axis.m_zAxis.m_textTitle.empty())
		return SetProjectError;
	if (TIXML_SUCCESS != pZaixs->QueryIntAttribute("minValue", &m_axis.m_zAxis.m_minValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pZaixs->QueryIntAttribute("maxValue", &m_axis.m_zAxis.m_maxValue))
		return SetProjectError;
	TiXmlElement* pColorbar_z = (pZaixs->NextSiblingElement("colorRGB"));
	if (!colorRGBX)
		return SetProjectError;
	if (TIXML_SUCCESS != colorRGBX->QueryUnsignedAttribute("r", (unsigned int *)&m_axis.m_zAxis.m_rgb.m_r))
		return SetProjectError;
	if (TIXML_SUCCESS != colorRGBX->QueryUnsignedAttribute("g", (unsigned int *)&m_axis.m_zAxis.m_rgb.m_g))
		return SetProjectError;
	if (TIXML_SUCCESS != colorRGBX->QueryUnsignedAttribute("b", (unsigned int *)&m_axis.m_zAxis.m_rgb.m_b))
		return SetProjectError;
	
	TiXmlElement* pLabelAxis_z = (pColorbar_z->NextSiblingElement("labelMarkAxis"));
	if (!pLabelAxis_z)
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_z->QueryBoolAttribute("showLabelMark", &m_axis.m_zAxis.m_labelmark.m_showLabelMark))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_z->QueryIntAttribute("minValue", &m_axis.m_zAxis.m_labelmark.m_minValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_z->QueryIntAttribute("maxValue", &m_axis.m_zAxis.m_labelmark.m_maxValue))
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_z->QueryIntAttribute("intervalValue", &m_axis.m_zAxis.m_labelmark.m_intervalValue))
		return SetProjectError;
	m_axis.m_zAxis.m_labelmark.m_format = pLabelAxis_z->Attribute("format");
	if (m_axis.m_zAxis.m_labelmark.m_format.empty())
		return SetProjectError;
	if (TIXML_SUCCESS != pLabelAxis_z->QueryUnsignedAttribute("significat_digits", (unsigned int*)&m_axis.m_zAxis.m_labelmark.m_significant_digits))
		return SetProjectError;

#endif
	return true;
}

//@param origin和target都是三元素的一维数组double[3]，length决定可视化向量的长度
vtkSmartPointer < vtkPolyData> GAxis::CreateSingleVectorArrowData(double* origin, double* target, double length)
{
	vtkMath* math;
	math->Subtract(target, origin, target); //计算向量的朝向 target=target-origin

	vtkSmartPointer < vtkPoints> points = vtkSmartPointer < vtkPoints>::New();  //记录起点坐标
	points->InsertNextPoint(origin);

	vtkSmartPointer < vtkVertex > vertex = vtkSmartPointer < vtkVertex>::New();  //建立起点的拓扑(不建立拓扑的话是不行的)
	vertex->GetPointIds()->SetNumberOfIds(points->GetNumberOfPoints());
	for (int i = 0; i < points->GetNumberOfPoints(); i++) {
		vertex->GetPointIds()->SetId(i, i); //setId(拓扑的id, 顶点的id)
	}

	vtkSmartPointer < vtkDoubleArray> normals = vtkSmartPointer < vtkDoubleArray>::New();  //创建法向量属性，存入向量的朝向target
	normals->SetNumberOfComponents(3);
	normals->InsertNextTuple(target);

	vtkSmartPointer < vtkDoubleArray> scalars = vtkSmartPointer < vtkDoubleArray>::New();  //创建标量属性，存入向量的长度length
	scalars->SetNumberOfComponents(1);
	scalars->SetName("scalars");
	scalars->InsertNextTuple1(length);

	vtkSmartPointer<vtkCellArray> vertices =
		vtkSmartPointer<vtkCellArray>::New();
	vertices->InsertNextCell(vertex);	//将建立的拓扑用vtkCellArray封装，用于赋予vtkPolyData

	vtkSmartPointer < vtkPolyData> polydata = vtkSmartPointer < vtkPolyData>::New();  //创建几何实体
	polydata->SetPoints(points);	//赋予起点
	polydata->SetVerts(vertices);	//赋予拓扑
	polydata->GetPointData()->SetNormals(normals);	//赋予向量朝向 
	//polydata->GetPointData()->SetScalars(scalars);	//赋予向量长度
	return polydata;
}

vtkSmartPointer < vtkGlyph3D> GAxis::GenerateNormalsArrow(vtkSmartPointer < vtkPolyData> polyData)
{
	vtkSmartPointer < vtkArrowSource >arrow = vtkSmartPointer < vtkArrowSource>::New();
	arrow->Update();

	vtkSmartPointer < vtkGlyph3D> glyph = vtkSmartPointer < vtkGlyph3D>::New();
	glyph->SetInputData(polyData);
	glyph->SetSourceData(arrow->GetOutput());
	glyph->SetScaleFactor(m_axis.m_axisDiameter);
	glyph->SetVectorModeToUseNormal();
	glyph->Update();
	return glyph;
}


#define string_To_UTF8(p1) p1
//ok #define FONT_PATH "C:/Windows/Fonts/simfang.ttf"	// yzw 0523
#define FONT_PATH "C:\\Windows\\Fonts\\simfang.ttf"

bool GAxis::run()	// yzw 0606
{
	if (m_excuted)
		return true;
	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	auto pos = vtkSmartPointer<vtkStringArray>::New();
	auto	idtype = points->InsertNextPoint(m_dataRange[0]+1, m_dataRange[1]+1, m_dataRange[2]);
	char title_origin[64];
	snprintf(title_origin, sizeof(title_origin), "(%8.3f, %8.3f, %8.3f)", m_dataRange[0], m_dataRange[1], m_dataRange[2]);	cells->InsertNextCell(1, &idtype);
	pos->InsertNextValue(title_origin);
	pos->SetName("pos");

	auto outlinePoints = vtkSmartPointer<vtkPolyData>::New();
	outlinePoints->SetPoints(points);
	outlinePoints->SetVerts(cells);
	outlinePoints->GetPointData()->AddArray(pos);

	vtkSmartPointer<vtkLabeledDataMapper> m_pointLabelMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
	m_pointLabelMapper->SetInputData(outlinePoints);
	m_pointLabelMapper->SetLabelModeToLabelFieldData();
	m_pointLabelMapper->SetFieldDataName("pos");
	m_pointLabelMapper->GetLabelTextProperty()->SetFontSize(20);
	m_pointLabelMapper->GetLabelTextProperty()->SetFontFamily(VTK_FONT_FILE); // SetFontFamilyToArial();// 
	m_pointLabelMapper->GetLabelTextProperty()->SetFontFile(FONT_PATH);

	//m_actor_2d = vtkSmartPointer<vtkActor2D>::New();
	m_textActor->SetMapper(m_pointLabelMapper);

	double tLenX = m_dataRange[3] - m_dataRange[0];
	double tLenY = m_dataRange[4] - m_dataRange[1];
	double tLenZ = m_dataRange[5] - m_dataRange[2];
	double tLen = tLenX < tLenY ? tLenY : tLenX;
	tLen = tLenZ < tLen ? tLen : tLenZ;

	// yzw 0606	m_axisActor->SetTotalLength(std::abs(m_bounds[1] - m_bounds[0]), std::abs(m_bounds[3] - m_bounds[2]), std::abs(m_bounds[5] - m_bounds[4]));
		// https://kitware.github.io/vtk-examples/site/Python/Visualization/CubeAxesActor/
	    m_cubeAxesActor->SetUse2DMode(true);
	//	not works m_cubeAxesActor->SetPosition(m_dataRange);
		/**/m_cubeAxesActor->SetBounds(
			m_dataRange[0], m_dataRange[0]+tLen,	// m_dataRange[3], // +m_axis.m_xAxis.m_minValue/10		+m_axis.m_xAxis.m_maxValue/10
			m_dataRange[1], m_dataRange[1]+tLen,	// m_dataRange[4],	// +m_axis.m_yAxis.m_minValue/10		+m_axis.m_yAxis.m_maxValue/10
			m_dataRange[2], m_dataRange[2]+tLen	// m_dataRange[5]	// +m_axis.m_zAxis.m_minValue/10		+m_axis.m_zAxis.m_maxValue/10
		);
	m_cubeAxesActor->SetGridLineLocation(0);	
	//double origin[] = { -0.9387755102, 0, 0 };
	//m_cubeAxesActor->SetAxisOrigin(m_dataRange); m_cubeAxesActor->SetUseAxisOrigin(1);
    m_cubeAxesActor->SetFlyModeToStaticTriad();

    m_cubeAxesActor->SetDrawXGridlines(m_axis.m_display_gridLine);
    m_cubeAxesActor->SetDrawYGridlines(m_axis.m_display_gridLine);
    m_cubeAxesActor->SetDrawZGridlines(m_axis.m_display_gridLine);

    m_cubeAxesActor->GetXAxesGridlinesProperty()->SetColor(m_axis.m_LineRgb.m_r / 255.0, m_axis.m_LineRgb.m_g / 255.0, m_axis.m_LineRgb.m_b / 255.0);
    m_cubeAxesActor->GetYAxesGridlinesProperty()->SetColor(m_axis.m_LineRgb.m_r / 255.0, m_axis.m_LineRgb.m_g / 255.0, m_axis.m_LineRgb.m_b / 255.0);
    m_cubeAxesActor->GetZAxesGridlinesProperty()->SetColor(m_axis.m_LineRgb.m_r / 255.0, m_axis.m_LineRgb.m_g / 255.0, m_axis.m_LineRgb.m_b / 255.0);
    
    m_cubeAxesActor->GetXAxesGridlinesProperty()->SetLineWidth(m_axis.m_lineWidth);
    m_cubeAxesActor->GetYAxesGridlinesProperty()->SetLineWidth(m_axis.m_lineWidth);
    m_cubeAxesActor->GetZAxesGridlinesProperty()->SetLineWidth(m_axis.m_lineWidth);
    
    m_cubeAxesActor->GetLabelTextProperty(0)->SetFontFamily(VTK_FONT_FILE);
    m_cubeAxesActor->GetLabelTextProperty(0)->SetFontFile(FONT_PATH);	// "C:/Windows/Fonts/simfang.ttf"
    m_cubeAxesActor->GetLabelTextProperty(1)->SetFontFamily(VTK_FONT_FILE);
    m_cubeAxesActor->GetLabelTextProperty(1)->SetFontFile(FONT_PATH);
    m_cubeAxesActor->GetLabelTextProperty(2)->SetFontFamily(VTK_FONT_FILE);
    m_cubeAxesActor->GetLabelTextProperty(2)->SetFontFile(FONT_PATH);
    m_cubeAxesActor->GetTitleTextProperty(0)->SetFontFamily(VTK_FONT_FILE);
    m_cubeAxesActor->GetTitleTextProperty(0)->SetFontFile(FONT_PATH);
    m_cubeAxesActor->GetTitleTextProperty(1)->SetFontFamily(VTK_FONT_FILE);
    m_cubeAxesActor->GetTitleTextProperty(1)->SetFontFile(FONT_PATH);
    m_cubeAxesActor->GetTitleTextProperty(2)->SetFontFamily(VTK_FONT_FILE);
    m_cubeAxesActor->GetTitleTextProperty(2)->SetFontFile(FONT_PATH);

    m_cubeAxesActor->GetLabelTextProperty(0)->SetFontSize(m_axis.m_labelMarkSize);
    m_cubeAxesActor->GetLabelTextProperty(1)->SetFontSize(m_axis.m_labelMarkSize);
    m_cubeAxesActor->GetLabelTextProperty(2)->SetFontSize(m_axis.m_labelMarkSize);

    m_cubeAxesActor->GetXAxesLinesProperty()->SetLineWidth(m_axis.m_axisDiameter);
    m_cubeAxesActor->GetYAxesLinesProperty()->SetLineWidth(m_axis.m_axisDiameter);
    m_cubeAxesActor->GetZAxesLinesProperty()->SetLineWidth(m_axis.m_axisDiameter);
  
    m_cubeAxesActor->GetTitleTextProperty(0)->SetFontSize(m_axis.m_textSize);
    m_cubeAxesActor->GetTitleTextProperty(1)->SetFontSize(m_axis.m_textSize);
    m_cubeAxesActor->GetTitleTextProperty(2)->SetFontSize(m_axis.m_textSize);
	
    vtkUnicodeString str = vtkUnicodeString::from_utf8(string_To_UTF8(m_axis.m_xAxis.m_textTitle));

    m_cubeAxesActor->SetXAxisVisibility(m_axis.m_xAxis.m_showAxis);   //
	//m_cubeAxesActor->SetXAxisRange(m_dataRange[0], m_dataRange[0] + tLen);// (m_dataRange[0] + m_axis.m_xAxis.m_minValue / 10, m_dataRange[3] + m_axis.m_xAxis.m_maxValue / 10);// (m_axis.m_xAxis.m_labelmark.m_minValue, m_axis.m_xAxis.m_labelmark.m_maxValue);
	m_cubeAxesActor->SetXAxisRange(0, tLen);// (m_dataRange[0] + m_axis.m_xAxis.m_minValue / 10, m_dataRange[3] + m_axis.m_xAxis.m_maxValue / 10);// (m_axis.m_xAxis.m_labelmark.m_minValue, m_axis.m_xAxis.m_labelmark.m_maxValue);
    m_cubeAxesActor->SetXTitle(str.utf8_str());
    m_cubeAxesActor->GetXAxesLinesProperty()->SetColor(m_axis.m_xAxis.m_rgb.m_r / 255.0, m_axis.m_xAxis.m_rgb.m_g / 255.0, m_axis.m_xAxis.m_rgb.m_b / 255.0);
    m_cubeAxesActor->SetXAxisLabelVisibility(m_axis.m_xAxis.m_labelmark.m_showLabelMark);
    m_cubeAxesActor->SetXAxisTickVisibility(true);
	// 长度大于10则显示小刻度
	m_cubeAxesActor->SetXAxisMinorTickVisibility(true);// ((m_dataRange[0] + m_axis.m_xAxis.m_minValue / 10) + 10 < (m_dataRange[3] + m_axis.m_xAxis.m_maxValue / 10));
    //m_cubeAxesActor->SetXLabelFormat("%5.2e");//5有效数字 2小数位数 f小数 e科学计数法 和printf一样
     str = vtkUnicodeString::from_utf8(string_To_UTF8(m_axis.m_yAxis.m_textTitle));
    m_cubeAxesActor->SetYAxisVisibility(m_axis.m_yAxis.m_showAxis);// + m_axis.m_yAxis.m_minValue / 10		+ m_axis.m_yAxis.m_maxValue / 10
	//m_cubeAxesActor->SetYAxisRange(m_dataRange[1], m_dataRange[1] + tLen);// (m_dataRange[1], m_dataRange[1] + tLenY /10);// m_dataRange[4] );// m_axis.m_yAxis.m_labelmark.m_minValue, m_axis.m_yAxis.m_labelmark.m_maxValue);
	m_cubeAxesActor->SetYAxisRange(0, tLen);// (m_dataRange[1], m_dataRange[1] + tLenY /10);// m_dataRange[4] );// m_axis.m_yAxis.m_labelmark.m_minValue, m_axis.m_yAxis.m_labelmark.m_maxValue);
    m_cubeAxesActor->SetYTitle(str.utf8_str());
    m_cubeAxesActor->GetYAxesLinesProperty()->SetColor(m_axis.m_yAxis.m_rgb.m_r / 255.0, m_axis.m_yAxis.m_rgb.m_g / 255.0, m_axis.m_yAxis.m_rgb.m_b / 255.0);
    m_cubeAxesActor->SetYAxisLabelVisibility(m_axis.m_yAxis.m_labelmark.m_showLabelMark);
    m_cubeAxesActor->SetYAxisTickVisibility(true);
    m_cubeAxesActor->SetYAxisMinorTickVisibility(true);// ((m_dataRange[1] + m_axis.m_yAxis.m_minValue / 10)+10< (m_dataRange[4] + m_axis.m_yAxis.m_maxValue / 10) );
    //m_cubeAxesActor->SetYLabelFormat("%5.2e");

     str = vtkUnicodeString::from_utf8(string_To_UTF8(m_axis.m_zAxis.m_textTitle));
    m_cubeAxesActor->SetZAxisVisibility(m_axis.m_zAxis.m_showAxis);
	//m_cubeAxesActor->SetZAxisRange(m_dataRange[2], m_dataRange[2] + tLen);//(m_dataRange[2] + m_axis.m_zAxis.m_minValue / 10, m_dataRange[5] + m_axis.m_zAxis.m_maxValue / 10);// m_axis.m_zAxis.m_labelmark.m_minValue, m_axis.m_zAxis.m_labelmark.m_maxValue);
	m_cubeAxesActor->SetZAxisRange(0, tLen);//(m_dataRange[2] + m_axis.m_zAxis.m_minValue / 10, m_dataRange[5] + m_axis.m_zAxis.m_maxValue / 10);// m_axis.m_zAxis.m_labelmark.m_minValue, m_axis.m_zAxis.m_labelmark.m_maxValue);
    m_cubeAxesActor->SetZTitle(str.utf8_str());
    m_cubeAxesActor->GetZAxesLinesProperty()->SetColor(m_axis.m_zAxis.m_rgb.m_r / 255.0, m_axis.m_zAxis.m_rgb.m_g / 255.0, m_axis.m_zAxis.m_rgb.m_b / 255.0);
    m_cubeAxesActor->SetZAxisLabelVisibility(m_axis.m_zAxis.m_labelmark.m_showLabelMark);
    m_cubeAxesActor->SetZAxisTickVisibility(true);
    m_cubeAxesActor->SetZAxisMinorTickVisibility(true);// ((m_dataRange[2] + m_axis.m_zAxis.m_minValue / 10)+10< (m_dataRange[5] + m_axis.m_zAxis.m_maxValue / 10));
    //m_cubeAxesActor->SetZLabelFormat("%5.2e");
	//int sc = (m_axis.m_xAxis.m_labelmark.m_minValue, m_axis.m_xAxis.m_labelmark.m_maxValue);
	//m_cubeAxesActor->SetLabelScaling(true, 1,10,1);

	//m_cubeAxesActor->SetScale(m_axis.m_labelMarkSize, m_axis.m_labelMarkSize, m_axis.m_labelMarkSize);
	double origin_arr[3] = { 0,0,0 };
	double target_arr[3] = { 1,0,0 };
	vtkSmartPointer < vtkPolyData> xpd = CreateSingleVectorArrowData(origin_arr, target_arr, 0.1);
	vtkSmartPointer < vtkGlyph3D> xgpd = GenerateNormalsArrow(xpd);
	target_arr[0] = 0;target_arr[1] = 1;target_arr[2] = 0;
	vtkSmartPointer < vtkPolyData> ypd = CreateSingleVectorArrowData(origin_arr, target_arr, 0.1);
	vtkSmartPointer < vtkGlyph3D> ygpd =  GenerateNormalsArrow(ypd);
	target_arr[0] = 0;target_arr[1] = 0;target_arr[2] = 1;
	vtkSmartPointer < vtkPolyData> zpd = CreateSingleVectorArrowData(origin_arr, target_arr, 0.1);
	vtkSmartPointer < vtkGlyph3D> zgpd = GenerateNormalsArrow(zpd);

	vtkSmartPointer<vtkPolyDataMapper> xarr_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	xarr_mapper->SetInputData(xgpd->GetOutput());
	xArrowActor->SetMapper(xarr_mapper);
	xArrowActor->GetProperty()->SetColor(m_axis.m_xAxis.m_rgb.m_r / 255.0, m_axis.m_xAxis.m_rgb.m_g / 255.0, m_axis.m_xAxis.m_rgb.m_b / 255.0);
	xArrowActor->SetPosition(m_dataRange[0]+tLen, m_dataRange[1], m_dataRange[2]);

	vtkSmartPointer<vtkPolyDataMapper> yarr_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	yarr_mapper->SetInputData(ygpd->GetOutput());
	yArrowActor->SetMapper(yarr_mapper);
	yArrowActor->GetProperty()->SetColor(m_axis.m_yAxis.m_rgb.m_r / 255.0, m_axis.m_yAxis.m_rgb.m_g / 255.0, m_axis.m_yAxis.m_rgb.m_b / 255.0);
	yArrowActor->SetPosition(m_dataRange[0], m_dataRange[1]+tLen, m_dataRange[2]);

	vtkSmartPointer<vtkPolyDataMapper> zarr_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	zarr_mapper->SetInputData(zgpd->GetOutput());
	zArrowActor->SetMapper(zarr_mapper);
	zArrowActor->GetProperty()->SetColor(m_axis.m_zAxis.m_rgb.m_r / 255.0, m_axis.m_zAxis.m_rgb.m_g / 255.0, m_axis.m_zAxis.m_rgb.m_b / 255.0);
	zArrowActor->SetPosition(m_dataRange[0], m_dataRange[1], m_dataRange[2]+tLen );



	if(m_checked){
		auto renderer = m_project.lock()->getRender();
		renderer->AddActor(m_cubeAxesActor);
		renderer->AddActor(m_textActor);
		renderer->AddActor(xArrowActor);
		renderer->AddActor(yArrowActor);
		renderer->AddActor(zArrowActor);
		m_cubeAxesActor->SetCamera(renderer->GetActiveCamera());
	}
	m_excuted = true;
	//makeNextNodeRun();
	return false;
}

bool GAxis::addToRender()
{
	if (m_excuted) {
		auto renderer = m_project.lock()->getRender();
		renderer->AddActor(m_cubeAxesActor);
		renderer->AddActor(m_textActor);
		renderer->AddActor(xArrowActor);
		renderer->AddActor(yArrowActor);
		renderer->AddActor(zArrowActor);
		m_cubeAxesActor->SetCamera(renderer->GetActiveCamera());
	}
	return m_excuted;
}

bool GAxis::removeFromRender()
{
	if (m_excuted)
	{
		auto renderer = m_project.lock()->getRender();
		renderer->RemoveActor(m_cubeAxesActor);
		renderer->RemoveActor(m_textActor);
		renderer->RemoveActor(xArrowActor);
		renderer->RemoveActor(yArrowActor);
		renderer->RemoveActor(zArrowActor);
	}
	return m_excuted;
}

void GAxis::getBound(double bounds[6])
{
	memcpy(bounds, m_bounds, sizeof(double) * 6);
}

void GAxis::setBound(double * bounds)
{
	memcpy(m_bounds, bounds, sizeof(double) * 6);
}

void GAxis::updateDisplay(std::shared_ptr<SuperNodeStruct> axis)
{
}

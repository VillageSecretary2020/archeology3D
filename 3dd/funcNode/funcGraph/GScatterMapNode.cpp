#include "GScatterMapNode.h"
#include "XmlProject.h"
#include "vtkConeSource.h"
#include <vtkCubeSource.h>
#include "vtkExtractGrid.h"
#include "vtkCleanPolyData.h"
#include "vtkLabeledDataMapper.h"
#include "vtkTextProperty.h"

#include "vtkProperty2D.h"
GScatterMapNode::GScatterMapNode(std::shared_ptr<XmlProject> project) :GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_SCATTERMAP_TYPE;
	m_nodeCanAppendNodeTypeList = {
		NodeType::NODE_G_AXIS_TYPE,
		NodeType::NODE_G_BOUNDINGBOX_TYPE,
		//NodeType::NODE_G_CLIPBOARD_TYPE 
	};
	
	m_excuted = false;

	////
	m_polyData = vtkSmartPointer<vtkPolyData>::New();
	m_sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	m_cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	m_glyph3d = vtkSmartPointer<vtkGlyph3D>::New();
	m_lut = vtkSmartPointer<vtkLookupTable>::New();
	m_lutMap.insert(std::pair<int, vtkSmartPointer<vtkLookupTable>>(1, m_lut));
	m_polyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_ScatterActor = vtkSmartPointer<vtkActor>::New();
	m_scalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();
	//m_scalarBar = vtkSmartPointer< vtkScalarBarActor>::New();
	m_pointLabelMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
	m_actor2dPointLabel = vtkSmartPointer<vtkActor2D>::New();
}

GScatterMapNode::~GScatterMapNode()
{
}

bool GScatterMapNode::setParameter(std::shared_ptr<SuperNodeStruct> data)
{
	std::shared_ptr<G_ScatterMap> _data = std::dynamic_pointer_cast<G_ScatterMap>(data);
	if(!_data || !m_project.lock())
		return false;
	SurperNode::setParameter(data);
	//if (m_excuted)
	{

		/*std::shared_ptr<G_ScatterMap> node = std::dynamic_pointer_cast<G_ScatterMap>(scatterMap);
		node->m_render.m_colorBar.m_direction = ColorBarDirection::D_HORIZONTAL;
		node->m_changetype = 5;
		if (!node->m_render.m_colorBar.m_displayOn)
		{
			node->m_render.m_colorBar.m_displayOn = true;
		}

		node->m_changetype = 4;
		node->m_render.m_useRGB = true;
		node->m_render.m_rgb.m_r = 255;
		node->m_render.m_rgb.m_g = 0;
		node->m_render.m_rgb.m_b = 0;*/

		/*node->m_changetype = 1;

		node->m_render.m_symbol = Child_Symbol_ScatterMap::DIAMONDS;*/
		//updateDisplay(scatterMap);
		m_scatterMap = *_data;
		m_scatterMap.setProjectId(m_project.lock()->getProjectId());
		if (m_excuted) {
			removeFromRender();
			m_excuted = false;
			return run();
		}
	}
	return true;
}

std::shared_ptr<SuperNodeStruct> GScatterMapNode::getParameter()
{
	if(!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return std::shared_ptr<SuperNodeStruct>();

	std::shared_ptr<G_ScatterMap> _data = std::make_shared<G_ScatterMap>();
	//memcpy_s(scatterMap.get(), sizeof(G_ScatterMap), &m_scatterMap, sizeof(G_ScatterMap));
	*_data = m_scatterMap;
	SurperNode::getParameter(_data);
	return _data;
}

bool GScatterMapNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return SetProjectError;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError;
	m_scatterMap.m_name = m_nodeName;
	m_scatterMap.m_nodeIndex = m_nodeIndex;
	
	TiXmlElement* pElement = (TiXmlElement * )node;
	TiXmlElement* pParameter = pElement->FirstChildElement("Parameters");
	m_scatterMap.fromXml(pParameter);

	return true;//
}

void GScatterMapNode::getDefault(SuperNodeStruct* st)
{// 默认参数
	G_ScatterMap* pst = (G_ScatterMap*)st;	//	默认参数
	G_ScatterMap m_scatterMap;

	Child_labelMark_Scatter map_mark_tmp;
	map_mark_tmp.m_showOn = true;//false;
	map_mark_tmp.m_markType = 2;
	map_mark_tmp.m_densityIndex = 0;
	map_mark_tmp.m_labelMarkIndex = 4;
	map_mark_tmp.m_numCount = 4;
	map_mark_tmp.m_showLine = true;

	Child_Render_ScatterMap map_tmp;
	map_tmp.m_symbol = Child_Symbol_ScatterMap::BALL;
	map_tmp.m_colorBar.getDefault();
	map_tmp.m_colorBar.m_title.m_title = "scatter";
	map_tmp.m_Symbol_size = 0.5;
	//map_tmp.m_scale_ScalarsOn = false;
	map_tmp.m_labelMark = map_mark_tmp;

	m_scatterMap.m_density = 01.0;
	m_scatterMap.m_render = map_tmp;
	*pst = m_scatterMap;
}

bool GScatterMapNode::run()
{
	if (m_excuted)
		return true;
	//	if(m_scatterMap.m_putIn.empty() || m_data.empty())
	//		return false;
	//	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
	//	vtkSmartPointer<vtkDoubleArray> dataVector = vtkSmartPointer<vtkDoubleArray>::New();
	//	dataVector->SetNumberOfValues(m_data.size());
	//	int i = 0;
	//	for each (auto var in m_data)
	//	{
	//		vtkIdType pid[1];
	//		pid[0] = points->InsertNextPoint(var.x, var.y, var.z);
	//		vertices->InsertNextCell(1, pid);
	//		dataVector->InsertTuple1(i++, var.scalar);
	//		
	//	}
	//	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	//	//points->GetPoints(id_list, points);
	//	polyData->SetPoints(points);
	//	polyData->SetVerts(vertices);
	//	polyData->GetPointData()->SetScalars(dataVector);
	//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//	/*mapper->SetInputData(polyData);
	//	double range[2];
	//	dataVector->GetRange(range);
	//	mapper->SetScalarRange(range);
	//	mapper->ScalarVisibilityOn();*/
	//	
	//	/*vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
	//	glyph->SetInputData(polyData);
	//	glyph->Update();
	//	mapper->SetInputData(glyph->GetOutput());*/
	//	vtkSmartPointer<vtkConeSource>cone = vtkSmartPointer<vtkConeSource>::New();
	//	cone->SetAngle(26.5651);
	//	vtkSmartPointer<vtkGlyph3D>gly = vtkSmartPointer<vtkGlyph3D>::New();
	//	//gly->SetInputData(polyData);
	//	//gly->SetSourceData(cone->GetOutput());
	//	//mapper->SetInputData(gly->GetOutput());
	//	
	//	vtkSmartPointer<vtkCleanPolyData>m_cleanpoint = vtkSmartPointer<vtkCleanPolyData>::New();
	//	m_cleanpoint->SetInputData(polyData);
	//	m_cleanpoint->SetTolerance(0.01);
	//	m_cleanpoint->Update();
	//	gly->SetInputConnection(m_cleanpoint->GetOutputPort());
	//	gly->SetSourceData(cone->GetOutput());
	//	//mapper->SetInputConnection(m_cleanpoint->GetOutputPort());
	//	mapper->SetInputData(gly->GetOutput());
	//	m_sphereActor->SetMapper(mapper);
	//	//m_sphereActor->GetProperty()->SetColor(color->GetColor4d("Red").GetData());
	//	m_sphereActor->GetProperty()->SetPointSize(m_scatterMap.m_render.m_Symbol_size);
	//	//m_sphereActor->GetProperty()->SetColor(1, 1, 0);
	//	
	//	//标注类actor
	//	vtkSmartPointer<vtkActor2D>actor2D = vtkSmartPointer<vtkActor2D>::New();
	//	vtkSmartPointer<vtkLabeledDataMapper>ldm = vtkSmartPointer<vtkLabeledDataMapper>::New();
	//	ldm->SetInputData(m_cleanpoint->GetOutput());
	//	ldm->SetLabelModeToLabelFieldData();
	//	//ldm->SetLabelFormat("%.2f");  //保留2位小数
	//	ldm->GetLabelTextProperty()->SetFontFamilyToArial();//设置标注字体为Arial字体
	//	ldm->GetLabelTextProperty()->SetFontSize(20); //设置标注字体大小
	//	ldm->GetLabelTextProperty()->SetJustification(1);//设置标注居中，0为靠左，2为靠右
	//	ldm->GetLabelTextProperty()->SetColor(1, 0, 0); //设置标注颜色(红色)
	//	//ldm->SetLabelFormat("2%.3f");//保留三位小数并添加前缀数字2
	//	ldm->SetLabelFormat("%.2f1"); //保留2位小数并添加后缀为1
	//	actor2D->SetMapper(ldm);
	//
	//#if 0
	//	//标注参数写进vtk属性值
	//	std::string sig_number = std::to_string(m_scatterMap.m_render.m_labelMark.m_decimalDigits);
	//	std::string pre = m_scatterMap.m_render.m_labelMark.m_preText;
	//	std::string suf = m_scatterMap.m_render.m_labelMark.m_lastText;
	//	std::string str = "";
	//	str += pre;
	//	str += "%.";
	//	str += sig_number;
	//	str += "f";
	//	str += suf;
	//	ldm->SetLabelFormat(str.c_str());
	//	ldm->GetLabelTextProperty()->SetFontSize(m_scatterMap.m_render.m_labelMark.m_fontSize);
	//#endif 
	//	//实验添加colorBar(色标刻度尺)
	//
	//	vtkSmartPointer<vtkScalarBarActor>colorbar = vtkSmartPointer<vtkScalarBarActor>::New();
	//	vtkSmartPointer<vtkLookupTable>lut = vtkSmartPointer<vtkLookupTable>::New();
	//	lut->Build();
	//	lut->SetAnnotation(0.0, "x");
	//	lut->SetAnnotation(1.0, "y");
	//	lut->SetAnnotation(0.1, "z");
	//	lut->SetAnnotation(0.5, "A");
	//	colorbar->SetTitle("colorBar");
	//	colorbar->GetTitleTextProperty()->SetFontSize(30);
	//	colorbar->SetLookupTable(lut);
	//	colorbar->DrawAnnotationsOn();
	//	colorbar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
	//	colorbar->SetOrientationToVertical(); //标量条竖直放置
	//	colorbar->GetPositionCoordinate()->SetValue(5, 9); // 放置位置
	//	colorbar->SetPosition(70, 12);
	//	colorbar->SetWidth(0.15);
	//	colorbar->SetHeight(0.5);
	//	colorbar->GetAnnotationTextProperty()->SetFontSize(40);
	//	colorbar->GetFrameProperty()->SetColor(1, 1, 0);
	//	
	//#if 0
	//	//色标刻度尺参数写进vtk属性值
	//	if (m_scatterMap.m_render.m_colorBar.m_direction == D_VERTICAL)
	//		colorbar->SetOrientationToVertical();
	//	else
	//		colorbar->SetOrientationToHorizontal();
	//	colorbar->GetPositionCoordinate()->SetValue(m_scatterMap.m_render.m_colorBar.m_xPos, m_scatterMap.m_render.m_colorBar.m_yPos);
	//	colorbar->SetWidth(m_scatterMap.m_render.m_colorBar.m_width);
	//	colorbar->SetHeight(m_scatterMap.m_render.m_colorBar.m_height);
	//	colorbar->SetTitle(m_scatterMap.m_render.m_colorBar.m_title.m_title.c_str());
	//	colorbar->GetTitleTextProperty()->SetFontSize(m_scatterMap.m_render.m_colorBar.m_title.m_fontSize);
	//	colorbar->GetAnnotationTextProperty()->SetFontSize(m_scatterMap.m_render.m_colorBar.m_label.m_fontSize);
	//
	//#endif 
	//	m_sphereActor->GetProperty()->SetColor(color->GetColor4d("Red").GetData());


	/*if (m_scatterMap.m_putIn.empty() || m_data.empty())
		return false;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkDoubleArray> dataVector = vtkSmartPointer<vtkDoubleArray>::New();
	dataVector->SetNumberOfValues(m_data.size());
	int i = 0;
	for each (auto var in m_data)
	{
		vtkIdType pid[1];
		pid[0] = points->InsertNextPoint(var.x, var.y, var.z);
		vertices->InsertNextCell(1, pid);
		dataVector->InsertTuple1(i++, var.scalar);
	}
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);
	polyData->SetVerts(vertices);
	polyData->GetPointData()->SetScalars(dataVector);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(polyData);
	double range[2];
	dataVector->GetRange(range);
	mapper->SetScalarRange(range);
	mapper->ScalarVisibilityOn();
	mapper->ScalarVisibilityOff();
	//vtkSmartPointer<vtkNamedColors> color = vtkSmartPointer<vtkNamedColors>::New();
	glyph = vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInputData(polyData);

	vtkSmartPointer<vtkConeSource>cone = vtkSmartPointer<vtkConeSource>::New();
	cone->SetAngle(26);
	cone->SetHeight(0.001);
	cone->SetRadius(0.0005);
	cone->Update();
	glyph->SetSourceData(cone->GetOutput());
	glyph->Update();
	mapper->SetInputData(glyph->GetOutput());
	m_sphereActor->SetMapper(mapper);
	m_sphereActor->GetProperty()->SetColor(color->GetColor4d("Red").GetData());

	//m_project.lock()->getRender()->AddActor(m_sphereActor);
	m_excuted = true;*/

	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	auto scalars = vtkSmartPointer<vtkDoubleArray>::New();
	scalars->SetNumberOfComponents(1);
	auto Xpos = vtkSmartPointer<vtkDoubleArray>::New();
	Xpos->SetName("Xpos");
	auto Ypos = vtkSmartPointer<vtkDoubleArray>::New();
	Ypos->SetName("Ypos");
	auto Zpos = vtkSmartPointer<vtkDoubleArray>::New();
	Zpos->SetName("Zpos");
	auto XYZpos = vtkSmartPointer<vtkStringArray>::New();
	XYZpos->SetName("XYZpos");

	vtkIdType idtype;
	int iline = 0, itotal=m_data.size();
	int mod= 1 / m_scatterMap.m_density;
	for (auto var : m_data)
	{
		if (iline % mod == 0)
		{
			idtype = points->InsertNextPoint(var.x, var.y, var.z);
			cells->InsertNextCell(1, &idtype);
			scalars->InsertNextValue(var.scalar);
			Xpos->InsertNextValue(var.x);
			Ypos->InsertNextValue(var.y);
			Zpos->InsertNextValue(var.z);
			XYZpos->InsertNextValue(std::to_string(var.x) + "," + std::to_string(var.y) + "," + std::to_string(var.z));
		}
		iline++;
		if (1 == iline % 999)
			set_progress(itotal,  iline, "");
	}
	set_progress(itotal,  iline, "");

	m_polyData->SetPoints(points);
	m_polyData->SetVerts(cells);
	m_polyData->GetPointData()->SetScalars(scalars);
	m_polyData->GetPointData()->AddArray(Xpos);
	m_polyData->GetPointData()->AddArray(Ypos);
	m_polyData->GetPointData()->AddArray(Zpos);
	m_polyData->GetPointData()->AddArray(XYZpos);
	/*
	switch (m_scatterMap.m_render.m_symbol)
	{
	case Child_Symbol_ScatterMap::BALL:
	{
		//m_sphereSource->SetRadius(m_scatterMap.m_render.m_Symbol_size);
		//m_sphereSource->SetThetaResolution(5);
		//m_sphereSource->SetPhiResolution(5);
		m_sphereSource->Update();
		m_glyph3d->SetSourceConnection(m_sphereSource->GetOutputPort());
	}
	break;
	case Child_Symbol_ScatterMap::DIAMONDS:
	{
		//m_cubeSource->SetXLength(m_scatterMap.m_render.m_Symbol_size);
		//m_cubeSource->SetYLength(m_scatterMap.m_render.m_Symbol_size);
		//m_cubeSource->SetZLength(m_scatterMap.m_render.m_Symbol_size);
		m_cubeSource->Update();
		m_glyph3d->SetSourceData(m_cubeSource->GetOutput());
	}
	break;
	default:
	{
		
	}
		break;
	}
	vtkSmartPointer<vtkGlyphSource2D> Glyph2d =
		vtkSmartPointer<vtkGlyphSource2D>::New();//新建二维箭头类型指针
	Glyph2d->SetScale(1);//设置二维箭头比例
	Glyph2d->SetGlyphTypeToSquare();// SetGlyphTypeToArrow();//设置箭头类型为箭头
	Glyph2d->SetFilled(0);//关闭填充
	Glyph2d->SetOutputPointsPrecision(vtkAlgorithm::SINGLE_PRECISION);//设置箭头输出精度
	Glyph2d->Update();
		m_glyph3d->SetSourceData(Glyph2d->GetOutput());
	m_glyph3d->SetScaleFactor(m_scatterMap.m_render.m_Symbol_size);//SetClamping(1);//  
	m_glyph3d->SetScaleModeToDataScalingOff();
	m_glyph3d->SetColorModeToColorByScale();//SetColorModeToColorByScalar(); // SetColorModeToColorByVector();// 
	m_glyph3d->SetInputData(m_polyData);
	//m_glyph3d->SetRange(m_polyData2->GetScalarRange());
	m_glyph3d->Update();*/

	int cr = setLut(m_scatterMap.m_render.m_gradientMap, m_polyData->GetScalarRange(), m_scatterMap.m_render.m_colorBar.m_label);

	m_polyMapper->SetInputData(m_polyData);//(m_glyph3d->GetOutput());
	m_polyMapper->SetLookupTable(m_lut);
	m_polyMapper->SetScalarRange(m_polyData->GetScalarRange());
	//m_polyMapper->ScalarVisibilityOff();

	m_ScatterActor->SetMapper(m_polyMapper);
	m_ScatterActor->GetProperty()->SetPointSize(m_scatterMap.m_render.m_Symbol_size);
	//m_ScatterActor->SetScale();

	m_scalarBar->SetLookupTable(m_lut);
#if 0
	m_scalarBar->SetUnconstrainedFontSize(true);
	if (ColorBarDirection::D_HORIZONTAL == m_scatterMap.m_render.m_colorBar.m_direction)
	{
		m_scalarBar->SetOrientationToHorizontal();
	}
	else if (ColorBarDirection::D_VERTICAL == m_scatterMap.m_render.m_colorBar.m_direction)
	{
		m_scalarBar->SetOrientationToVertical();
	}
	m_scalarBar->GetPositionCoordinate()->SetValue(m_scatterMap.m_render.m_colorBar.m_xPos, m_scatterMap.m_render.m_colorBar.m_yPos);
	m_scalarBar->SetWidth(m_scatterMap.m_render.m_colorBar.m_width);
	m_scalarBar->SetHeight(m_scatterMap.m_render.m_colorBar.m_height);

	vtkUnicodeString str = vtkUnicodeString::from_utf8(m_scatterMap.m_render.m_colorBar.m_title.m_title);
	m_scalarBar->SetTitle(str.utf8_str());
	m_scalarBar->GetTitleTextProperty()->SetFontSize(m_scatterMap.m_render.m_colorBar.m_title.m_fontSize);

	if (m_scatterMap.m_render.m_colorBar.m_label.m_customOn)
	{
		m_scalarBar->SetMaximumNumberOfColors(m_lut->GetNumberOfColors());
		m_scalarBar->SetNumberOfLabels(m_scatterMap.m_render.m_colorBar.m_label.m_num);
		m_scalarBar->GetLabelTextProperty()->SetFontSize(m_scatterMap.m_render.m_colorBar.m_label.m_fontSize);

		std::string  format;
		if (0 == m_scatterMap.m_render.m_colorBar.m_label.m_type)
		{
			format = "%." + std::to_string(m_scatterMap.m_render.m_colorBar.m_label.m_decimalDigits)+"f";

		}
		if (1 == m_scatterMap.m_render.m_colorBar.m_label.m_type)
		{
			format =  "%f";
		}
		else if (2 == m_scatterMap.m_render.m_colorBar.m_label.m_type)
		{
			format =   "%."+ std::to_string(m_scatterMap.m_render.m_colorBar.m_label.m_decimalDigits)+"e";
		}
		format = m_scatterMap.m_render.m_colorBar.m_label.m_preText + format +
			m_scatterMap.m_render.m_colorBar.m_label.m_lastText;
		m_scalarBar->SetLabelFormat(format.c_str());
	}
#else
	setColorBar(m_scatterMap.m_render.m_colorBar);
#endif
	m_excuted = true;
	addToRender();
	//makeNextNodeRun();
	return true;
}


bool GScatterMapNode::addToRender()	// yzw 0608
{
	if (m_excuted) {
		m_project.lock()->getRender()->AddActor(m_ScatterActor);
		if (m_scatterMap.m_render.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
	}
	return m_excuted; // true;	// 
}

bool GScatterMapNode::removeFromRender()	// yzw 0607
{
	if (m_excuted) {
		m_project.lock()->getRender()->RemoveActor(m_ScatterActor);
		m_project.lock()->getRender()->RemoveActor(m_scalarBar);
	}
	return m_excuted;	// false

}

void GScatterMapNode::makeNextNodeRun()
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

void GScatterMapNode::getBound(double bounds[6])
{
	
}

void GScatterMapNode::setBound(double * bounds)
{
}


void GScatterMapNode::updateDisplay(std::shared_ptr<SuperNodeStruct> scatterMap)
{
	std::shared_ptr<G_ScatterMap> node = std::dynamic_pointer_cast<G_ScatterMap>(scatterMap);
	switch (scatterMap->m_changetype)
	{
	case 1:
	{
		if (!node)
		{
			return;
		}
		if (node->m_render.m_symbol == Child_Symbol_ScatterMap::BALL)
		{
			m_sourceType = 0;
			m_scatterMap.m_render.m_Symbol_size = 0.005;//test
			//m_sphereSource->SetRadius(m_scatterMap.m_render.m_Symbol_size);
			//m_sphereSource->SetThetaResolution(5);
			//m_sphereSource->SetPhiResolution(5);
			m_sphereSource->Update();
			m_glyph3d->SetSourceData(m_sphereSource->GetOutput());
			m_glyph3d->Update();
		}
		else if (node->m_render.m_symbol == Child_Symbol_ScatterMap::DIAMONDS)
		{
			m_sourceType = 1;
			m_scatterMap.m_render.m_Symbol_size = 0.0005;//test
			m_cubeSource->SetXLength(m_scatterMap.m_render.m_Symbol_size);
			m_cubeSource->SetYLength(m_scatterMap.m_render.m_Symbol_size);
			m_cubeSource->SetZLength(m_scatterMap.m_render.m_Symbol_size);
			m_cubeSource->Update();
			m_glyph3d->SetSourceData(m_cubeSource->GetOutput());
			m_glyph3d->Update();
		}
		m_scatterMap.m_render.m_symbol = node->m_render.m_symbol;
	}
	break;
	case 2:
	{
		m_scatterMap.m_render.m_Symbol_size = node->m_render.m_Symbol_size;
		if (0 == m_sourceType)
		{
			//m_scatterMap.m_render.m_Symbol_size = 0.005;//test
			//m_sphereSource->SetRadius(m_scatterMap.m_render.m_Symbol_size);
			//m_sphereSource->SetThetaResolution(5);
			//m_sphereSource->SetPhiResolution(5);
			m_sphereSource->Update();
		}
		else
		{
			m_cubeSource->SetXLength(m_scatterMap.m_render.m_Symbol_size);
			m_cubeSource->SetYLength(m_scatterMap.m_render.m_Symbol_size);
			m_cubeSource->SetZLength(m_scatterMap.m_render.m_Symbol_size);
			m_cubeSource->Update();
		}
			//m_scatterMap.m_render.m_Symbol_size = node->m_render.m_Symbol_size;
	}
	break;
	case 3:
	{
		if (node->m_render.m_labelMark.m_showOn)
		{
			if (m_scatterMap.m_render.m_labelMark.m_showOn)
			{
				switch (node->m_render.m_labelMark.m_labelMarkIndex)
				{
				case 0:
				{
					m_pointLabelMapper->SetLabelModeToLabelFieldData();
					m_pointLabelMapper->SetFieldDataName("Xpos");
				}
				break;
				case 1:
				{
					m_pointLabelMapper->SetLabelModeToLabelFieldData();
					m_pointLabelMapper->SetFieldDataName("Ypos");
				}
				break;
				case 2:
				{
					m_pointLabelMapper->SetLabelModeToLabelFieldData();
					m_pointLabelMapper->SetFieldDataName("Zpos");
				}
				break;
				case 3:
				{
					m_pointLabelMapper->SetLabelModeToLabelFieldData();
					m_pointLabelMapper->SetFieldDataName("XYZpos");
				}
				case 4:
				{
					m_pointLabelMapper->SetLabelModeToLabelScalars();
				}
				break;
				default:
					break;
				}
				std::string format_first, format;
				format_first = "%" + std::to_string(m_scatterMap.m_render.m_labelMark.m_numCount) + ".3";
				switch (m_scatterMap.m_render.m_labelMark.m_markType)
				{
				case 2:
				{
					format = format_first + "f";
				}
				break;
				case 3:
				{
					format = format_first + "e";
				}
				break;
				default:
					break;
				}
				m_pointLabelMapper->SetLabelFormat(format.c_str());

				m_pointLabelMapper->GetLabelTextProperty()->SetFontSize(10);
				m_pointLabelMapper->Update();
			}
			else
			{
				m_project.lock()->getRender()->AddActor2D(m_actor2dPointLabel);
			}
		}
		else
		{
			if (m_scatterMap.m_render.m_labelMark.m_showOn)
			{
				m_project.lock()->getRender()->RemoveActor2D(m_actor2dPointLabel);
			}
		}
		memcpy_s(&m_scatterMap.m_render.m_labelMark, sizeof(Child_labelMark_Scatter), &node->m_render.m_labelMark, sizeof(Child_labelMark_Scatter));
	}
	break;
	case 4:
	{
		if (node->m_render.m_useRGB)
		{
			m_polyMapper->SetScalarVisibility(false);
			//m_polyMapper->ScalarVisibilityOff();
			m_polyMapper->Update();
			m_ScatterActor->GetProperty()->SetColor(node->m_render.m_rgb.m_r, node->m_render.m_rgb.m_g, node->m_render.m_rgb.m_b);
			m_ScatterActor->Modified();
		}
		else
		{
			m_polyMapper->SetScalarVisibility(true);
			m_polyMapper->SetLookupTable(m_lutMap[1]);
			m_polyMapper->Update();
			m_scalarBar->SetLookupTable(m_polyMapper->GetLookupTable());
		}
		m_scatterMap.m_render.m_useRGB = node->m_render.m_useRGB;
		memcpy_s(&m_scatterMap.m_render.m_rgb, sizeof(Child_colorRGB), &m_scatterMap.m_render.m_rgb, sizeof(Child_colorRGB));
	}
	break;
	case 5:
	{
#if 0
		if (node->m_render.m_colorBar.m_displayOn)
		{
			if (node->m_render.m_colorBar.m_displayOn)
			{
				if (ColorBarDirection::D_HORIZONTAL == node->m_render.m_colorBar.m_direction)
				{
					m_scalarBar->SetOrientationToHorizontal();

					break;
				}
				else
				{
					m_scalarBar->SetOrientationToVertical();
					break;
				}
				if (node->m_render.m_colorBar.m_xPos != m_scatterMap.m_render.m_colorBar.m_xPos || 
					node->m_render.m_colorBar.m_yPos != m_scatterMap.m_render.m_colorBar.m_yPos)
				{
					m_scalarBar->SetPosition(node->m_render.m_colorBar.m_xPos, node->m_render.m_colorBar.m_yPos);
					m_scalarBar->SetPosition2(node->m_render.m_colorBar.m_xPos + node->m_render.m_colorBar.m_width,
						node->m_render.m_colorBar.m_yPos + node->m_render.m_colorBar.m_height);
					break;
				}
				if (node->m_render.m_colorBar.m_width != m_scatterMap.m_render.m_colorBar.m_width ||
					node->m_render.m_colorBar.m_height != m_scatterMap.m_render.m_colorBar.m_height)
				{
					m_scalarBar->SetPosition2(node->m_render.m_colorBar.m_xPos + node->m_render.m_colorBar.m_width,
						node->m_render.m_colorBar.m_yPos + node->m_render.m_colorBar.m_height);
					break;
				}
				if (node->m_render.m_colorBar.m_title.m_title != m_scatterMap.m_render.m_colorBar.m_title.m_title)
				{
					m_scalarBar->SetTitle(node->m_render.m_colorBar.m_title.m_title.c_str());
					break;
				}
				if (node->m_render.m_colorBar.m_title.m_fontSize != m_scatterMap.m_render.m_colorBar.m_title.m_fontSize)
				{
					m_scalarBar->GetTitleTextProperty()->SetFontSize(node->m_render.m_colorBar.m_title.m_fontSize);
					break;
				}
				if (node->m_render.m_colorBar.m_label.m_num != m_scatterMap.m_render.m_colorBar.m_label.m_num)
				{
					m_scalarBar->SetMaximumNumberOfColors(node->m_render.m_colorBar.m_label.m_num);
					m_scalarBar->SetNumberOfLabels(node->m_render.m_colorBar.m_label.m_num);
					break;
				}
				if (node->m_render.m_colorBar.m_label.m_fontSize != m_scatterMap.m_render.m_colorBar.m_label.m_fontSize)
				{
					m_scalarBar->GetLabelTextProperty()->SetFontSize(node->m_render.m_colorBar.m_label.m_fontSize);
					break;
				}
			}
			else
			{
				m_project.lock()->getRender()->AddActor2D(m_scalarBar);
				break;
			}
		}
		else
		{
			if (m_scatterMap.m_render.m_colorBar.m_displayOn)
			{
				m_project.lock()->getRender()->RemoveActor2D(m_scalarBar);
			}
		}
		memcpy_s(&m_scatterMap.m_render.m_colorBar, sizeof(Child_ColorBar), &node->m_render.m_colorBar, sizeof(Child_ColorBar));
#else
		updateDisplayColorBar(m_scatterMap.m_render.m_colorBar, node->m_render.m_colorBar);
#endif
	}
	break;
	default:
		break;
	}
	m_project.lock()->getRender()->Render();
}

std::shared_ptr<TiXmlNode> GScatterMapNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlNode> pElemNode = std::dynamic_pointer_cast<TiXmlNode>(pNode);
	std::shared_ptr<TiXmlElement> pParameter(new TiXmlElement("Parameters"));
	m_scatterMap.toXml(pParameter);

	pElemNode->InsertEndChild(*pParameter);
	return pElemNode;
}

#include "GSurfaceNode.h"
#include "XmlProject.h"
GSurfaceNode::GSurfaceNode(std::shared_ptr<XmlProject> project):GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_SURFACERENDER_TYPE;

	m_surfaceVOI = vtkSmartPointer<vtkExtractVOI>::New();
	m_excuted = false;
	m_resolution[0] = m_resolution[1] = m_resolution[2] = NAN;
	m_dataRange[0] = m_dataRange[1] = m_dataRange[2] = m_dataRange[3] = m_dataRange[4]
		= m_dataRange[5] = NAN;
	//m_lut = vtkSmartPointer<vtkLookupTable>::New();
	m_dataSetMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	m_surfaceActor = vtkSmartPointer<vtkActor>::New();
	m_edgeActor = vtkSmartPointer<vtkActor>::New();
	m_edgeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	m_edgeLut = vtkSmartPointer<vtkLookupTable>::New();
}

GSurfaceNode::~GSurfaceNode()
{
}

void GSurfaceNode::setCallbackFun(XYZLimits_cb * limits_func, updata_info_cb * info_func, void * ptr)
{
	if (limits_func)
		m_call_fun = limits_func;
	if (info_func)
		m_info_cb = info_func;
	if (ptr)
		m_ptr = ptr;
}

bool GSurfaceNode::setParameter(std::shared_ptr<SuperNodeStruct> data)
{
	if (!m_project.lock())
		return false;
	SurperNode::setParameter(data);
	std::shared_ptr<G_SurfaceRender> _data = std::dynamic_pointer_cast<G_SurfaceRender>(data);
	m_surfaceRender=*_data;
	m_surfaceRender.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return true;
}

std::shared_ptr<SuperNodeStruct> GSurfaceNode::getParameter()
{
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return std::shared_ptr<SuperNodeStruct>();

	std::shared_ptr<G_SurfaceRender> surface = std::make_shared<G_SurfaceRender>();
	//memcpy_s(surface.get(), sizeof(G_SurfaceRender), &m_surfaceRender, sizeof(G_SurfaceRender));
	*surface = m_surfaceRender;
	SurperNode::getParameter(surface);
	return surface;
}

std::shared_ptr<TiXmlNode> GSurfaceNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElementNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_surfaceRender.toXml(pParameters);

	pElementNode->InsertEndChild(*pParameters);
	return pElementNode;
}

bool GSurfaceNode::readParameterFromXmlNode(TiXmlNode * node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError;
	m_surfaceRender.m_name = m_nodeName;
	m_surfaceRender.m_nodeIndex = m_nodeIndex;
	TiXmlElement* pElement = (TiXmlElement*)node;
	TiXmlElement* pParameters = pElement->FirstChildElement("Parameters");
	if(!pParameters)
		return SetProjectError;
	m_surfaceRender.fromXml(pParameters);

	return true;
}

void GSurfaceNode::getDefault(SuperNodeStruct* st)
{	//	默认参数
	G_SurfaceRender* pst = (G_SurfaceRender*)st;
	G_SurfaceRender m_surfaceRender;

	int m_resolution[3] = {30,30,30}; //解析度
	Child_Render_Surface SurfaceRender_Render_tmp;
	SurfaceRender_Render_tmp.m_displayEdge = true;
	SurfaceRender_Render_tmp.m_colorMetod = ColorMethod::GRADIENT_OF_COLOR;
	SurfaceRender_Render_tmp.m_edgeColor.m_r = 0;
	SurfaceRender_Render_tmp.m_edgeColor.m_g = 0;
	SurfaceRender_Render_tmp.m_edgeColor.m_b = 0;

	Child_geometric_attribute SurfaceGeometric_tmp;
	SurfaceGeometric_tmp.m_xAxis.m_firstNodeOfAxis = 0;
	SurfaceGeometric_tmp.m_xAxis.m_lastNodeOfAxis = m_resolution[0]-1;
	SurfaceGeometric_tmp.m_xAxis.m_singleSlice = false;
	SurfaceGeometric_tmp.m_yAxis.m_firstNodeOfAxis = 0;
	SurfaceGeometric_tmp.m_yAxis.m_lastNodeOfAxis = m_resolution[2]-1;
	SurfaceGeometric_tmp.m_yAxis.m_singleSlice = false;
	SurfaceGeometric_tmp.m_zAxis.m_firstNodeOfAxis = 0;
	SurfaceGeometric_tmp.m_zAxis.m_lastNodeOfAxis = m_resolution[2]-1;
	SurfaceGeometric_tmp.m_zAxis.m_singleSlice = false;

	//G_SurfaceRender SurfaceRender_tmp;
	m_surfaceRender.m_blockNum = (m_resolution[2]) * (m_resolution[1]) * (m_resolution[0]); //纯显示
	m_surfaceRender.m_isCalculateVolume = true; //纯显示
	//m_surfaceRender.m_volume = (end_row - begin_row)* (end_col - begin_col)* (end_high - begin_high);//纯显示
	m_surfaceRender.m_render_surface = SurfaceRender_Render_tmp;
	m_surfaceRender.m_geometric = SurfaceGeometric_tmp;
	m_surfaceRender.m_colorBar.getDefault();
	m_surfaceRender.m_colorBar.m_title.m_title = "surface";
	*pst = m_surfaceRender;
}


bool GSurfaceNode::run()
{
	if (m_excuted)
		return true;
	if (!m_project.lock())
		return false;
	if (m_data.empty())
		return false;
	if (NAN == m_resolution[0] || NAN == m_resolution[1] || NAN == m_resolution[2])
		return false;
	if (NAN == m_dataRange[0] || NAN == m_dataRange[1] || NAN == m_dataRange[2]
		|| NAN == m_dataRange[3] || NAN == m_dataRange[4] || NAN == m_dataRange[5])
		return false;

	//测试添加表面渲染参数
	double begin_row = *(m_dataRange);
	double begin_col = *(m_dataRange + 1);// = -0.9387755102;
	double begin_high = *(m_dataRange + 2);
	double end_row = *(m_dataRange + 3);
	double end_col = *(m_dataRange + 4);// = 0.9387755102;
	double end_high = *(m_dataRange + 5);

	double cnt_x = (end_row - begin_row) / (double)(m_resolution[0]-1);
	double cnt_y = (end_col - begin_col) / (double)(m_resolution[1]-1);
	double cnt_z = (end_high - begin_high) / (double)(m_resolution[2]-1);

	//m_surfaceRender.m_geometric.m_xAxis.m_lastNodeOfAxis = m_resolution[0]-1;
	//m_surfaceRender.m_geometric.m_yAxis.m_lastNodeOfAxis = m_resolution[2]-1;
	//m_surfaceRender.m_geometric.m_zAxis.m_lastNodeOfAxis = m_resolution[2]-1;

	m_surfaceRender.m_blockNum = (m_resolution[2]) * (m_resolution[1]) * (m_resolution[0]); 
	m_surfaceRender.m_isCalculateVolume = true; //纯显示
	m_surfaceRender.m_volume = (end_row - begin_row)* (end_col - begin_col)* (end_high - begin_high);//纯显示


	m_imageData = vtkSmartPointer<vtkImageData>::New();
	m_imageData->SetDimensions(m_resolution[0] , m_resolution[1] , m_resolution[2] );
	m_imageData->SetSpacing(cnt_x, cnt_y, cnt_z);
	m_imageData->SetOrigin(begin_row, begin_col, begin_high);
	m_imageData->AllocateScalars(VTK_DOUBLE, 1);

	for (int i = 0; i < m_resolution[0]; i++)
	{
		//double i_f = cnt_x * i + begin_row;
		for (int j = 0; j < m_resolution[1]; j++)
		{
			//double j_f = cnt_y * j + begin_col;
			for (int k = 0; k < m_resolution[2]; k++)
			{
				//double k_f = cnt_z * k + begin_high;
				//*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * 50 * 50 + j * 50 + k].scalar;
				*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * m_resolution[1] * m_resolution[2] + j * m_resolution[2] + k].scalar;
			}
		}
	}

	m_surfaceVOI->SetInputData(m_imageData);
	if (!m_surfaceRender.m_geometric.m_xAxis.m_singleSlice && !m_surfaceRender.m_geometric.m_yAxis.m_singleSlice
		&& !m_surfaceRender.m_geometric.m_zAxis.m_singleSlice)
	{// 先算好体积
		m_surfaceVOI->SetVOI(
			0, m_resolution[0] - 1,
			0, m_resolution[1] - 1,
			0, m_resolution[2] - 1);
		m_surfaceVOI->Update();
		int X_limits[2] = { m_surfaceRender.m_geometric.m_xAxis.m_firstNodeOfAxis, m_surfaceRender.m_geometric.m_xAxis.m_lastNodeOfAxis };
		int Y_limits[2] = { m_surfaceRender.m_geometric.m_yAxis.m_firstNodeOfAxis, m_surfaceRender.m_geometric.m_yAxis.m_lastNodeOfAxis };
		int Z_limits[2] = { m_surfaceRender.m_geometric.m_zAxis.m_firstNodeOfAxis, m_surfaceRender.m_geometric.m_zAxis.m_lastNodeOfAxis };
		m_surfaceRender.m_meanBlock = caculateMeanBlocks(X_limits, Y_limits, Z_limits);
		// 再设置可视部分
		m_surfaceVOI->SetVOI(
			m_surfaceRender.m_geometric.m_xAxis.m_firstNodeOfAxis, m_surfaceRender.m_geometric.m_xAxis.m_lastNodeOfAxis,
			m_surfaceRender.m_geometric.m_yAxis.m_firstNodeOfAxis, m_surfaceRender.m_geometric.m_yAxis.m_lastNodeOfAxis,
			m_surfaceRender.m_geometric.m_zAxis.m_firstNodeOfAxis, m_surfaceRender.m_geometric.m_zAxis.m_lastNodeOfAxis);
		m_surfaceVOI->Update();
	}
	else
	{
		int tempX_begin = m_surfaceRender.m_geometric.m_xAxis.m_firstNodeOfAxis;
		int tempX_end = m_surfaceRender.m_geometric.m_xAxis.m_lastNodeOfAxis;
		int tempY_begin = m_surfaceRender.m_geometric.m_yAxis.m_firstNodeOfAxis;
		int tempY_end = m_surfaceRender.m_geometric.m_yAxis.m_lastNodeOfAxis;
		int tempZ_begin = m_surfaceRender.m_geometric.m_zAxis.m_firstNodeOfAxis;
		int tempZ_end = m_surfaceRender.m_geometric.m_zAxis.m_lastNodeOfAxis;
		if (m_surfaceRender.m_geometric.m_xAxis.m_singleSlice)
		{
			tempX_end = tempX_begin + 1;
		}
		if (m_surfaceRender.m_geometric.m_yAxis.m_singleSlice)
		{
			tempY_end = tempY_begin + 1;
		}
		if (m_surfaceRender.m_geometric.m_zAxis.m_singleSlice)
		{
			tempZ_end = tempZ_begin + 1;
		}// 先算好体积
		m_surfaceVOI->SetVOI(
			0, m_resolution[0] - 1,
			0, m_resolution[1] - 1,
			0, m_resolution[2] - 1);
		m_surfaceVOI->Update();
		int X_limits[2] = { tempX_begin, tempX_end };
		int Y_limits[2] = { tempY_begin, tempY_end };
		int Z_limits[2] = { tempZ_begin, tempZ_end };
		m_surfaceRender.m_meanBlock = caculateMeanBlocks(X_limits, Y_limits, Z_limits);
		m_surfaceVOI->SetVOI(tempX_begin, tempX_end, tempY_begin, tempY_end, tempZ_begin, tempZ_end);
		m_surfaceVOI->Update();
	}

	m_info_cb(m_surfaceRender.m_blockNum, m_surfaceRender.m_meanBlock, m_surfaceRender.m_volume, m_ptr);
	//m_lut->SetHueRange(0.0, 0.6);
	//m_lut->SetAlphaRange(1.0, 1.0);
	//m_lut->SetValueRange(1.0, 1.0);
	//m_lut->SetSaturationRange(1.0, 1.0);
	//m_lut->SetNumberOfTableValues(50);
	//m_lut->SetRange(m_imageData->GetScalarRange());
	//m_lut->Build();
	int cr = setLut(m_surfaceRender.m_render_surface.m_gradientMap, m_imageData->GetScalarRange(), m_surfaceRender.m_colorBar.m_label);
	
	m_dataSetMapper->SetInputConnection(m_surfaceVOI->GetOutputPort());
	m_dataSetMapper->SetLookupTable(m_lut);
	m_dataSetMapper->SetScalarModeToUsePointData();
	//m_dataSetMapper->UseLookupTableScalarRangeOn();
	m_dataSetMapper->SetScalarRange(m_imageData->GetScalarRange());

	m_scalarBar->SetLookupTable(m_dataSetMapper->GetLookupTable());
	setColorBar(m_surfaceRender.m_colorBar);

	m_surfaceActor->SetMapper(m_dataSetMapper);
	m_surfaceActor->GetProperty()->SetRepresentationToSurface();

	m_edgeLut->SetNumberOfTableValues(1);
	m_edgeLut->SetTableValue(0, m_surfaceRender.m_render_surface.m_edgeColor.m_r / 255.0, m_surfaceRender.m_render_surface.m_edgeColor.m_g / 255.0, m_surfaceRender.m_render_surface.m_edgeColor.m_b / 255.0);
	m_edgeLut->SetRange(m_imageData->GetScalarRange());
	m_edgeLut->Build();

	m_edgeMapper->SetInputConnection(m_surfaceVOI->GetOutputPort());
	m_edgeMapper->SetResolveCoincidentTopologyToShiftZBuffer();
	m_edgeMapper->SetLookupTable(m_edgeLut);

	m_edgeActor->SetMapper(m_edgeMapper);
	m_edgeActor->GetProperty()->SetRepresentationToWireframe();
	if(m_checked){
		m_project.lock()->getRender()->AddActor(m_surfaceActor);
		if (m_surfaceRender.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);

		if (m_surfaceRender.m_render_surface.m_displayEdge)
		{
			m_project.lock()->getRender()->AddActor(m_edgeActor);
		}
	}
	m_excuted = true;
	//makeNextNodeRun();
	return true;
}

bool GSurfaceNode::addToRender()	// yzw 0608
{
	if (m_excuted){
		m_project.lock()->getRender()->AddActor(m_surfaceActor);
		if (m_surfaceRender.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
		m_project.lock()->getRender()->AddActor(m_edgeActor);
	}
	return m_excuted;
}

bool GSurfaceNode::removeFromRender()	// yzw 0607
{
	if (m_excuted){
		m_project.lock()->getRender()->RemoveActor(m_surfaceActor);
		m_project.lock()->getRender()->RemoveActor(m_scalarBar);
		m_project.lock()->getRender()->RemoveActor(m_edgeActor);
	}
	return m_excuted;
}

void GSurfaceNode::makeNextNodeRun()
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

void GSurfaceNode::updateDisplay(std::shared_ptr<SuperNodeStruct> surface)
{
	/*
	m_changetype = 1:更改输入分量
	= 2：更改渲染参数
	= 3：更改几何参数
	= 4：更改色标刻度尺
	*/
	double begin_row = *(m_dataRange);
	double begin_col = *(m_dataRange + 1) = -0.9387755102;
	double begin_high = *(m_dataRange + 2);
	double end_row = *(m_dataRange + 3);
	double end_col = *(m_dataRange + 4) = 0.9387755102;
	double end_high = *(m_dataRange + 5);

	double cnt_x = (end_row - begin_row) / (double)m_resolution[0];
	double cnt_y = (end_col - begin_col) / (double)m_resolution[1];
	double cnt_z = (end_high - begin_high) / (double)m_resolution[2];

	std::shared_ptr<G_SurfaceRender> surfaceRender = std::dynamic_pointer_cast<G_SurfaceRender>(surface);
	if (!surfaceRender)
		return;
	switch (surface->m_changetype)
	{
	case 1:
	{

	}
	break;
	case 2:
	{
		if (!surfaceRender->m_render_surface.m_displayEdge)
		{
			m_project.lock()->getRender()->RemoveActor(m_edgeActor);
		}
		else
		{
			m_edgeActor->GetProperty()->SetColor(surfaceRender->m_render_surface.m_edgeColor.m_r,
				surfaceRender->m_render_surface.m_edgeColor.m_g, surfaceRender->m_render_surface.m_edgeColor.m_b);
			/* todo : 处理着色方法，放在最后*/
		}
		memcpy_s(&m_surfaceRender.m_render_surface, sizeof(Child_Render_Surface), &surfaceRender->m_render_surface, sizeof(Child_Render_Surface));
	}
	break;
	case 3:
	{
		if (!surfaceRender->m_geometric.m_xAxis.m_singleSlice && !surfaceRender->m_geometric.m_yAxis.m_singleSlice
			&& !surfaceRender->m_geometric.m_zAxis.m_singleSlice)
		{
			m_surfaceVOI->SetVOI(surfaceRender->m_geometric.m_xAxis.m_firstNodeOfAxis, surfaceRender->m_geometric.m_xAxis.m_lastNodeOfAxis,
				surfaceRender->m_geometric.m_yAxis.m_firstNodeOfAxis, surfaceRender->m_geometric.m_yAxis.m_lastNodeOfAxis,
				surfaceRender->m_geometric.m_zAxis.m_firstNodeOfAxis, surfaceRender->m_geometric.m_zAxis.m_lastNodeOfAxis);
			m_surfaceVOI->Update();
			int X_limits[2] = { surfaceRender->m_geometric.m_xAxis.m_firstNodeOfAxis, surfaceRender->m_geometric.m_xAxis.m_lastNodeOfAxis };
			int Y_limits[2] = { surfaceRender->m_geometric.m_yAxis.m_firstNodeOfAxis, surfaceRender->m_geometric.m_yAxis.m_lastNodeOfAxis };
			int Z_limits[2] = { surfaceRender->m_geometric.m_zAxis.m_firstNodeOfAxis, surfaceRender->m_geometric.m_zAxis.m_lastNodeOfAxis };
			surfaceRender->m_meanBlock = caculateMeanBlocks(X_limits, Y_limits, Z_limits);
			surfaceRender->m_blockNum = (surfaceRender->m_geometric.m_xAxis.m_lastNodeOfAxis - surfaceRender->m_geometric.m_xAxis.m_firstNodeOfAxis)*
				(surfaceRender->m_geometric.m_yAxis.m_lastNodeOfAxis - surfaceRender->m_geometric.m_yAxis.m_firstNodeOfAxis) *
				(surfaceRender->m_geometric.m_zAxis.m_lastNodeOfAxis - surfaceRender->m_geometric.m_zAxis.m_firstNodeOfAxis);
			surfaceRender->m_volume = (end_row - begin_row) * (end_col - begin_col) * (end_high - begin_high);
			if (m_info_cb && m_ptr)
			{
				begin_row = begin_row + (surfaceRender->m_geometric.m_xAxis.m_firstNodeOfAxis - 1) * cnt_x;
				end_row = begin_row + (surfaceRender->m_geometric.m_xAxis.m_lastNodeOfAxis - 1)* cnt_x;
				begin_col = begin_col + (surfaceRender->m_geometric.m_yAxis.m_firstNodeOfAxis - 1)* cnt_y;
				end_col = begin_col + (surfaceRender->m_geometric.m_yAxis.m_lastNodeOfAxis - 1) * cnt_y;
				begin_high = begin_high + (surfaceRender->m_geometric.m_zAxis.m_firstNodeOfAxis - 1) * cnt_z;
				end_high = begin_high + (surfaceRender->m_geometric.m_zAxis.m_lastNodeOfAxis - 1) * cnt_z;
				m_info_cb((surfaceRender->m_geometric.m_xAxis.m_lastNodeOfAxis - surfaceRender->m_geometric.m_xAxis.m_firstNodeOfAxis)*
					(surfaceRender->m_geometric.m_yAxis.m_lastNodeOfAxis - surfaceRender->m_geometric.m_yAxis.m_firstNodeOfAxis) *
					(surfaceRender->m_geometric.m_zAxis.m_lastNodeOfAxis - surfaceRender->m_geometric.m_zAxis.m_firstNodeOfAxis),
					surfaceRender->m_meanBlock,
					(end_row - begin_row) * (end_col - begin_col) * (end_high - begin_high),
					m_ptr);
			}
		}
		else
		{
			int tempX_begin = surfaceRender->m_geometric.m_xAxis.m_firstNodeOfAxis, tempX_end = surfaceRender->m_geometric.m_xAxis.m_lastNodeOfAxis, tempY_begin = surfaceRender->m_geometric.m_yAxis.m_firstNodeOfAxis, tempY_end = surfaceRender->m_geometric.m_yAxis.m_lastNodeOfAxis, tempZ_begin = surfaceRender->m_geometric.m_zAxis.m_firstNodeOfAxis, tempZ_end = surfaceRender->m_geometric.m_zAxis.m_lastNodeOfAxis;
			if (surfaceRender->m_geometric.m_xAxis.m_singleSlice)
			{
				tempX_end = tempX_begin + 1;
			}
			if (surfaceRender->m_geometric.m_yAxis.m_singleSlice)
			{
				tempY_end = tempY_begin + 1;
			}
			if (surfaceRender->m_geometric.m_zAxis.m_singleSlice)
			{
				tempZ_end = tempZ_begin + 1;
			}
			m_surfaceVOI->SetVOI(tempX_begin, tempX_end, tempY_begin, tempY_end, tempZ_begin, tempZ_end);
			m_surfaceVOI->Update();
			int X_limits[2] = { tempX_begin, tempX_end };
			int Y_limits[2] = { tempY_begin, tempY_end };
			int Z_limits[2] = { tempZ_begin, tempZ_end };
			surfaceRender->m_meanBlock = caculateMeanBlocks(X_limits, Y_limits, Z_limits);
			if (m_info_cb && m_ptr)
			{
				begin_row = begin_row + (surfaceRender->m_geometric.m_xAxis.m_firstNodeOfAxis - 1) * cnt_x;
				end_row = begin_row + (tempX_end - 1)* cnt_x;
				begin_col = begin_col + (surfaceRender->m_geometric.m_yAxis.m_firstNodeOfAxis - 1)* cnt_y;
				end_col = begin_col + (tempY_end - 1) * cnt_y;
				begin_high = begin_high + (surfaceRender->m_geometric.m_zAxis.m_firstNodeOfAxis - 1) * cnt_z;
				end_high = begin_high + (tempZ_end - 1) * cnt_z;

				surfaceRender->m_blockNum = (tempX_end - tempX_begin) *(tempY_end - tempY_begin) * (tempZ_end - tempZ_begin);
				surfaceRender->m_volume = (end_row - begin_row) * (end_col - begin_col) * (end_high - begin_high);
				m_info_cb((tempX_end - tempX_begin) *(tempY_end - tempY_begin) * (tempZ_end - tempZ_begin),
					surfaceRender->m_meanBlock,
					(end_row - begin_row) * (end_col - begin_col) * (end_high - begin_high),
					m_ptr);
			}
		}
		m_dataSetMapper->Update();
		memcpy_s(&m_surfaceRender.m_geometric, sizeof(Child_geometric_attribute), &surfaceRender->m_geometric, sizeof(Child_geometric_attribute));
		m_surfaceRender.m_meanBlock = surfaceRender->m_meanBlock;
		m_surfaceRender.m_blockNum = surfaceRender->m_blockNum;
		m_surfaceRender.m_volume = surfaceRender->m_volume;
	}
	break;
	case 4:
	{
		//		memcpy_s(&m_surfaceRender.m_colorBar, sizeof(Child_ColorBar), &surfaceRender->m_colorBar, sizeof(Child_ColorBar));
	}
	break;
	case 5:
	{
		updateDisplayColorBar(m_surfaceRender.m_colorBar, surfaceRender->m_colorBar);
	}
	break;
	default:
		break;
	}
	m_project.lock()->getRender()->Render();
}

double GSurfaceNode::caculateMeanBlocks(int * X_limits, int * Y_limits, int * Z_limits)
{
	double scalarssum = 0;
	int count = 0;
	for (int i = X_limits[0]; i < X_limits[1]; i++)
	{
		for (int j = Y_limits[0]; j < Y_limits[1]; j++)
		{
			for (int k = Z_limits[0]; k < Z_limits[1]; k++)
			{
				scalarssum += *(double*)(m_surfaceVOI->GetOutput()->GetScalarPointer(i, j, k));
				count++;
			}
		}
	}
	if(0 == count)
		return 0.0f;
	return scalarssum / count;
}

#include "GVolumeRenderNode.h"
#include "XmlProject.h"

GVolumeRenderNode::GVolumeRenderNode(std::shared_ptr< XmlProject> project) :GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_VOLUMERENDERING_TYPE;
	m_nodeCanAppendNodeTypeList = {//NodeType::NODE_G_CLIPBOARD_TYPE,
		NodeType::NODE_G_AXIS_TYPE,
		NodeType::NODE_G_BOUNDINGBOX_TYPE
	};
	m_imageData = vtkSmartPointer<vtkImageData>::New();
	m_compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	m_color = vtkSmartPointer<vtkColorTransferFunction>::New();
	m_volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	m_volume = vtkSmartPointer<vtkVolume>::New();
}

GVolumeRenderNode::~GVolumeRenderNode()
{
}

bool GVolumeRenderNode::setParameter(std::shared_ptr<SuperNodeStruct> volumeRender)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if (!m_project.lock())
		return false;
	std::shared_ptr<G_VolumeRender> _volumeRender = std::dynamic_pointer_cast<G_VolumeRender>(volumeRender);

	SurperNode::setParameter(volumeRender);
	m_volumeRender= *_volumeRender;
	m_volumeRender.setProjectId(m_project.lock()->getProjectId());
	removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return true;
}

bool GVolumeRenderNode::getParameter(G_VolumeRender & volumeRender)
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return false;
	//memcpy_s(&volumeRender, sizeof(G_VolumeRender), &m_volumeRender, sizeof(G_VolumeRender));
	volumeRender = m_volumeRender;
	return true;
}

	// yzw 0629
std::shared_ptr<SuperNodeStruct> GVolumeRenderNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_VolumeRender> ptr = std::make_shared<G_VolumeRender>();
	//memcpy_s(ptr.get(), sizeof(G_VolumeRender), &m_volumeRender, sizeof(G_VolumeRender));
	*ptr = m_volumeRender;
	SurperNode::getParameter(ptr);
	return ptr;
}


std::shared_ptr<TiXmlNode> GVolumeRenderNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	//todo:
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_volumeRender.toXml(pParameters);
	pElemNode->InsertEndChild(*pParameters);

	return pElemNode;
}

bool GVolumeRenderNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	m_volumeRender.m_name = m_nodeName;
	m_volumeRender.m_nodeIndex = m_nodeIndex;

	TiXmlElement* pParaters = ((TiXmlElement*)node)->FirstChildElement("Parameters");
	if (!pParaters)
		return SetProjectError
		m_volumeRender.fromXml(pParaters);

	return true;
}

void GVolumeRenderNode::getDefault(SuperNodeStruct* st)
{
	G_VolumeRender* pst = (G_VolumeRender*)st;	//	默认参数
	G_VolumeRender m_volumeRender;
	m_volumeRender.m_method = RenderMethod::Alpha;
	m_volumeRender.renderMethod.m_insertMethod = Child_InsertMethod_VRender::THREE_LINEAR_METHOD;
	m_volumeRender.renderMethod.m_opacity = 0.9;
	m_volumeRender.renderMethod.m_renderMode = RenderMode::GPU; //默认值Default
	m_volumeRender.renderMethod.m_sampleDistance = 0.5; //默认值设为1.0
	m_volumeRender.renderMethod.m_averageIPScalarRange[0] = 100;
	m_volumeRender.renderMethod.m_averageIPScalarRange[1] = 200;
	*pst = m_volumeRender;
	pst->renderMethod.m_colorBar.getDefault();
	pst->renderMethod.m_colorBar.m_title.m_title = "volume";
}

bool GVolumeRenderNode::run()
{
	if (m_excuted)
		return true;
	double begin_row = *(m_dataRange);
	double begin_col = *(m_dataRange + 1);//  -0.9387755102;//此类数值来源来自voxler手册
	double begin_high = *(m_dataRange + 2);
	double end_row = *(m_dataRange + 3);
	double end_col = *(m_dataRange + 4);// +0.9387755102;
	double end_high = *(m_dataRange + 5);

	double cnt_x = (end_row - begin_row) / (double)(m_resolution[0] -1);
	double cnt_y = (end_col - begin_col) / (double)(m_resolution[1] -1);
	double cnt_z = (end_high - begin_high) / (double)(m_resolution[2]-1);

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
				//*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * nx * ny + j * ny + k].scalar;
				*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * m_resolution[1] * m_resolution[2] + j * m_resolution[2] + k].scalar;
			}
		}
	}

		double dd = m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0];
		double db = m_imageData->GetScalarRange()[0];
		{
	//m_compositeOpacity->AddPoint(0 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 0.33333333333333);
	//m_compositeOpacity->AddPoint(0.020408163265306 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 0.33333333333333);
	//m_compositeOpacity->AddPoint(0.046382189239332 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 0.86666666666667);
	//m_compositeOpacity->AddPoint(0.12987012987013 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 1);
	//m_compositeOpacity->AddPoint(0.26901669758813 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 1);
	//m_compositeOpacity->AddPoint(0.40074211502783 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 0.088888888888889);
	//m_compositeOpacity->AddPoint(1 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 0.088888888888889);
		double x[] = {0, 0.020408163265306, 0.046382189239332, 0.12987012987013, 0.26901669758813, 0.40074211502783, 1};
		double y[] = {0.33333333333333, 0.33333333333333, 0.86666666666667, 1, 1, 0.088888888888889, 0.088888888888889};
		int ic;
		for (ic = 0; ic < sizeof(x) / sizeof(double); ic++)
			m_compositeOpacity->AddPoint(x[ic] * dd + db, y[ic]);
	}
	int cr = setLut(m_volumeRender.renderMethod.m_gradientMap, m_imageData->GetScalarRange(), m_volumeRender.renderMethod.m_colorBar.m_label);
	m_scalarBar->SetLookupTable(m_lut);
	setColorBar(m_volumeRender.renderMethod.m_colorBar);
	{
		//m_color->AddRGBPoint(0 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 153 / 255.0, 102 / 255.0, 252 / 255.0);
		//m_color->AddRGBPoint(0.038961038961039 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 58 / 255.0, 39 / 255.0, 255 / 255.0);
		//m_color->AddRGBPoint(0.061224489795918 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 0 / 255.0, 255 / 255.0, 0 / 255.0);
		//m_color->AddRGBPoint(0.14100185528757 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 255 / 255.0, 255 / 255.0, 0 / 255.0);
		//m_color->AddRGBPoint(0.20408163265306 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 255 / 255.0, 220 / 255.0, 0 / 255.0);
		//m_color->AddRGBPoint(0.31354359925788 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 255 / 255.0, 102 / 255.0, 0 / 255.0);
		//m_color->AddRGBPoint(1 * (m_imageData->GetScalarRange()[1] - m_imageData->GetScalarRange()[0]) + m_imageData->GetScalarRange()[0], 255 / 255.0, 0 / 255.0, 0 / 255.0);
	}
	{
		int ic;
		for (ic = 0; ic < LT_COLOR_NUM; ic++){
			double rgb[4];
			double xic = ic * dd/ LT_COLOR_NUM + db;		//  以索引算标量值
			m_lut->GetTableValue(ic,rgb);		// 以索引获取颜色
			m_color->AddRGBPoint(xic , rgb[0], rgb[1], rgb[2]);	// 标量-颜色
		}
	}
	m_volumeProperty->SetInterpolationTypeToLinear();
	m_volumeProperty->ShadeOn();
	m_volumeProperty->SetAmbient(0.4);
	m_volumeProperty->SetDiffuse(0.6);
	m_volumeProperty->SetSpecular(0.2);
	m_volumeProperty->SetScalarOpacity(m_compositeOpacity);
	m_volumeProperty->SetColor(m_color);

	m_mapper->SetInputData(m_imageData);
	float opacity;
	opacity= m_volumeRender.renderMethod.m_opacity;
		m_mapper->SetBlendMode(m_volumeRender.m_method);
	if (m_volumeRender.m_method == RenderMethod::Average_strength)
	{
		m_mapper->SetAverageIPScalarRange(m_volumeRender.renderMethod.m_averageIPScalarRange);
	}
		m_mapper->SetRequestedRenderMode(m_volumeRender.renderMethod.m_renderMode);

	if (m_volumeRender.renderMethod.m_insertMethod == Child_InsertMethod_VRender::NEARESTNEIGHBOR)
	{
		m_mapper->SetInterpolationModeToNearestNeighbor();
	}
	if (m_volumeRender.renderMethod.m_insertMethod == Child_InsertMethod_VRender::LINEAR_METHOD)
	{
		m_mapper->SetInterpolationModeToLinear();
	}
	if (m_volumeRender.renderMethod.m_insertMethod == Child_InsertMethod_VRender::THREE_LINEAR_METHOD)
	{
		m_mapper->SetInterpolationModeToCubic();
	}

	//m_mapper->SetInteractiveAdjustSampleDistances(1);
	m_mapper->SetAutoAdjustSampleDistances(0);
	m_mapper->SetSampleDistance(m_volumeRender.renderMethod.m_sampleDistance);

	m_volume->SetMapper(m_mapper);
	m_volume->SetProperty(m_volumeProperty);

	vtkPiecewiseFunction* colorfunc = m_volume->GetProperty()->GetScalarOpacity();
	for (size_t i = 0; i < colorfunc->GetSize(); i++)
	{
		double val[4];
		colorfunc->GetNodeValue(i, val);
		val[1] *= opacity;
		colorfunc->SetNodeValue(i, val);
	}
	if(m_checked){
		vtkSmartPointer<vtkRenderer> rd= m_project.lock()->getRender();
		rd->AddVolume(m_volume);
		if (m_volumeRender.renderMethod.m_colorBar.m_displayOn)
			rd->AddActor2D(m_scalarBar);
	}
	m_excuted = true;
	//makeNextNodeRun();
	return true;
}

bool GVolumeRenderNode::addToRender()	// yzw 0608
{
	if (m_excuted){
		m_project.lock()->getRender()->AddVolume(m_volume);
		if (m_volumeRender.renderMethod.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
		m_project.lock()->getRender() -> Render();
	}
	return m_excuted;
}

// yzw 0526
bool GVolumeRenderNode::removeFromRender()
{
	if(m_excuted){
		m_project.lock()->getRender()->RemoveVolume(m_volume);
		m_project.lock()->getRender()->RemoveActor(m_scalarBar);
	}
	return m_excuted;
}

void GVolumeRenderNode::makeNextNodeRun()
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

void GVolumeRenderNode::getBound(double bounds[6])
{
	
}

void GVolumeRenderNode::setBound(double * bounds)
{
}


void GVolumeRenderNode::updateDisplay(std::shared_ptr<SuperNodeStruct> volumeRender)
{
	if (!volumeRender)
		return;
	std::shared_ptr<G_VolumeRender> _volumeRender = std::dynamic_pointer_cast<G_VolumeRender>(volumeRender);
	if (!_volumeRender)
		return;
	switch (_volumeRender->m_changetype)
	{
	case 1:
	{

	}
	break;
	case 2:
	{

	}
	break;
	case 3:
	{
		m_volumeRender.m_method = _volumeRender->m_method;
		{
			m_mapper->SetBlendMode(vtkVolumeMapper::AVERAGE_INTENSITY_BLEND);
			if (m_volumeRender.m_method == RenderMethod::Average_strength)
				m_mapper->SetAverageIPScalarRange(m_volumeRender.renderMethod.m_averageIPScalarRange);
			m_mapper->SetRequestedRenderMode(m_volumeRender.renderMethod.m_renderMode);
		}

		m_mapper->Update();
	}
	break;
	case 4:
	{

	}
	break;
	case 5:
	{
		updateDisplayColorBar(m_volumeRender.renderMethod.m_colorBar, _volumeRender->renderMethod.m_colorBar);
	}
	break;
	default:
		break;
	}
	m_project.lock()->getRender()->ResetCamera();
	m_project.lock()->getRender()->Render();
}

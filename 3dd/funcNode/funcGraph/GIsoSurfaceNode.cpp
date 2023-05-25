#include "GIsoSurfaceNode.h"
#include "XmlProject.h"
#include "isosurface_property_widget.h"

GIsoSurfaceNode::GIsoSurfaceNode(std::shared_ptr< XmlProject> project) :GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_ISOSURFACE_TYPE;
	m_nodeCanAppendNodeTypeList = {//NodeType::NODE_G_CLIPBOARD_TYPE,
		NodeType::NODE_G_AXIS_TYPE,
		NodeType::NODE_G_BOUNDINGBOX_TYPE,
	};

	m_imageData = vtkSmartPointer<vtkImageData>::New();
	m_clipper = vtkSmartPointer<vtkClipVolume>::New();
	m_surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	m_triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
	m_mass = vtkSmartPointer<vtkMassProperties>::New();
	m_marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
	m_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_prop = vtkSmartPointer<vtkProperty>::New();
	m_isosurfaceActor = vtkSmartPointer<vtkActor>::New();
}

GIsoSurfaceNode::~GIsoSurfaceNode()
{
}

bool GIsoSurfaceNode::setParameter(std::shared_ptr<SuperNodeStruct> data)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if(!m_project.lock())
		return false;
	std::shared_ptr<G_IsoSurface> _data = std::dynamic_pointer_cast<G_IsoSurface>(data);
	if (!_data)
		return false;
	//if (m_excuted)
	{
	//	updateDisplay(isoSurface);
	}
	//else
	{
		SurperNode::setParameter(data);
		_data->setProjectId(m_isoSurface.getProjectId());
		//memcpy_s(&m_isoSurface, sizeof(G_IsoSurface), _isosurface.get(), sizeof(G_IsoSurface));
		m_isoSurface = *_data;
		removeFromRender();
		if (m_excuted) {
			m_excuted = false;
			return run();
		}
	}
	
	return true;
}

bool GIsoSurfaceNode::getParameter(G_IsoSurface &data)
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return false;
	//memcpy_s(&isoSurface, sizeof(G_IsoSurface), &m_isoSurface, sizeof(G_IsoSurface));
	m_isoSurface = data;
	return true;
}

std::shared_ptr<SuperNodeStruct> GIsoSurfaceNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<G_IsoSurface> data = std::make_shared<G_IsoSurface>();
	//memcpy_s(mesh.get(), sizeof(C_Mesh), &m_mesh, sizeof(C_Mesh));
	*data = m_isoSurface;
	SurperNode::getParameter(data);
	return data;
}

bool GIsoSurfaceNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return SetProjectError;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError;
	TiXmlElement* pElement = (TiXmlElement*)(node);
	TiXmlElement* pParameter = (pElement->FirstChildElement("Parameters"));
	if (!pParameter)
		return SetProjectError;
	m_isoSurface.fromXml(pParameter);

	return true;
}

void GIsoSurfaceNode::getDefault(SuperNodeStruct* st)		// 取默认参数
{
	G_IsoSurface* pst = (G_IsoSurface*)st;	//	默认参数
	pst->m_render.m_colorMethod = (ColorMethod)0;
	pst->m_render.m_gradientMap = 0;
	pst->m_materials.m_colorBar.getDefault();
	pst->m_materials.m_colorBar.m_title.m_title = "isoSurface";
	pst->m_CriticalValue = 100.7199444;
	pst->m_isCalculateVolume = true;
	pst->m_materials.m_glossiness = 1;
	pst->m_materials.m_HighLight_strength = 1;
	pst->m_materials.m_opacity = 1;
	pst->m_materials.m_HighLight_rgb = { 55, 255, 55};
}

bool GIsoSurfaceNode::run()
{
	if (m_excuted)
		return true;
	if (!m_project.lock())
		return false;

	double begin_row = *(m_dataRange);
	double begin_col = *(m_dataRange + 1);// = -0.9387755102;
	double begin_high = *(m_dataRange + 2);
	double end_row = *(m_dataRange + 3);
	double end_col = *(m_dataRange + 4);// = 0.9387755102;
	double end_high = *(m_dataRange + 5);

	double cnt_x = (end_row - begin_row) / (double)(m_resolution[0]-1);
	double cnt_y = (end_col - begin_col) / (double)(m_resolution[1]-1);
	double cnt_z = (end_high - begin_high) / (double)(m_resolution[2]-1);
	m_imageData->SetDimensions(m_resolution[0] , m_resolution[1] , m_resolution[2] );
	m_imageData->SetSpacing(cnt_x, cnt_y, cnt_z);
	m_imageData->SetOrigin(begin_row, begin_col, begin_high);
	m_imageData->AllocateScalars(VTK_DOUBLE, 1);

	for (int i = 0; i < m_resolution[0]; i++) {
		//double i_f = cnt_x * i + begin_row;
		for (int j = 0; j < m_resolution[1]; j++) {
			//double j_f = cnt_y * j + begin_col;
			for (int k = 0; k < m_resolution[2]; k++) {
				//double k_f = cnt_z * k + begin_high;
				*(double*)(m_imageData->GetScalarPointer(i, j, k)) = m_data[i * m_resolution[1] *  m_resolution[2] + j *  m_resolution[2] + k].scalar;
			}
		}
	}
	if (m_isoSurface.m_isCalculateVolume)
	{
		m_clipper->SetInputData(m_imageData);
		m_clipper->SetValue(m_isoSurface.m_CriticalValue);
		m_clipper->SetInsideOut(true);
		m_clipper->Update();

		m_surfaceFilter->SetInputConnection(m_clipper->GetOutputPort());
		m_surfaceFilter->Update();

		m_triangleFilter->SetInputConnection(m_surfaceFilter->GetOutputPort());
		m_triangleFilter->Update();

		m_mass->SetInputConnection(m_triangleFilter->GetOutputPort());
		m_mass->Update();

		m_isoSurface.m_volumeSmallerThanCriticalValue = m_mass->GetVolume();
		m_isoSurface.m_volumeBigerThanCriticalValue = m_imageData->GetSpacing()[0] * m_imageData->GetSpacing()[1] * m_imageData->GetSpacing()[2] * (m_imageData->GetDimensions()[0] - 1) * (m_imageData->GetDimensions()[1] - 1) * (m_imageData->GetDimensions()[2] - 1) - m_isoSurface.m_volumeSmallerThanCriticalValue;
		if (widget) {
			isosurface_property_widget* pWidget = (isosurface_property_widget*)widget;
			pWidget->displayVolume(m_isoSurface.m_volumeSmallerThanCriticalValue, m_isoSurface.m_volumeBigerThanCriticalValue);
		}
	}

	m_marchingCubes->SetInputData(m_imageData);
	m_marchingCubes->SetNumberOfContours(1);
	m_marchingCubes->SetValue(0, m_isoSurface.m_CriticalValue);
	m_marchingCubes->ComputeNormalsOn();
	m_marchingCubes->ComputeGradientsOn();
	m_marchingCubes->Update();

	m_lut = vtkSmartPointer<vtkLookupTable>::New();

	int cr= setLut(m_isoSurface.m_render.m_gradientMap, m_scalarRange, m_isoSurface.m_materials.m_colorBar.m_label);
	m_scalarBar->SetLookupTable(m_lut);
	setColorBar(m_isoSurface.m_materials.m_colorBar);

	m_mapper->SetInputConnection(m_marchingCubes->GetOutputPort());
	m_mapper->ScalarVisibilityOff();
	{
	//m_prop->SetColor(m_isoSurface.m_materials.m_HighLight_rgb.m_r / 255.0,
	//	m_isoSurface.m_materials.m_HighLight_rgb.m_g / 255.0, m_isoSurface.m_materials.m_HighLight_rgb.m_b / 255.0);
		double rgba[4];
		//  以标量值算索引
		double ic = (m_isoSurface.m_CriticalValue- m_scalarRange[0])*  LT_COLOR_NUM/ m_scalarRange[1]  ;
		m_lut->GetTableValue(ic, rgba);		// 以索引获取颜色
		m_prop->SetColor(rgba);
	}
	m_prop->SetShading(true);
	m_prop->SetOpacity(m_isoSurface.m_materials.m_opacity);
	m_prop->SetSpecular(m_isoSurface.m_materials.m_glossiness);
	m_prop->SetSpecularPower(m_isoSurface.m_materials.m_HighLight_strength * 128.0);
	m_prop->SetSpecularColor(
		m_isoSurface.m_materials.m_HighLight_rgb.m_r / 255.0,
		m_isoSurface.m_materials.m_HighLight_rgb.m_g / 255.0, 
		m_isoSurface.m_materials.m_HighLight_rgb.m_b / 255.0);
	m_prop->Modified();

	m_isosurfaceActor->SetMapper(m_mapper);
	m_isosurfaceActor->SetProperty(m_prop);

	if(m_checked){
		m_project.lock()->getRender()->AddActor(m_isosurfaceActor);
		if (m_isoSurface.m_materials.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
	}
	m_excuted = true;
	//makeNextNodeRun();
	return false;
}

bool GIsoSurfaceNode::addToRender()
{
	if (m_excuted){
		m_project.lock()->getRender()->AddActor(m_isosurfaceActor);
		if (m_isoSurface.m_materials.m_colorBar.m_displayOn)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
	}
	return m_excuted;
}

bool GIsoSurfaceNode::removeFromRender()
{
	if (m_excuted){
		m_project.lock()->getRender()->RemoveActor(m_isosurfaceActor);
		m_project.lock()->getRender()->RemoveActor(m_scalarBar);
	}
	return m_excuted;
}

void GIsoSurfaceNode::makeNextNodeRun()
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

void GIsoSurfaceNode::getBound(double bounds[6])
{
}

void GIsoSurfaceNode::setBound(double * bounds)
{
}

void GIsoSurfaceNode::updateDisplay(std::shared_ptr<SuperNodeStruct> isoSurface)
{
	std::shared_ptr<G_IsoSurface> _surface = std::dynamic_pointer_cast<G_IsoSurface>(isoSurface);
	if (!_surface)
		return;
	switch (_surface->m_changetype)
	{
	case 1:
	{
		m_isoSurface.m_CriticalValue = _surface->m_CriticalValue;
		m_marchingCubes->SetValue(0, m_isoSurface.m_CriticalValue);
		m_marchingCubes->Update();
		m_mapper->Update();
	}
	break;
	case 2:
	{

	}
	break;
	case 3:
	{
		memcpy_s(&m_isoSurface.m_materials, sizeof(Child_Materials),
			&_surface->m_materials, sizeof(Child_Materials));
		m_prop->SetOpacity(m_isoSurface.m_materials.m_opacity);
		m_prop->SetSpecular(m_isoSurface.m_materials.m_HighLight_strength);
		m_prop->SetSpecularPower(m_isoSurface.m_materials.m_glossiness * 128.0);
		m_prop->SetSpecularColor(m_isoSurface.m_materials.m_HighLight_rgb.m_r / 255.0,
			m_isoSurface.m_materials.m_HighLight_rgb.m_g / 255.0, m_isoSurface.m_materials.m_HighLight_rgb.m_b / 255.0);
		m_prop->Modified();
		m_mapper->Update();
	}
	break;
	case 5:
	{
		updateDisplayColorBar(m_isoSurface.m_materials.m_colorBar, _surface->m_materials.m_colorBar);
	}
	default:
		break;
	}
	m_project.lock()->getRender()->Render();
}


std::shared_ptr<TiXmlNode> GIsoSurfaceNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameter = std::make_shared<TiXmlElement>("Parameters");
	m_isoSurface.toXml(pParameter);


	pElemNode->InsertEndChild(*pParameter);

	return pElemNode;
}

void GIsoSurfaceNode::setInputData(std::vector<pointFinal> &data)
{
	m_excuted = false;
	m_data = data;
	m_scalarRange[0] = m_scalarRange[1] = 0;
	pointFinalGetRange(m_data, m_scalarRange);
	if (widget) {
		isosurface_property_widget* pWidget = (isosurface_property_widget*)widget;
		pWidget->setScalarRange(m_scalarRange);
	}
}

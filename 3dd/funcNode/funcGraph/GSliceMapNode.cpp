#include "GSliceMapNode.h"
#include "XmlProject.h"
#include "slice_property_widget.h"

void Cal_Normal_3D(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3, Vec3d &vn);
GSliceMapNode::GSliceMapNode(std::shared_ptr< XmlProject> project) :GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_SLICEMAP_TYPE;
	m_nodeCanAppendNodeTypeList = {
		NodeType::NODE_G_AXIS_TYPE,
		NodeType::NODE_G_BOUNDINGBOX_TYPE,
		//NodeType::NODE_G_CLIPBOARD_TYPE
	};
	m_imageData = vtkSmartPointer<vtkImageData>::New();
	m_centerPlane = vtkSmartPointer<vtkPlane>::New();
	m_cutter = vtkSmartPointer<vtkCutter>::New();
	m_sliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_actor = vtkSmartPointer<vtkActor>::New();
	originalPolyData = vtkSmartPointer<vtkPolyData>::New();
	delaunayMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	delaunayActor = vtkSmartPointer<vtkActor>::New();
}

GSliceMapNode::~GSliceMapNode()
{
}

void GSliceMapNode::setCutPoint(double* p)
{
	m_sliceMap.m_cutPlane.m_normalInfo.m_x = p[0];
	m_sliceMap.m_cutPlane.m_normalInfo.m_y = p[1];
	m_sliceMap.m_cutPlane.m_normalInfo.m_z = p[2];
	if (tri_point_n){
	if (widget) {
		slice_property_widget* pw = (slice_property_widget*)widget;
		double pos[3] = { p[0] - m_dataRange[0], p[1] - m_dataRange[1],p[2] - m_dataRange[2] };//相对左下角偏移量
		pw->setCutPoint(pos);
	}
		tri_point[tri_point_n - 1]->SetPosition(p);
	}
}

void GSliceMapNode::setNewPos(double* pos)
{
	setCutPoint(pos);
}

void GSliceMapNode::setDataRange(double* dataRange)
{
	m_excuted = false;
	memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	float m_centerRange[3];//x/y/z
	m_centerRange[0] = *(m_dataRange + 3) - *(m_dataRange);
	m_centerRange[1] = *(m_dataRange + 4) + 0.9387755102 - (*(m_dataRange + 1) - 0.9387755102);
	m_centerRange[2] = *(m_dataRange + 5) - *(m_dataRange + 2);
	tri_point[0]->SetPosition(m_dataRange[0] + m_centerRange[0] / 2, m_dataRange[1], m_dataRange[2]);
	tri_point[1]->SetPosition(m_dataRange[0], m_dataRange[1] + m_centerRange[1] / 2, m_dataRange[2]);
	tri_point[2]->SetPosition(m_dataRange[0], m_dataRange[1], m_dataRange[2] + m_centerRange[2] / 2);
	if (widget) {
		slice_property_widget*pw = (slice_property_widget*)widget;
		pw->setCutPlaneRange(m_centerRange);
	}
}

void GSliceMapNode::setOriginData(POINT_FINAL_VEC *data) 
{
	origin_data = data;
	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	for (auto var : *data)
	{
		auto	idtype = points->InsertNextPoint(var.x, var.y, var.z);
		//cells->InsertNextCell(1, &idtype);
	}

	originalPolyData->SetPoints(points);
	originalPolyData->SetVerts(cells);
	delaunay3D = vtkSmartPointer<vtkDelaunay3D>::New();
	delaunay3D->SetInputData(originalPolyData);// fillHolesFilter->GetOutput()
	delaunay3D->Update();

	vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
		vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surfaceFilter->SetInputConnection(delaunay3D->GetOutputPort());
	surfaceFilter->Update();
	//线性网格细分滤波器
	vtkSmartPointer<vtkLinearSubdivisionFilter> linear =
		vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
	linear->SetInputData(surfaceFilter->GetOutput());
	linear->SetNumberOfSubdivisions(3);
	linear->Update();

	//Loop网格细分滤波器
	//vtkSmartPointer<vtkLoopSubdivisionFilter> loop =
		subDiv=vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	subDiv->SetInputConnection(linear->GetOutputPort());//
	subDiv->SetNumberOfSubdivisions(3);
	subDiv->Update();
	bfprobe_updated = false;
}

void GSliceMapNode::setInputData(std::vector<pointFinal> &data)
{
	m_excuted = false;
	m_data = data;
	auto points = vtkSmartPointer<vtkPoints>::New();
	auto cells = vtkSmartPointer<vtkCellArray>::New();
	for (int i = 0; i < m_resolution[0]; i++) {
		//double i_f = cnt_x * i + begin_row;
		for (int j = 0; j < m_resolution[1]; j++) {
			//double j_f = cnt_y * j + begin_col;
			for (int k = 0; k < m_resolution[2]; k++) {
				//double k_f = cnt_z * k + begin_high;
				pointFinal pf = m_data[i * m_resolution[1] * m_resolution[2] + j * m_resolution[2] + k];
				if (i == 0 || i == m_resolution[0] - 1
					|| j == 0 || j == m_resolution[1] - 1
					|| k == 0 || k == m_resolution[2] - 1)
				{
					auto	idtype = points->InsertNextPoint(pf.x, pf.y, pf.z);
					cells->InsertNextCell(1, &idtype);
				}
			}
		}
	}

	inputPolyData->SetPoints(points);
	inputPolyData->SetVerts(cells);

	probe->SetInputData(point);
	probe->SetSourceData(inputPolyData);

	sphereSource->Update();

	glyph->SetInputConnection(probe->GetOutputPort());
	glyph->SetSourceConnection(sphereSource->GetOutputPort());//cone 
	glyph->SetVectorModeToUseVector();
	glyph->SetScaleModeToDataScalingOff();
	glyph->SetScaleFactor(0.4);

	glyphMapper->SetInputConnection(glyph->GetOutputPort());

	glyphActor->SetMapper(glyphMapper);
	glyphActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	glyphActor->VisibilityOff();

	sphere->SetRadius(0.4);
	sphere->Update();
	vtkPolyData* polydata1 = sphere->GetOutput();

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputData(polydata1);
	tri_point[0]->SetMapper(mapper); tri_point[0]->VisibilityOff();
	tri_point[1]->SetMapper(mapper); tri_point[1]->VisibilityOff();
	tri_point[2]->SetMapper(mapper); tri_point[2]->VisibilityOff();

	// The SetInteractor method is how 3D widgets are associated with the render
	// window interactor. Internally, SetInteractor sets up a bunch of callbacks
	// using the Command/Observer mechanism (AddObserver()).
	//myCallback->PolyData = point;
	//myCallback->CursorActor = glyphActor;
	//myCallback->PositionActor = textActor;

}

bool GSliceMapNode::setParameter(std::shared_ptr<SuperNodeStruct> node)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if(!m_project.lock())
		return false;
	std::shared_ptr<G_SliceMap> _sliceMap = std::dynamic_pointer_cast<G_SliceMap>(node);
	int mode = _sliceMap->m_cutPlane.m_mode;
	if (node->m_changetype == G_Slice_MN_CT + 40) {
		node->m_changetype = 0;
		//m_excuted = false;
		//return run();
	}
	else if (node->m_changetype == G_Slice_MN_CT + 20) {
		node->m_changetype = 0;
		if (mode == 0) {//轴垂直面
			glyphActor->VisibilityOff();
			dragger_en = false;
			draggerOn(false); 
			glyphActor->SetVisibility(false);
			tri_point[0]->VisibilityOff();
			tri_point[1]->VisibilityOff();
			tri_point[2]->VisibilityOff();
		}
			//return false;
		else if (mode == 1) {//轴平行面
			node->m_changetype = 0;
			glyphActor->VisibilityOn();
	#if TRI_POINT
			tri_point[0]->VisibilityOn();
			tri_point[1]->VisibilityOn();
			tri_point[2]->VisibilityOff();
			if (tri_point_n)
				tri_point[tri_point_n - 1]->GetProperty()->SetColor(1.0, 0.0, 0.0);
			//#else
			dragger_en = true;
			draggerOn(); 
	#endif
			//return false;
		}
	//else if (node->m_changetype == G_Slice_MN_CT + 21) 
		else //if (_sliceMap->m_cutPlane.m_mode == 2) //三点定面
		{
				node->m_changetype = 0;
			glyphActor->VisibilityOn();
	#if TRI_POINT
			tri_point[0]->VisibilityOn();
			tri_point[1]->VisibilityOn();
			tri_point[2]->VisibilityOn();
			if (tri_point_n)
				tri_point[tri_point_n - 1]->GetProperty()->SetColor(1.0, 0.0, 0.0);
			//#else
			dragger_en = true;
			draggerOn(); 
	#endif
			//return false;
		}
	}
	else if (node->m_changetype == G_Slice_MN_CT + 22) {
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		tri_point_n = 0;
		//return false;
	}
	else if (node->m_changetype == G_Slice_MN_CT + 23) {
		tri_point[0]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		pointWidget->SetPosition(tri_point[0]->GetPosition());
		tri_point_n = 1;
		//return false;
	}
	else if (node->m_changetype == G_Slice_MN_CT + 24) {
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		tri_point[1]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		pointWidget->SetPosition(tri_point[1]->GetPosition());
		tri_point_n = 2;
		//return false;
	}
	else if (node->m_changetype == G_Slice_MN_CT + 25) {
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0, 1.0);
		tri_point[2]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		pointWidget->SetPosition(tri_point[2]->GetPosition());
		tri_point_n = 3;
		//return false;
	}
	else{
		;
	}
	{
		SurperNode::setParameter(node);
		_sliceMap->setProjectId(m_sliceMap.getProjectId());
		m_sliceMap = *_sliceMap;
		removeFromRender();
		if (m_excuted)
		{
			m_excuted = false;
			return run();
		}
	}
	return true;
}

bool GSliceMapNode::getParameter(G_SliceMap & sliceMap)
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return false;
	sliceMap = m_sliceMap;
	return true;
}

std::shared_ptr<SuperNodeStruct> GSliceMapNode::getParameter()
{
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return std::shared_ptr<SuperNodeStruct>();

	std::shared_ptr<G_SliceMap> sliceMap = std::make_shared<G_SliceMap>();
	*sliceMap = m_sliceMap;
	SurperNode::getParameter(sliceMap);
	return sliceMap;
}

bool GSliceMapNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	m_sliceMap.m_name = m_nodeName;
	m_sliceMap.m_nodeIndex = m_nodeIndex;

	TiXmlElement* pElement = (TiXmlElement*)(node);
	TiXmlElement* pParameters = (pElement->FirstChildElement("Parameters"));
	if (!pParameters)
		return SetProjectError;
	m_sliceMap.fromXml(pParameters);

	return true;
}

void GSliceMapNode::getDefault(SuperNodeStruct* st) {
	G_SliceMap* pst = (G_SliceMap*)st;
	pst->m_lightOn = true; //光线参数未知
	pst->m_original_data_slice = false;
	pst->m_opacityValue = 1;
//	pst->m_colorMethod = 0;
	pst->m_cutPlane.getDefault();
	pst->m_colorBar.getDefault(); 
	pst->m_colorBar.m_title.m_title = "slice";
}

bool GSliceMapNode::run()
{
	if (m_excuted)
		return true;
	if(!m_project.lock())
		return false;

	double begin_row = *(m_dataRange);
	double begin_col = *(m_dataRange + 1);//  -0.9387755102;
	double begin_high = *(m_dataRange + 2);
	double end_row = *(m_dataRange + 3);
	double end_col = *(m_dataRange + 4);// +0.9387755102;
	double end_high = *(m_dataRange + 5);

	double cnt_x = (end_row - begin_row) / (double)(m_resolution[0]-1);
	double cnt_y = (end_col - begin_col) / (double)(m_resolution[1]-1);
	double cnt_z = (end_high - begin_high) / (double)(m_resolution[2]-1);

	m_imageData->SetDimensions(m_resolution[0] , m_resolution[1] , m_resolution[2] );
	m_imageData->SetSpacing(cnt_x, cnt_y, cnt_z);
	m_imageData->SetOrigin(begin_row, begin_col, begin_high);
	m_imageData->AllocateScalars(VTK_DOUBLE, 1);

	for (int i = 0; i < m_resolution[0]; i++)
	{
		for (int j = 0; j < m_resolution[1]; j++)
		{
			for (int k = 0; k < m_resolution[2]; k++)
			{
				*(double*)(m_imageData->GetScalarPointer(i, j, k)) =
					m_data[i * m_resolution[1]* m_resolution[2] + j * m_resolution[2]  + k].scalar;
			}
		}
	}

	double imagedataCenter[3] = { 
		m_imageData->GetOrigin()[0] + m_imageData->GetSpacing()[0] * (m_imageData->GetDimensions()[0] - 1) / 2.0, 
		m_imageData->GetOrigin()[1] + m_imageData->GetSpacing()[1] * (m_imageData->GetDimensions()[1] - 1) / 2.0 ,
		m_imageData->GetOrigin()[2] + m_imageData->GetSpacing()[2] * (m_imageData->GetDimensions()[2] - 1) / 2.0 };
	double vectorNorm = sqrt(
		m_sliceMap.m_cutPlane.m_normalInfo.m_x * m_sliceMap.m_cutPlane.m_normalInfo.m_x + 
		m_sliceMap.m_cutPlane.m_normalInfo.m_y * m_sliceMap.m_cutPlane.m_normalInfo.m_y + 
		m_sliceMap.m_cutPlane.m_normalInfo.m_z * m_sliceMap.m_cutPlane.m_normalInfo.m_z );
	double unitVector[3] = { 
		m_sliceMap.m_cutPlane.m_normalInfo.m_x,	// m_sliceMap.m_cutPlane.m_normalInfo.m_x / vectorNorm,
		m_sliceMap.m_cutPlane.m_normalInfo.m_y,	// m_sliceMap.m_cutPlane.m_normalInfo.m_y / vectorNorm,
		m_sliceMap.m_cutPlane.m_normalInfo.m_z,	// m_sliceMap.m_cutPlane.m_normalInfo.m_z / vectorNorm, 
	};
	double origin[3] = {
		imagedataCenter[0] + unitVector[0] * m_sliceMap.m_cutPlane.m_centerOffset,
		imagedataCenter[1] + unitVector[1] * m_sliceMap.m_cutPlane.m_centerOffset, 
		imagedataCenter[2] + unitVector[2] * m_sliceMap.m_cutPlane.m_centerOffset };
	double normal[3] = {
		m_sliceMap.m_cutPlane.m_normalInfo.m_x, 
		m_sliceMap.m_cutPlane.m_normalInfo.m_y, 
		m_sliceMap.m_cutPlane.m_normalInfo.m_z
	};
	if (m_sliceMap.m_cutPlane.m_mode == 1) {	// 轴平行面
		double point[3];
		tri_point[0]->GetPosition(point);
		point[m_sliceMap.m_cutPlane.m_axis] = 0;
			Vec3d tri_pointA(tri_point[0]->GetPosition());
			Vec3d tri_pointB(tri_point[1]->GetPosition());
			Vec3d tri_pointC(point);
			Vec3d nor;
			Cal_Normal_3D(tri_pointA, tri_pointB, tri_pointC, nor);
			m_cutterPlane->SetOrigin(tri_point[0]->GetPosition());
			m_cutterPlane->SetNormal(nor.x, nor.y, nor.z);
	}
	else if (m_sliceMap.m_cutPlane.m_mode == 2) {
		if (0 == tri_point_n) {	// 球切面
			double nor[3] = { // 相对左下角偏移量
				origin_cmd[0] - begin_row,
				origin_cmd[1] - begin_col,
				origin_cmd[2] - begin_high,
			};
			m_cutterPlane->SetOrigin(origin_cmd);
			m_cutterPlane->SetNormal(nor);
		}
		else {	//	三点定面
			Vec3d tri_pointA(tri_point[0]->GetPosition());
			Vec3d tri_pointB(tri_point[1]->GetPosition());
			Vec3d tri_pointC(tri_point[2]->GetPosition());
			Vec3d nor;
			Cal_Normal_3D(tri_pointA, tri_pointB, tri_pointC, nor);
			m_cutterPlane->SetOrigin(tri_point[0]->GetPosition());
			m_cutterPlane->SetNormal(nor.x, nor.y, nor.z);
		}
	}
	else {	// 轴垂直面
		m_cutterPlane->SetOrigin(origin);
		m_cutterPlane->SetNormal(normal);
	}
	m_cutter->SetCutFunction(m_cutterPlane);
	m_cutter->SetInputData(m_imageData);
	m_cutter->GenerateCutScalarsOff();
	m_cutter->Update();

	m_lut = vtkSmartPointer<vtkLookupTable>::New();
	int cr= setLut(m_sliceMap.m_gradientMap, m_imageData->GetScalarRange(), m_sliceMap.m_colorBar.m_label);

	m_scalarBar->SetLookupTable(m_lut);
	setColorBar(m_sliceMap.m_colorBar);

	m_sliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_sliceMapper->SetInputData(m_cutter->GetOutput());
	m_sliceMapper->SetLookupTable(m_lut);
	m_sliceMapper->SetScalarRange(m_imageData->GetScalarRange());
	m_sliceMapper->UseLookupTableScalarRangeOff();
#if 0
	if(!bfprobe_updated){
	vtkSmartPointer<vtkProbeFilter> fprobe = vtkSmartPointer<vtkProbeFilter>::New();
	fprobe->SetInputConnection(subDiv->GetOutputPort());//delaunay3D
	fprobe->SetSourceData(m_imageData);
	fprobe->Update();
	bfprobe_updated = true;
	delaunayMapper->SetInputConnection(fprobe->GetOutputPort());
	delaunayMapper->SetScalarRange(m_imageData->GetScalarRange());
	delaunayMapper->UseLookupTableScalarRangeOff();
	delaunayActor->SetMapper(delaunayMapper);
	}
	delaunayMapper->SetLookupTable(m_lut);
	delaunayActor->GetProperty()->SetOpacity(m_sliceMap.m_opacityValue);
	//delaunayActor->GetProperty()->SetColor(color->GetColor3d("banana").GetData());
	//delaunayActor->GetProperty()->EdgeVisibilityOn();
#endif
	m_actor->SetMapper(m_sliceMapper);
	m_actor->GetProperty()->SetOpacity(m_sliceMap.m_opacityValue);
	if(m_checked){
		vtkSmartPointer<vtkRenderer> ren = m_project.lock()->getRender();
		ren->AddActor(m_actor);
		if (m_sliceMap.m_colorBar.m_displayOn)
			ren->AddActor2D(m_scalarBar);
		//ren->AddActor(glyphActor);
		//if (m_sliceMap.m_original_data_slice)
		//ren->AddActor(delaunayActor);
		ren->AddActor(tri_point[0]);
		ren->AddActor(tri_point[1]);
		ren->AddActor(tri_point[2]);
		if (pointWidget)
			pointWidget->SetEnabled(dragger_en);	 
	}
	m_excuted = true;
	//makeNextNodeRun();
	return true;
}

bool GSliceMapNode::addToRender()
{
	if (m_excuted){
		vtkSmartPointer<vtkRenderer> ren = m_project.lock()->getRender();
		ren->AddActor(m_actor);
		if (m_sliceMap.m_colorBar.m_displayOn)
			ren->AddActor2D(m_scalarBar);
		//ren->AddActor(glyphActor);
		//if (m_sliceMap.m_original_data_slice)
		//ren->AddActor(delaunayActor);
		ren->AddActor(tri_point[0]);
		ren->AddActor(tri_point[1]);
		ren->AddActor(tri_point[2]);
		if (pointWidget)
		pointWidget->SetEnabled(dragger_en);	 
	}
	return m_excuted;
}

bool GSliceMapNode::removeFromRender()
{
	if (m_excuted){
		vtkSmartPointer<vtkRenderer> ren = m_project.lock()->getRender();
		ren->RemoveActor(m_actor);
		ren->RemoveActor(m_scalarBar);

		ren->RemoveActor(glyphActor);
		//ren->RemoveActor(delaunayActor);
		ren->RemoveActor(tri_point[0]);
		ren->RemoveActor(tri_point[1]);
		ren->RemoveActor(tri_point[2]);
		if (pointWidget)
		pointWidget->SetEnabled(false);
	}
	return m_excuted;
}

void GSliceMapNode::getBound(double bounds[6])
{
}

void GSliceMapNode::setBound(double * bounds)
{
}

void GSliceMapNode::updateDisplay(std::shared_ptr<SuperNodeStruct> sliceMap)
{
	std::shared_ptr<G_SliceMap> _sliceMap = std::dynamic_pointer_cast<G_SliceMap>(sliceMap);
	switch (_sliceMap->m_changetype)
	{
	case 0:
	{

	}
	break;
	case 1://不透明度
	{
		m_sliceMap.m_opacityValue = _sliceMap->m_opacityValue;
		m_actor->GetProperty()->SetOpacity(m_sliceMap.m_opacityValue);
		
	}
	break;
	case 2://着色方法
	{

	}
	break;
	case 3://色标刻度尺
	{

	}
	break;
	case 4://切割平面
	{
		//_sliceMap->m_cutPlane.m_centerOffset += 0.1;/*对中心偏移量进行测试*/
		memcpy_s(&m_sliceMap.m_cutPlane, sizeof(Child_CutingPlane), &_sliceMap->m_cutPlane, sizeof(Child_CutingPlane));
		double imagedataCenter[3] = { m_imageData->GetOrigin()[0] + m_imageData->GetSpacing()[0] * (m_imageData->GetDimensions()[0] - 1) / 2.0, m_imageData->GetOrigin()[1] + m_imageData->GetSpacing()[1] * (m_imageData->GetDimensions()[1] - 1) / 2.0 ,m_imageData->GetOrigin()[2] + m_imageData->GetSpacing()[2] * (m_imageData->GetDimensions()[2] - 1) / 2.0 };
		double vectorNorm = sqrt(m_sliceMap.m_cutPlane.m_normalInfo.m_x * m_sliceMap.m_cutPlane.m_normalInfo.m_x + m_sliceMap.m_cutPlane.m_normalInfo.m_y * m_sliceMap.m_cutPlane.m_normalInfo.m_y + m_sliceMap.m_cutPlane.m_normalInfo.m_z * m_sliceMap.m_cutPlane.m_normalInfo.m_z);
		double unitVector[3] = { m_sliceMap.m_cutPlane.m_normalInfo.m_x / vectorNorm, m_sliceMap.m_cutPlane.m_normalInfo.m_y / vectorNorm, m_sliceMap.m_cutPlane.m_normalInfo.m_z / vectorNorm, };
		double origin[3] = { imagedataCenter[0] + unitVector[0] * m_sliceMap.m_cutPlane.m_centerOffset, imagedataCenter[1] + unitVector[1] * m_sliceMap.m_cutPlane.m_centerOffset, imagedataCenter[2] + unitVector[2] * m_sliceMap.m_cutPlane.m_centerOffset };
		m_cutterPlane->SetOrigin(origin);
		m_cutterPlane->SetNormal(m_sliceMap.m_cutPlane.m_normalInfo.m_x, m_sliceMap.m_cutPlane.m_normalInfo.m_y, m_sliceMap.m_cutPlane.m_normalInfo.m_z);
		m_cutter->SetCutFunction(m_cutterPlane);
		m_cutter->SetInputData(m_imageData);
		m_cutter->GenerateCutScalarsOff();
		m_cutter->Update();
		m_sliceMapper->Update();
		
	}
	break;
	case 5:
	{
		updateDisplayColorBar(m_sliceMap.m_colorBar, _sliceMap->m_colorBar);
	}
		break;
	default:
		break;
	}
	m_project.lock()->getRender()->Render();
}

void GSliceMapNode::makeNextNodeRun()
{
	for (auto id : m_nodeNextNodeList)
	{
		std::shared_ptr<SurperNode> node = m_project.lock()->getSpecifiedNode(id);
		switch (node->getNodeType())
		{
		case NodeType::NODE_G_BOUNDINGBOX_TYPE:
		{
			std::shared_ptr<GBoundingBoxNode> boundBoxNode = std::dynamic_pointer_cast<GBoundingBoxNode>(node);
			if (!boundBoxNode)
				return;
			boundBoxNode->setDataRange(m_dataRange);
			boundBoxNode->run();
		}
		break;
		case NodeType::NODE_G_AXIS_TYPE:
		{
			std::shared_ptr<GAxis> axis = std::dynamic_pointer_cast<GAxis>(node);
			if (!axis)
				return;

		}
		break;
		default:
			break;
		}
	}
}

std::shared_ptr<TiXmlNode> GSliceMapNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameter = std::make_shared<TiXmlElement>("Parameters");
	m_sliceMap.toXml(pParameter);
	pElemNode->InsertEndChild(*pParameter);
	return pElemNode;
}

#include "GIsoLineNode.h"
#include "XmlProject.h"
#include "isoline_property_widget.h"
#include <vtkPointsProjectedHull.h>

#include "istyle.h"

/* https://www.cnblogs.com/charlee44/p/10744158.html
	空间已知三点的位置p1(x1,y1,z1),p2(x2,y2,z2),p3(x3,y3,z3),令它们逆时针在空间摆放。
	这样就可以得到平面的两个向量p1p2(x2-x1,y2-y1,z2-z1),p1p3(x3-x1,y3-y1,z3-z1)，而平面法线总是和这两个向量垂直。
	也就是说，p1p2与p1p3的向量积就是平面的法向量n。
	已知向量
	a=(a1,a2,a3) b=(b1,b2,b3)
	其向量积可表示为：
	a×b=(a2b3-a3b2，a3b1-a1b3，a1b2-a2b1)
*/
//	计算三点成面的法向量
void Cal_Normal_3D(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3, Vec3d &vn)
{
	//v1(n1,n2,n3);
	//平面方程: na * (x – n1) + nb * (y – n2) + nc * (z – n3) = 0 ;
	double na = (v2.y - v1.y)*(v3.z - v1.z) - (v2.z - v1.z)*(v3.y - v1.y);
	double nb = (v2.z - v1.z)*(v3.x - v1.x) - (v2.x - v1.x)*(v3.z - v1.z);
	double nc = (v2.x - v1.x)*(v3.y - v1.y) - (v2.y - v1.y)*(v3.x - v1.x);

	//平面法向量
	vn.Set(na, nb, nc);
}

class MouseInteractorHighLightActor;

GIsoLineNode::GIsoLineNode(std::shared_ptr< XmlProject> project) :GraphNode(project)
{
	m_nodeType = NodeType::NODE_G_ISOLINE_TYPE;
	m_nodeCanAppendNodeTypeList = {//NodeType::NODE_G_CLIPBOARD_TYPE,
		NodeType::NODE_G_AXIS_TYPE, 
		NodeType::NODE_G_BOUNDINGBOX_TYPE,
		NodeType::NODE_G_HIGHANDLOWDIAGRAM_TYPE
	};

	isoline_actor = vtkSmartPointer<vtkActor>::New();
	outline_actor = vtkSmartPointer<vtkActor>::New();

}

GIsoLineNode::~GIsoLineNode()
{
}

void GIsoLineNode::setCutPoint(double* p)
{
	m_isoLine.m_cutPlane.m_normalInfo.m_x = p[0];
	m_isoLine.m_cutPlane.m_normalInfo.m_y = p[1];
	m_isoLine.m_cutPlane.m_normalInfo.m_z = p[2];
	if (widget) {
		isoline_property_widget* pw = (isoline_property_widget*)widget;
		double pos[3] = {p[0]- m_dataRange [0], p[1]- m_dataRange [1],p[2]- m_dataRange [2]};//相对左下角偏移量
		pw->setCutPoint(pos);
	}
	if (tri_point_n)
		tri_point[tri_point_n-1]->SetPosition(p);
}

void GIsoLineNode::setNewPos(double* pos)
{
	setCutPoint(pos);
}

void GIsoLineNode::setDataRange(double* dataRange)
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
		isoline_property_widget* pw = (isoline_property_widget*)widget;
		pw->setCutPlaneRange(m_centerRange);
	}
}

void GIsoLineNode::setInputData(std::vector<pointFinal> &data)
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
				if(	i==0 || i== m_resolution[0]-1
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
	glyph->SetScaleFactor( 0.4);

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

bool GIsoLineNode::setParameter(std::shared_ptr<SuperNodeStruct> node) 
{
	if ( !m_project.lock())	//RETURN_ERROR == m_nodeIndex ||
		return false;
	std::shared_ptr<G_IsoLine> _data = std::dynamic_pointer_cast<G_IsoLine>(node);
	int mode = _data->m_cutPlane.m_mode;
	if(node->m_changetype == G_IsoLine_MN_CT+ 40){	// 拖动选点
		node->m_changetype = 0;
		//m_excuted = false;
		//return run();
	}
	else if(node->m_changetype == G_IsoLine_MN_CT+ 20){
		node->m_changetype = 0;
		if (mode == 0) {//轴垂直面
		glyphActor->VisibilityOff();
			dragger_en = false;
		draggerOn(false);  //pointWidget->Off();
		glyphActor->VisibilityOff();
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
	//else if(node->m_changetype == G_IsoLine_MN_CT+ 21){
		else  //三点定面
		{
		node->m_changetype = 0;
		glyphActor->VisibilityOn();
#if TRI_POINT
		tri_point[0]->VisibilityOn();
		tri_point[1]->VisibilityOn();
		tri_point[2]->VisibilityOn();
		if(tri_point_n)
			tri_point[tri_point_n-1]->GetProperty()->SetColor(1.0, 0.0, 0.0);
//#else
			dragger_en = true;
		draggerOn(); 
#endif
		//return false;
		}
	}
	else if(node->m_changetype == G_IsoLine_MN_CT+ 22){
		//vtkRenderWindowInteractor* iren = m_project.lock()->getWinInteractor();
		//MouseInteractorHighLightActor* style = dynamic_cast<MouseInteractorHighLightActor*>(iren->GetInteractorStyle());
		//style->active = nullptr; 
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point_n = 0;
		//return false;
	}
	else if(node->m_changetype == G_IsoLine_MN_CT+ 23){
		//vtkRenderWindowInteractor* iren = m_project.lock()->getWinInteractor();
		//MouseInteractorHighLightActor* style = dynamic_cast<MouseInteractorHighLightActor*>(iren->GetInteractorStyle());
		//style->active = tri_point[0];
		//style->pf_set_point = std::bind(&GIsoLineNode::setCutPoint, this, std::placeholders::_1);
		tri_point[0]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		pointWidget->SetPosition(tri_point[0]->GetPosition());
		tri_point_n = 1;
		//return false;
	}
	else if(node->m_changetype == G_IsoLine_MN_CT+ 24){
		//vtkRenderWindowInteractor* iren = m_project.lock()->getWinInteractor();
		//MouseInteractorHighLightActor* style = dynamic_cast<MouseInteractorHighLightActor*>(iren->GetInteractorStyle());
		//style->active = tri_point[1];
		//style->pf_set_point = std::bind(&GIsoLineNode::setCutPoint, this, std::placeholders::_1);
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[1]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		tri_point[2]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		pointWidget->SetPosition(tri_point[1]->GetPosition());
		tri_point_n = 2;
		//return false;
	}
	else if(node->m_changetype == G_IsoLine_MN_CT+ 25){
		//vtkRenderWindowInteractor* iren = m_project.lock()->getWinInteractor();
		//MouseInteractorHighLightActor* style = dynamic_cast<MouseInteractorHighLightActor*>(iren->GetInteractorStyle());
		//style->active = tri_point[2];
		//style->pf_set_point = std::bind(&GIsoLineNode::setCutPoint, this, std::placeholders::_1);
		tri_point[0]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[1]->GetProperty()->SetColor(0.0, 0.0,  1.0);
		tri_point[2]->GetProperty()->SetColor(1.0, 0.0, 0.0);
		pointWidget->SetPosition(tri_point[2]->GetPosition());
		tri_point_n = 3;
		//return false;
	}
	node->setProjectId(m_isoLine.getProjectId());
	SurperNode::setParameter(node);
	m_isoLine = *_data;
	removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return m_excuted;
}

bool GIsoLineNode::getParameter(G_IsoLine & isoLine)
{
	if ( !m_project.lock())	// RETURN_ERROR == m_nodeIndex ||
		return false;
	isoLine = m_isoLine;
	return true;
}

std::shared_ptr<SuperNodeStruct> GIsoLineNode::getParameter()
{
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return std::shared_ptr<SuperNodeStruct>();

	std::shared_ptr<G_IsoLine> data = std::make_shared<G_IsoLine>();
	*data = m_isoLine;
	SurperNode::getParameter(data);
	return data;
}

bool GIsoLineNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return SetProjectError;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return SetProjectError;
	TiXmlElement* pElement = (TiXmlElement * )(node);
	TiXmlElement* pParameter = (pElement->FirstChildElement("Parameters"));
	if (!pParameter)
	{
		return SetProjectError;
	}
	m_isoLine.fromXml(pParameter);
	return true;
}

std::shared_ptr<TiXmlNode> GIsoLineNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	
	std::shared_ptr<TiXmlElement> pParameter = std::make_shared<TiXmlElement>("Parameters");
	m_isoLine.toXml(pParameter);
	
	pElemNode->InsertEndChild(*pParameter);
	return pElemNode;
}

vtkSmartPointer<vtkProp> GIsoLineNode::getActor()
{
	return vtkSmartPointer<vtkProp>();
}

void GIsoLineNode::getDefault(SuperNodeStruct* st){
	G_IsoLine* pst = (G_IsoLine*)st;
	G_IsoLine  isoline;
	isoline.m_level = AUTO_CALCULATE;
	isoline.m_type.m_typeAuto.m_count = 5;

	isoline.m_type.m_typeInterval.m_minLevel = 0;
	isoline.m_type.m_typeInterval.m_maxLevel = 1;
	isoline.m_type.m_typeInterval.m_interval = 0.1;

	isoline.m_type.m_typeCount.m_minLevel = 0;
	isoline.m_type.m_typeCount.m_maxLevel = 1;
	isoline.m_type.m_typeCount.m_count = 10;

	isoline.m_type.m_typeExplicit.m_levelValue = 0.4;

	isoline.m_cutPlane.getDefault();
	isoline.m_render.m_colorBar.getDefault();
	isoline.m_render.m_colorBar.m_title.m_title = "isoline";
	isoline.m_render.m_lineWidth = 3;
	isoline.m_render.display_bound = true;
	isoline.m_render.m_boundWidth = 2;
	isoline.m_render.m_boundRgb = { 0, 0, 255 };
	*pst = isoline;
}

bool GIsoLineNode::run()
{
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
	return false;
	if (m_excuted)
		return true;
	double begin_row = *(m_dataRange);
	double begin_col = *(m_dataRange + 1);// = -0.9387755102;
	double begin_high = *(m_dataRange + 2);
	double end_row = *(m_dataRange + 3);
	double end_col = *(m_dataRange + 4);// = 0.9387755102;
	double end_high = *(m_dataRange + 5);

	double cnt_x = (end_row - begin_row) / (double)(m_resolution[0]-1);
	double cnt_y = (end_col - begin_col) / (double)(m_resolution[1]-1);
	double cnt_z = (end_high - begin_high) / (double)(m_resolution[2]-1);

	auto imageData = vtkSmartPointer<vtkImageData>::New();
	imageData->SetDimensions(m_resolution[0] , m_resolution[1] , m_resolution[2] );
	imageData->SetSpacing(cnt_x, cnt_y, cnt_z);
	imageData->SetOrigin(begin_row, begin_col, begin_high);
	imageData->AllocateScalars(VTK_DOUBLE, 1);

	for (int i = 0; i < m_resolution[0]; i++) {
		for (int j = 0; j < m_resolution[1]; j++) {
			for (int k = 0; k < m_resolution[2]; k++) {
				pointFinal pf = m_data[i * m_resolution[1] * m_resolution[2] + j * m_resolution[2] + k];
				*(double*)(imageData->GetScalarPointer(i, j, k)) = pf.scalar;
			}
		}
	}

//	G_IsoLine isoline;
#define isoline m_isoLine
	//这部分是给三维数据做切片 原理同slice 
	double imagedataCenter[3] = {
		imageData->GetOrigin()[0] + imageData->GetSpacing()[0] * (imageData->GetDimensions()[0] - 1) / 2.0, 
		imageData->GetOrigin()[1] + imageData->GetSpacing()[1] * (imageData->GetDimensions()[1] - 1) / 2.0 ,
		imageData->GetOrigin()[2] + imageData->GetSpacing()[2] * (imageData->GetDimensions()[2] - 1) / 2.0 };
	double vectorNorm = sqrt(
		isoline.m_cutPlane.m_normalInfo.m_x * isoline.m_cutPlane.m_normalInfo.m_x +
		isoline.m_cutPlane.m_normalInfo.m_y * isoline.m_cutPlane.m_normalInfo.m_y + 
		isoline.m_cutPlane.m_normalInfo.m_z * isoline.m_cutPlane.m_normalInfo.m_z );	
	double unitVector[3] = {
		isoline.m_cutPlane.m_normalInfo.m_x,	// / vectorNorm,
		isoline.m_cutPlane.m_normalInfo.m_y,	// / vectorNorm,
		isoline.m_cutPlane.m_normalInfo.m_z	// / vectorNorm,
	};
	double origin[3] = {
		imagedataCenter[0] + unitVector[0] * isoline.m_cutPlane.m_centerOffset,
		imagedataCenter[1] + unitVector[1] * isoline.m_cutPlane.m_centerOffset,
		imagedataCenter[2] + unitVector[2] * isoline.m_cutPlane.m_centerOffset
	};
	double normal[3] = {
		isoline.m_cutPlane.m_normalInfo.m_x, 
		isoline.m_cutPlane.m_normalInfo.m_y, 
		isoline.m_cutPlane.m_normalInfo.m_z
	};
	//auto CutterPlane = vtkSmartPointer<vtkPlane>::New();
	if (isoline.m_cutPlane.m_mode == 1) {	// 轴平行面
		double point[3];
		tri_point[0]->GetPosition(point);
		point[isoline.m_cutPlane.m_axis] = 0;
			Vec3d tri_pointA(tri_point[0]->GetPosition());
			Vec3d tri_pointB(tri_point[1]->GetPosition());
			Vec3d tri_pointC(point);
			Vec3d nor;
			Cal_Normal_3D(tri_pointA, tri_pointB, tri_pointC, nor);
			m_cutterPlane->SetOrigin(tri_point[0]->GetPosition());
			m_cutterPlane->SetNormal(nor.x, nor.y, nor.z);
	}
	else if (isoline.m_cutPlane.m_mode ==2) {
		if(0== tri_point_n){
			double nor[3] = { // 相对左下角偏移量
				origin_cmd[0]- begin_row,
				origin_cmd[1]- begin_col,
				origin_cmd[2]- begin_high,
			};
			m_cutterPlane->SetOrigin(origin_cmd);
			m_cutterPlane->SetNormal(nor);
		}
		else {
			Vec3d tri_pointA(tri_point[0]->GetPosition());
			Vec3d tri_pointB(tri_point[1]->GetPosition());
			Vec3d tri_pointC(tri_point[2]->GetPosition());
			Vec3d nor;
			Cal_Normal_3D(tri_pointA, tri_pointB, tri_pointC, nor);
			m_cutterPlane->SetOrigin(tri_point[0]->GetPosition());
			m_cutterPlane->SetNormal(nor.x, nor.y, nor.z);
		}
	}else{	// 轴垂直面
		m_cutterPlane->SetOrigin(origin); //normal不必归一化处理；
		m_cutterPlane->SetNormal(normal);
			//unitVector);
	}
	auto Cutter = vtkSmartPointer<vtkCutter>::New();
	Cutter->SetCutFunction(m_cutterPlane);
	Cutter->SetInputData(imageData);
	Cutter->GenerateCutScalarsOff();
	Cutter->Update();
	auto cutter_data = Cutter->GetOutput();

	std::vector<float> line_vale;
	switch (isoline.m_level)
	{
	case AUTO_CALCULATE:
	{
		double scalar_range[2];
		cutter_data->GetScalarRange(scalar_range);
		//isoline.m_type.m_typeAuto.m_count = 5;;//只有此选项可以用户更改
		isoline.m_type.m_typeAuto.m_minLevel = scalar_range[0] + (scalar_range[1] - scalar_range[0]) / isoline.m_type.m_typeAuto.m_count * 2;//auto的最小等级值计算方法scalar_min+(scalar_max-scalar_min)/m_count*2
		isoline.m_type.m_typeAuto.m_maxLevel = scalar_range[1] - (scalar_range[1] - scalar_range[0]) / isoline.m_type.m_typeAuto.m_count * 2;//auto的最大等级值计算方法scalar_max-(scalar_max-scalar_min)/m_count*2

		float step = (isoline.m_type.m_typeAuto.m_maxLevel - isoline.m_type.m_typeAuto.m_minLevel) / (isoline.m_type.m_typeAuto.m_count - 1);
		for (size_t i = 0; i < isoline.m_type.m_typeAuto.m_count; i++)
		{
			line_vale.push_back(isoline.m_type.m_typeAuto.m_minLevel + step * i);
		}
		break;
	}
	case MIN_MAX_INTERVAL:
	{
		//double scalar_range[2];
		//cutter_data->GetScalarRange(scalar_range);//isoline.m_type.m_typeInterval.m_minLevel和isoline.m_type.m_typeInterval.m_maxLevel 需要处理scalar_range[0]到scalar_range[1]之间

		//isoline.m_type.m_typeInterval.m_minLevel = scalar_range[0];//可以用户设置 值有限制如上所说
		//isoline.m_type.m_typeInterval.m_maxLevel = scalar_range[1];//可以用户设置 值有限制如上所说
		//isoline.m_type.m_typeInterval.m_interval = (scalar_range[1] - scalar_range[0]) / 3;//次选项值应小于scalar_range[1]-scalar_range[0]

		int count = (isoline.m_type.m_typeInterval.m_maxLevel - isoline.m_type.m_typeInterval.m_minLevel) / isoline.m_type.m_typeInterval.m_interval + 2;
		for (size_t i = 0; i < count; i++)
		{
			if (i == count - 1)
				line_vale.push_back(isoline.m_type.m_typeInterval.m_maxLevel);
			line_vale.push_back(isoline.m_type.m_typeInterval.m_minLevel + isoline.m_type.m_typeInterval.m_interval * i);
		}
		break;
	}
	case MIN_MAX_CALCULATE:
	{
		//double scalar_range[2];
		//cutter_data->GetScalarRange(scalar_range);//isoline.m_type.m_typeCount.m_minLevel和isoline.m_type.m_typeCount.m_maxLevel 需要处理scalar_range[0]到scalar_range[1]之间
		//isoline.m_type.m_typeCount.m_count = 5;//无限制但值过大可能可很吃资源
		//isoline.m_type.m_typeCount.m_minLevel = scalar_range[0];//可以用户设置 值有限制如上所说
		//isoline.m_type.m_typeCount.m_maxLevel = scalar_range[1];//可以用户设置 值有限制如上所说

		float step = (isoline.m_type.m_typeCount.m_maxLevel - isoline.m_type.m_typeCount.m_minLevel) / (isoline.m_type.m_typeCount.m_count - 1);
		for (size_t i = 0; i < isoline.m_type.m_typeCount.m_count; i++)
		{
			line_vale.push_back(isoline.m_type.m_typeCount.m_minLevel + step * i);
		}
		break;
	}
	case EXPLICIT:
	{
		//double scalar_range[2];
		//cutter_data->GetScalarRange(scalar_range);
		//isoline.m_type.m_typeExplicit.m_levelValue = (scalar_range[1] - scalar_range[0]) / 10 + scalar_range[0];//直接给定值计算一条等值线
		line_vale.push_back(isoline.m_type.m_typeExplicit.m_levelValue);
	}
	default:
		break;
	}

	auto contour = vtkSmartPointer<vtkContourFilter>::New();
	contour->SetInputData(cutter_data);
	for (size_t i = 0; i < line_vale.size(); i++)
	{
		contour->SetValue(i, line_vale[i]);
	}
	contour->Update();

	int cr = setLut(m_isoLine.m_render.m_gradientMap, imageData->GetScalarRange(), isoline.m_render.m_colorBar.m_label);
	m_scalarBar->SetLookupTable(m_lut);
	setColorBar(isoline.m_render.m_colorBar);

	auto isolineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	isolineMapper->SetInputData(contour->GetOutput());
	isolineMapper->SetLookupTable(m_lut);
	isolineMapper->SetScalarRange (imageData->GetScalarRange());

	isoline_actor->SetMapper(isolineMapper);
	isoline_actor->GetProperty()->SetLineWidth(isoline.m_render.m_lineWidth);

	auto outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//outlineMapper->SetInputData(outline->GetOutput());
	outlineMapper->SetInputConnection(Cutter->GetOutputPort());
	outlineMapper->SetResolveCoincidentTopologyToPolygonOffset();
	outlineMapper->SetLookupTable(m_lut);
	outlineMapper->SetScalarRange(imageData->GetScalarRange());
	outlineMapper->UseLookupTableScalarRangeOff();

	outline_actor->SetMapper(outlineMapper);
	outline_actor->GetProperty()->SetLineWidth(isoline.m_render.m_boundWidth);
	//outline_actor->GetProperty()->SetColor(isoline.m_render.m_boundRgb.m_r / 255.0, isoline.m_render.m_boundRgb.m_g / 255.0, isoline.m_render.m_boundRgb.m_b / 255.0);
	outline_actor->GetProperty()->SetColor(255 / 255.0, 0 / 255.0, 0 / 255.0);
	outline_actor->GetProperty()->SetDiffuse(1.0);
	outline_actor->GetProperty()->SetAmbient(1.0);
	if(m_checked){
		vtkSmartPointer<vtkRenderer> ren= m_project.lock()->getRender();
		ren->AddActor(isoline_actor);
		if (isoline.m_render.m_colorBar.m_displayOn)
			ren->AddActor2D(m_scalarBar);
		if (isoline.m_render.display_bound)
			ren->AddActor(outline_actor);
		else
			ren->RemoveActor(outline_actor);
		//rd->AddActor(actor);
		ren->AddActor(glyphActor);
		ren->AddActor(tri_point[0]);
		ren->AddActor(tri_point[1]);
		ren->AddActor(tri_point[2]);
		if(pointWidget)
		pointWidget->SetEnabled(dragger_en);	 
		//rd->Render();
		//m_project.lock()->getWinInteractor()->GetRenderWindow()->Render();
	}
	m_excuted = true;
	//makeNextNodeRun();
	return true;
}

bool GIsoLineNode::addToRender()	
{
	if (m_excuted){
		vtkSmartPointer<vtkRenderer> ren = m_project.lock()->getRender();
		ren->AddActor(isoline_actor);
		if (m_isoLine.m_render.m_colorBar.m_displayOn)
			ren->AddActor2D(m_scalarBar);
		if (m_isoLine.m_render.display_bound)
			ren->AddActor(outline_actor);
		//ren->AddActor(glyphActor);
		ren->AddActor(tri_point[0]);
		ren->AddActor(tri_point[1]);
		ren->AddActor(tri_point[2]);
		if (pointWidget)
		pointWidget->SetEnabled(dragger_en);	 
	}
	return  m_excuted;
}

bool GIsoLineNode::removeFromRender()	
{
	if (m_excuted){
		vtkSmartPointer<vtkRenderer> ren = m_project.lock()->getRender();
		ren->RemoveActor(isoline_actor);
		ren->RemoveActor(m_scalarBar);
		ren->RemoveActor(outline_actor);
		//ren->RemoveActor(glyphActor);
		ren->RemoveActor(tri_point[0]);
		ren->RemoveActor(tri_point[1]);
		ren->RemoveActor(tri_point[2]);
		if (pointWidget)
		pointWidget->SetEnabled(false);
	}
	return m_excuted;
}

void GIsoLineNode::getBound(double bounds[6])
{
}

void GIsoLineNode::setBound(double * bounds)
{
}

void GIsoLineNode::updateDisplay(std::shared_ptr<SuperNodeStruct> isoLine)
{
	std::shared_ptr<G_IsoLine> node = std::dynamic_pointer_cast<G_IsoLine>(isoLine);
	switch (isoLine->m_changetype)
	{
	case 5:
	{
		updateDisplayColorBar(m_isoLine.m_render.m_colorBar, node->m_render.m_colorBar);
	}
	break;
	default:
		break;
	}
	//m_project.lock()->getRender()->Render();
}

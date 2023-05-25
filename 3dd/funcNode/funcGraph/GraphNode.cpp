#include "GraphNode.h"
#include "qcolor.h"

#include "XmlProject.h"

GraphNode::GraphNode(std::shared_ptr< XmlProject> project) :SurperNode(project)
{
	m_nodeBelongType = NodeType::NODE_G_TYPE;
	m_scalarBar = vtkSmartPointer< vtkScalarBarActor>::New();
	m_lut = vtkSmartPointer<vtkLookupTable>::New();

	probe = vtkSmartPointer<vtkProbeFilter>::New();
	nodeCallback = vtkSmartPointer<vtkNodeCallback>::New();
	sphereSource = vtkSmartPointer<vtkSphereSource>::New(); ;
	inputPolyData = vtkSmartPointer<vtkPolyData>::New();
	point = vtkSmartPointer<vtkPolyData>::New();
	glyph = vtkSmartPointer<vtkGlyph3D>::New();
	glyphMapper = vtkSmartPointer<vtkPolyDataMapper> ::New();
	glyphActor = vtkSmartPointer<vtkActor>::New();

	sphere = vtkSmartPointer<vtkSphereSource>::New();
	tri_point[0] = vtkSmartPointer<vtkActor>::New();
	tri_point[1] = vtkSmartPointer<vtkActor>::New();
	tri_point[2] = vtkSmartPointer<vtkActor>::New();
	tri_point[0]->GetProperty()->SetColor(0.0, 0.0, 1.0);
	tri_point[1]->GetProperty()->SetColor(0.0, 0.0, 1.0);
	tri_point[2]->GetProperty()->SetColor(0.0, 0.0, 1.0);
	m_cutterPlane = vtkSmartPointer<vtkPlane>::New();
}

GraphNode::~GraphNode()
{
	if (pointWidget)
		pointWidget->Off();
}

std::shared_ptr<SuperNodeStruct> GraphNode::getParameter()
{
	return nullptr;
}

void GraphNode::setChecked(bool checked)
{
	m_checked = checked;
	if (m_checked)
		addToRender();
	else
		removeFromRender();
}

void GraphNode::draggerOn(bool s)
{
	dragger_en = s;
	if(pointWidget){
		pointWidget->Off();
		pointWidget = nullptr;
	}
	if (!s) 
		return;

	pointWidget = vtkSmartPointer<vPointWidget>::New();
	pointWidget->node = this;
	// The point widget is used probe the dataset.
	vtkRenderWindowInteractor* iren = m_project.lock()->getWinInteractor();
	pointWidget->SetInteractor(iren);
	pointWidget->AllOff();
	pointWidget->AddObserver(vtkCommand::InteractionEvent, nodeCallback);
	pointWidget->SetInputData(inputPolyData);
	pointWidget->PlaceWidget();
	pointWidget->On();
	pointWidget->GetPosition(origin_cmd);
	setNewPos(origin_cmd);
}
void GraphNode::setFocused(bool checked)
{
	vtkSmartPointer<vtkRenderer> ren = m_project.lock()->getRender();
	if (checked) {
		ren->AddActor(tri_point[0]);
		ren->AddActor(tri_point[1]);
		ren->AddActor(tri_point[2]);
		if (pointWidget)
		pointWidget->SetEnabled(dragger_en);
	}
	else {
		ren->RemoveActor(tri_point[0]);
		ren->RemoveActor(tri_point[1]);
		ren->RemoveActor(tri_point[2]);
		if(pointWidget)
		pointWidget->SetEnabled(false);
	}
}

void GraphNode::setInputData(std::vector<pointFinal>& data)
{
	m_excuted = false;
	m_data = data;
}

bool GraphNode::getOnputData(std::vector<pointFinal>& data)
{
	if (!m_project.lock() || m_nodeIndex == RETURN_ERROR)
		return false;
	data = m_data;
	return true;
}

void GraphNode::setInputData(std::vector<pointOriginal>& data, int dataIndex)
{
	std::vector<pointFinal> _data;
	_data.clear();
	int size = m_data.size();
	m_data.clear();
	for (auto var : data)
	{
		m_data.push_back(pointFinal(var.x, var.y, var.z, var.scalar[dataIndex]));
	}
}

void GraphNode::setDataRange(double* dataRange)
{
	m_excuted = false;
	memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	m_ranged = 1;
}

void GraphNode::setResolution(int * slt)
{
	m_excuted = false;
}

void GraphNode::setLimits(double * limits)
{
}

bool GraphNode::set_progress(int range, int pos, const char*)
{
	if (widget) {
		base_property_widget* pWidget = (base_property_widget*)widget;
		pWidget->data_count_to_widget(m_data.size()*pos/range, 0);
	}
	return widget;
}

// 对应于base_property_widget::gradient_color
int GraphNode::setLut( int index, double range[2], Child_labelMark& m_label, bool enLabelMark)
{
	int cn[] = {7, 4, 4, 4, 7, 6, 6, 7, 25};	
	const QColor ct[][25] = {
		{QColor( 153, 102, 252), QColor( 58, 39, 255), QColor( 0, 255, 0), QColor( 255, 255, 0), QColor( 255, 220, 0), QColor( 255, 102, 0), QColor( 255, 0, 0)},
		{ Qt::white, Qt::red, Qt::green, Qt::black }, // "white-black"	//	{ Qt::white, Qt::black }, 
		{ Qt::red, Qt::yellow, Qt::green, Qt::blue },		// "red-blue"		//{ Qt::red,  Qt::blue },			//{QColor("seagreen"),  QColor("mediumblue")},	
		{QColor("seagreen"), QColor("gold"), QColor("beige"), QColor("mediumblue")},	// "seagreen-mediumblue"
		{QColor("red"), QColor("orange"), QColor("yellow"), QColor("green"), QColor("darkcyan"),QColor("blue"), QColor("purple")}, // "red-purple"		//{QColor("red"),  QColor("purple")}, 
		{QColor("blue"), QColor("black"), QColor("green"),  QColor("yellow"), QColor("orange"), QColor("red")}, //"blue-red"		//{QColor("blue"),  QColor("red")}, 
		{QColor(195, 20, 24), QColor(247, 106, 14), QColor(234, 205, 30), QColor(102, 171, 9), QColor(19, 125, 224), QColor(136, 9, 181)},	// prism
		//{QColor(238, 238, 238), QColor(204, 204, 204), QColor(153, 153, 153), QColor(102, 102, 102), QColor(51, 51, 51), QColor(0, 0, 0)},
	{QColor(0, 0, 0), QColor(43, 43, 43), QColor(85, 85, 85), QColor(128, 128, 128), QColor(171, 171, 171), QColor(213, 214, 213), QColor(255, 255, 255)},
		{QColor(0,0,0), QColor(255,0,0), QColor(255,255,0), QColor(0,255,0), QColor(0,255,255), QColor(0,0,255), QColor(255,0,255), QColor(255,255,255),
		QColor(127,127,127), QColor(191,191,191), QColor(255, 0, 0), QColor(255,127,127), QColor(204,0,0), QColor(204,102,102), QColor(153,0,0), QColor(153,76,76), 
			QColor(127,0,0), QColor(127,63,63), QColor(76,0,0), QColor(76, 38,38), QColor(255,63,0), QColor(255,159,127), QColor(255,159,127), QColor(255,159,127), QColor(255,159,127)}	//autocad
	};
	if (sizeof(cn)/sizeof(int) < index+1)
		index = 0;
	m_lut->SetNumberOfColors(LT_COLOR_NUM);//cn[index]
#if 1
	int ic, id, iv;
	for (ic = 1; ic < cn[index]; ic++) {
		double br, bg, bb, ba;
		double dr, dg, db, da;
		ct[index][ic - 1].getRgbF(&br, &bg, &bb, &ba);
		ct[index][ic].getRgbF(&dr, &dg, &db, &da);
		iv = LT_COLOR_NUM / (cn[index] - 1);	// step
		for (id = 0; id < iv; id++) {	// 插值
			double r = br + (dr - br)*id / iv;
			double g = bg + (dg - bg)*id / iv;
			double b = bb + (db - bb)*id / iv;
			double a = ba + (da - ba)*id / iv;
			m_lut->SetTableValue((ic-1)*iv + id, r, g, b, a);	// 索引-颜色 https://www.cnblogs.com/ybqjymy/p/13984752.html
		}
	}
	m_lut->SetTableRange(range);		// 标量smin, smax
	m_lut->Build();
#else
	double hue1= ct[index][0].hueF() ;
	double hue2=  ct[index][1].hueF() ;
	//m_lut->SetHueRange(0, 1);
	m_lut->SetHueRange(hue1, hue2);// (0, 1);
#endif
	if(!enLabelMark)
		return LT_COLOR_NUM;

	std::string  format;
	if (0 == m_label.m_type)
	{
		format = "%." + std::to_string(m_label.m_decimalDigits) + "f";
	}
	if (1 == m_label.m_type)
	{
		format = "%f";
	}
	else if (2 == m_label.m_type)
	{
		format = "%." + std::to_string(m_label.m_decimalDigits) + "e";
	}
	format = m_label.m_preText + format +
		m_label.m_lastText;

		double step = (range[1]-range[0])/(m_label.m_num-1);
	for (ic = 0; ic < m_label.m_num; ic++) {
		double val = step * ic+ range[0];	// 标量scalar
		char str[260] = "";
		_snprintf_s(str, sizeof(str), format.c_str(), val);
		m_lut->SetAnnotation(val, str);		// scalar-string
	}
	return LT_COLOR_NUM;
}

void GraphNode::setColorBarOn(bool checked)// yzw 0810
{
	if (m_excuted) {
		if (checked)
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
		else
			m_project.lock()->getRender()->RemoveActor(m_scalarBar);
	}
}

void GraphNode::setColorBar(Child_ColorBar &m_colorBar)
{
	m_scalarBar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
	m_scalarBar->SetUnconstrainedFontSize(true);
	if (ColorBarDirection::D_HORIZONTAL == m_colorBar.m_direction)
	{
		m_scalarBar->SetOrientationToHorizontal();
	}
	else if (ColorBarDirection::D_VERTICAL == m_colorBar.m_direction)
	{
		m_scalarBar->SetOrientationToVertical();
	}
	m_scalarBar->GetPositionCoordinate()->SetValue(m_colorBar.m_xPos, m_colorBar.m_yPos);
	m_scalarBar->SetWidth(m_colorBar.m_width);
	m_scalarBar->SetHeight(m_colorBar.m_height);

	vtkUnicodeString str = vtkUnicodeString::from_utf8(m_colorBar.m_title.m_title);
	m_scalarBar->SetTitle(str.utf8_str());
	m_scalarBar->GetTitleTextProperty()->SetFontSize(m_colorBar.m_title.m_fontSize);

	//	m_scalarBar->DrawAnnotationsOn();	// The default is true
		m_scalarBar->SetTextPositionToPrecedeScalarBar();
		m_scalarBar->GetAnnotationTextProperty()->SetColor(0., 0., 1.);
		m_scalarBar->GetAnnotationTextProperty()->SetFontSize(m_colorBar.m_label.m_fontSize);
		m_scalarBar->SetNumberOfLabels(0); //m_colorBar.m_label.m_num
	if (0) //(m_colorBar.m_label.m_customOn) //
	{
		m_scalarBar->SetMaximumNumberOfColors(m_lut->GetNumberOfColors());
		m_scalarBar->GetLabelTextProperty()->SetFontSize(m_colorBar.m_label.m_fontSize);

		std::string  format;
		if (0 == m_colorBar.m_label.m_type)
		{
			format = "%." + std::to_string(m_colorBar.m_label.m_decimalDigits) + "f";
		}
		if (1 == m_colorBar.m_label.m_type)
		{
			format = "%f";
		}
		else if (2 == m_colorBar.m_label.m_type)
		{
			format = "%." + std::to_string(m_colorBar.m_label.m_decimalDigits) + "e";
		}
		format = m_colorBar.m_label.m_preText + format +
			m_colorBar.m_label.m_lastText;
		m_scalarBar->SetLabelFormat(format.c_str());
	}
}

// 从GScatterMapNode::updateDisplay修改过来
void GraphNode::updateDisplayColorBar(Child_ColorBar &cur, Child_ColorBar &new1)
{
	do{
	if (new1.m_displayOn)
	{
			if(new1.m_direction != cur.m_direction)
			if (ColorBarDirection::D_HORIZONTAL == new1.m_direction)
			{
				m_scalarBar->SetOrientationToHorizontal();
				//break;
			}
			else
			{
				m_scalarBar->SetOrientationToVertical();
				//break;
			}
			if (new1.m_xPos != cur.m_xPos ||
				new1.m_yPos != cur.m_yPos)
			{
				m_scalarBar->SetPosition(new1.m_xPos, new1.m_yPos);
				//m_scalarBar->SetPosition2(new1.m_xPos + new1.m_width,		new1.m_yPos + new1.m_height);
				//break;
			}
			if (new1.m_width != cur.m_width)
				m_scalarBar->SetWidth(new1.m_width);
			if	(new1.m_height != cur.m_height)
			{
				//m_scalarBar->SetPosition2(new1.m_xPos + new1.m_width,		new1.m_yPos + new1.m_height);
				m_scalarBar->SetHeight(new1.m_height);
				//break;
			}
			if (new1.m_title.m_title != cur.m_title.m_title)
			{
				m_scalarBar->SetTitle(new1.m_title.m_title.c_str());
				//break;
			}
			if (new1.m_title.m_fontSize != cur.m_title.m_fontSize)
			{
				m_scalarBar->GetTitleTextProperty()->SetFontSize(new1.m_title.m_fontSize);
				//break;
			}
			if (new1.m_label.m_num != cur.m_label.m_num)
			{
				//m_scalarBar->SetMaximumNumberOfColors(new1.m_label.m_num);
				//m_scalarBar->SetNumberOfLabels(new1.m_label.m_num);
				//break;
			}
			if (new1.m_label.m_fontSize != cur.m_label.m_fontSize)
			{
				m_scalarBar->GetLabelTextProperty()->SetFontSize(new1.m_label.m_fontSize);
				m_scalarBar->GetAnnotationTextProperty()->SetFontSize(new1.m_label.m_fontSize);
				//break;
			}
			if(0)// (new1.m_label.m_customOn)
			{
				m_scalarBar->SetMaximumNumberOfColors(m_lut->GetNumberOfColors());
				m_scalarBar->SetNumberOfLabels(new1.m_label.m_num);
				m_scalarBar->GetLabelTextProperty()->SetFontSize(new1.m_label.m_fontSize);

				std::string  format;
				if (0 == new1.m_label.m_type)
				{
					format = "%." + std::to_string(new1.m_label.m_decimalDigits) + "f";
				}
				if (1 == new1.m_label.m_type)
				{
					format = "%f";
				}
				else if (2 == new1.m_label.m_type)
				{
					format = "%." + std::to_string(new1.m_label.m_decimalDigits) + "e";
				}
				format = new1.m_label.m_preText + format +
					new1.m_label.m_lastText;
				m_scalarBar->SetLabelFormat(format.c_str());
			}
			m_project.lock()->getRender()->AddActor2D(m_scalarBar);
			//break;
	}
	else
	{
			m_project.lock()->getRender()->RemoveActor2D(m_scalarBar);
	}
	} while (0);
	//memcpy_s(&cur, sizeof(Child_ColorBar), &new1, sizeof(Child_ColorBar));
	cur = new1;
}

//void GraphNode::beginRun()
//{
//	m_project.lock()->run(shared_from_this());
//}

bool GraphNode::saveData(std::string path)
{
	if (m_excuted) {
		//cpl 7.29
		std::ofstream ofs(path);
		if (!ofs.is_open())
		{
			return false;
		}
		else
		{
			ofs << "x\ty\tz\tscalar\n";
			for (int i = 0; i < m_data.size(); i++)
			{
				ofs << (double)m_data.at(i).x << "\t" 
					<< (double)m_data.at(i).y << "\t" 
					<< (double)m_data.at(i).z << "\t" 
					<< (double)m_data.at(i).scalar << "\n";
			}
			ofs.flush();
			ofs.close();
		}
	}
	return true;
}

bool GraphNode::nextNodeRun(int8_t nodeId)
{
	return true;
}

void GraphNode::setNewPos(double* pos)
{}

vPointWidget* vPointWidget::New()
{
	return new vPointWidget;
}

vPointWidget::vPointWidget()
{
	;
}

vPointWidget::~vPointWidget()
{
	;
}

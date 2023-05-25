/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
//
// 文件功能描述：
//
//
// 创建者：tangAo
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once
#include "GlobalInclude.h"

struct modelInfo
{
	int m_leftTopPosX = 0;
	int m_leftTopPosY = 0;
	int m_width = 0;
	int m_length = 0;
	modelInfo() {
		 m_leftTopPosX = 0;
		 m_leftTopPosY = 0;
		 m_width = 0;
		 m_length = 0;
	}
	CONSTRUCTOR_MEMCPY(modelInfo)
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pPosInfo)
	{
		pPosInfo->SetAttribute("leftTopPosX", m_leftTopPosX);
		pPosInfo->SetAttribute("leftTopPosY", m_leftTopPosY);
		pPosInfo->SetAttribute("width", m_width);
		pPosInfo->SetAttribute("length", m_length);
	}
	int fromXml(TiXmlElement* pPosInfo)
	{
		int ret = pPosInfo->QueryIntAttribute("leftTopPosX", &m_leftTopPosX);
		if (TIXML_SUCCESS != ret) return ret;
		ret = pPosInfo->QueryIntAttribute("leftTopPosY", &m_leftTopPosY);
		if (TIXML_SUCCESS != ret) return ret;
		ret = pPosInfo->QueryIntAttribute("width", &m_width);
		if (TIXML_SUCCESS != ret) return ret;
		return pPosInfo->QueryIntAttribute("length", &m_length);
	}
};

struct SuperNodeStruct
{
		SuperNodeStruct(){}
	CONSTRUCTOR_MEMCPY(SuperNodeStruct)		
	void setProjectId(int8_t id)
	{
		m_projectId = id;
	}
	virtual int8_t getProjectId()
	{
		return m_projectId;
	}

	void  set_modelInfo(modelInfo a)
	{
		this->m_info.m_leftTopPosX = a.m_leftTopPosX;
		this->m_info.m_leftTopPosY = a.m_leftTopPosY;
	}

	modelInfo m_info;
	int m_changetype = 0;//此字段为0时，则所有字段都发生更新
	int8_t m_nodeIndex;
	NodeType m_nodeType;
	std::string m_name;
	std::string m_putIn;//输入源
	void* widget= nullptr;
private:
	int8_t m_projectId = -1;
};

enum fileType
{
	DAT,
	DEM,
};
struct D_DataInfo : public SuperNodeStruct
{
	std::string m_filePath;
	fileType m_filetype;
	int data_count = 0;	
	int fields_num = 0;	// of scalar
	D_DataInfo()
	{
		m_nodeType = NodeType::NODE_D_DATASOURCE_TYPE;
	}
	D_DataInfo	(const D_DataInfo& s):SuperNodeStruct(s)
	{
		this->m_filePath = s.m_filePath;
	}
	// yzw 0702
	void toXml(std::shared_ptr<TiXmlElement> pDataInfo)
	{
		pDataInfo->SetAttribute("filePath", m_filePath.data());
		pDataInfo->SetAttribute("fileType", m_filetype);

	}
	int fromXml(TiXmlElement* pDataInfo)
	{
		int tmp=0;
		m_filePath = pDataInfo->Attribute("filePath");
		pDataInfo->QueryIntAttribute("fileType", &tmp); m_filetype = (fileType)tmp;
		return TIXML_SUCCESS;
	}
};

struct Child_colorRGB
{
	Child_colorRGB()
	{
	}
	CONSTRUCTOR_MEMCPY(Child_colorRGB)
	Child_colorRGB(	uint16_t r,	uint16_t g,	uint16_t b)
	{
		this->m_r = r;
		this->m_g = g;
		this->m_b = b;
	}
	uint16_t m_r;
	uint16_t m_g;
	uint16_t m_b;
};

enum ColorMethod
{
	GRADIENT_OF_COLOR,
	CUSTORM_OF_COLOR
};

enum CutPlaneDirection
{
	D_XY,
	D_XZ,
	D_YZ
};

enum ColorBarDirection
{
	D_VERTICAL,
	D_HORIZONTAL
};

//三维double矢量
struct Vec3d
{
	double x, y, z;

	Vec3d()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	Vec3d(double dx, double dy, double dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	Vec3d(double *xyz)
	{
		x = xyz[0];
		y = xyz[1];
		z = xyz[2];
	}
	void Set(double dx, double dy, double dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
};

struct PosXYZ
{
	float m_x;
	float m_y;
	float m_z;
	PosXYZ() {}
	PosXYZ(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
	CONSTRUCTOR_MEMCPY(PosXYZ)
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pPosXYZ)
	{
		pPosXYZ->SetAttribute("m_x", m_x);
		pPosXYZ->SetAttribute("m_y", m_y);
		pPosXYZ->SetAttribute("m_z", m_z);
	}
	//cpl 7.4
	int fromXml(TiXmlElement* pDataInfo)
	{
		int ret = pDataInfo->QueryFloatAttribute("m_x", &m_x);
		if (TIXML_SUCCESS != ret) return ret;
		ret = pDataInfo->QueryFloatAttribute("m_y", &m_y);
		if (TIXML_SUCCESS != ret) return ret;
		ret = pDataInfo->QueryFloatAttribute("m_z", &m_z);
		if (TIXML_SUCCESS != ret) return ret;
		return TIXML_SUCCESS;
	}
};
#define Child_NormalDirection PosXYZ
///
struct Child_CutingPlane
{
	Child_CutingPlane() {}
	CONSTRUCTOR_MEMCPY(Child_CutingPlane)
	CutPlaneDirection m_direction;//摆放方向
	Child_NormalDirection m_normalInfo;//法线方向
	float m_centerOffset;//离中心偏移量
	int		m_mode;
	int		m_axis;
	void getDefault()
	{
		m_direction = CutPlaneDirection::D_XY;//此参数前三个平面只是生成常用平面发线方向 计算由法线方向决定
		//D_XY： 0，0，1    D_XZ： 0，1，0   D_YZ： 1，0，0
		m_normalInfo.m_x = 0;
		m_normalInfo.m_y = 0;
		m_normalInfo.m_z = 15;
		m_centerOffset = 0;
		m_mode = 0;
		m_axis = 1;
	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pCuttingPlane)
	{
		pCuttingPlane->SetAttribute("direction", (int)m_direction);
		pCuttingPlane->SetAttribute("centerOffset", m_centerOffset);
		pCuttingPlane->SetAttribute("mode", m_mode);
		pCuttingPlane->SetAttribute("axis", m_axis);
		std::shared_ptr<TiXmlElement> pNormalInfo = std::make_shared<TiXmlElement>("NormalInfo");
		m_normalInfo.toXml(pNormalInfo);
		pCuttingPlane->InsertEndChild(*pNormalInfo);
	}
	int fromXml(TiXmlElement* pCuttingPlane)
	{
		int ret, tmp;
		ret = pCuttingPlane->QueryIntAttribute("direction", &tmp); m_direction = (CutPlaneDirection)tmp;
		if (TIXML_SUCCESS != ret) return ret;
		ret = pCuttingPlane->QueryFloatAttribute("centerOffset", &m_centerOffset);
		if (TIXML_SUCCESS != ret) return ret;
		ret = pCuttingPlane->QueryIntAttribute("mode", &m_mode); 
		if (TIXML_SUCCESS != ret) return ret;
		ret = pCuttingPlane->QueryIntAttribute("axis", &m_axis);
		if (TIXML_SUCCESS != ret) return ret;
		TiXmlElement* pNormalInfo = pCuttingPlane->FirstChildElement("NormalInfo");
		ret= m_normalInfo.fromXml(pNormalInfo);
		if (TIXML_SUCCESS != ret) return ret;
		return ret;
	}
};

struct Child_Title
{
	Child_Title() {}
	std::string m_title;
	uint16_t m_fontSize;
	CONSTRUCTOR_MEMCPY(Child_Title)
		// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pText)
	{
		pText->SetAttribute("text", m_title.data());
		pText->SetAttribute("fontSize", m_fontSize);
	}
	int fromXml(TiXmlElement* pText)
	{
		int size = 0;
		m_title = pText->Attribute("text");
		int ret = pText->QueryIntAttribute("fontSize", &size);		m_fontSize = size;
		return ret;
	}
};

struct Child_labelMark //标注
{
	Child_labelMark() {}
	CONSTRUCTOR_MEMCPY(Child_labelMark)
	int m_num;//标注数量
	uint16_t m_fontSize;//字号大小
	bool m_customOn;//采用自定义标注
	uint16_t m_type;////标注格式类型，常规，固定值，指数形式
	uint16_t m_decimalDigits;//小数位数
	std::string m_preText;//前缀
	std::string m_lastText;//后缀
		// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pLabelMark)
	{
		pLabelMark->SetAttribute("num", m_num);
		pLabelMark->SetAttribute("fondSize", m_fontSize);
		pLabelMark->SetAttribute("customOn", m_customOn);
		pLabelMark->SetAttribute("type", m_type);
		pLabelMark->SetAttribute("decimalDigits", m_decimalDigits);
		pLabelMark->SetAttribute("preText", m_preText.data());
		pLabelMark->SetAttribute("lastText", m_lastText.data());
	}
	int fromXml(TiXmlElement* pLabelMark)
	{
		int tmp;
		int ret = pLabelMark->QueryIntAttribute("num", &m_num);
		ret = pLabelMark->QueryIntAttribute("fontSize", &tmp);	m_fontSize = tmp;
		ret = pLabelMark->QueryBoolAttribute("customOn", &m_customOn);
		ret = pLabelMark->QueryIntAttribute("type", &tmp);	m_type = tmp;
		ret = pLabelMark->QueryIntAttribute("decimalDigits", &tmp);	m_decimalDigits = tmp;
		m_preText= pLabelMark->Attribute("preText");
		m_lastText = pLabelMark->Attribute("lastText");
		return ret;
	}
};

struct Child_ColorBar
{
	bool m_displayOn;//显示色标
	ColorBarDirection m_direction;//摆放方向
	float m_xPos;//X轴位置
	float m_yPos;//Y轴位置
	float m_width;//宽度
	float m_height;//长度
	Child_Title m_title;//标题
	Child_labelMark m_label;//标注
	Child_ColorBar()
	{
		m_direction = ColorBarDirection::D_HORIZONTAL;
	}
	Child_ColorBar(const Child_ColorBar& s)
	{
		this->m_displayOn =s.m_displayOn;
		this->m_direction =s.m_direction;
		this->m_xPos =s.m_xPos;
		this->m_yPos =s.m_yPos;
		this->m_width =s.m_width;
		this->m_height =s.m_height;
		this->m_title =s.m_title;
		this->m_label =s.m_label;
	}
	void getDefault()
	{
		Child_Title title_tmp;
		title_tmp.m_title = "color bar";
		title_tmp.m_fontSize = 15;

		Child_labelMark mark_tmp;
		mark_tmp.m_customOn = true;
		mark_tmp.m_num = 10;
		mark_tmp.m_fontSize = 15;
		mark_tmp.m_type = 0;
		mark_tmp.m_decimalDigits = 3;
		mark_tmp.m_preText = "";
		mark_tmp.m_lastText = "";

		m_displayOn = false;
		m_direction = ColorBarDirection::D_VERTICAL;
		m_xPos = 0;
		m_yPos = 0;
		m_width = 0.100;
		m_height = 0.600;
		m_label = mark_tmp;
		m_title = title_tmp;
	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pColorBar)
	{
		pColorBar->SetAttribute("displayOn", m_displayOn);
		pColorBar->SetAttribute("direction", (int)m_direction);
		pColorBar->SetAttribute("xpos", m_xPos);
		pColorBar->SetAttribute("ypos", m_yPos);
		pColorBar->SetAttribute("width", m_width);
		pColorBar->SetAttribute("height", m_height);
		std::shared_ptr<TiXmlElement> pText = std::make_shared<TiXmlElement>("Text");
		m_title.toXml(pText);
		pColorBar->InsertEndChild(*pText);
		std::shared_ptr<TiXmlElement> pLabelMark = std::make_shared<TiXmlElement>("LabelMark");
		m_label.toXml(pLabelMark);
		pColorBar->InsertEndChild(*pLabelMark);
	}
	int fromXml(TiXmlElement* pColorBar)
	{
		int ret;
		if (!pColorBar)
			return TIXML_NO_ATTRIBUTE;
		ret = pColorBar->QueryBoolAttribute("displayOn", &m_displayOn);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pColorBar->QueryIntAttribute("direction", (int*)&m_direction);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pColorBar->QueryFloatAttribute("xpos", &m_xPos);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pColorBar->QueryFloatAttribute("ypos", &m_yPos);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pColorBar->QueryFloatAttribute("width", &m_width);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pColorBar->QueryFloatAttribute("height", &m_height);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement* pText = pColorBar->FirstChildElement("Text");
		ret= m_title.fromXml(pText);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement* pLabelMark = pColorBar->FirstChildElement("LabelMark");
		ret= m_label.fromXml(pLabelMark);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_SliceMap : public SuperNodeStruct//切片参数
{
	/*
	m_changeType = 1: 不透明度
	 = 2: 着色方法
	 = 3: 色标刻度尺
	 = 4: 切割平面
	*/
	std::string m_putIn;//输入源
	uint16_t m_putIndex;//输入分量
	bool m_lightOn;//照明
	bool m_original_data_slice;	// 原始数据位置切片
	double m_opacityValue;//不透明度
	ColorMethod m_colorMethod;//着色方法
	uint16_t m_gradientMap;//渐变色谱序列号
	Child_CutingPlane m_cutPlane;//切割平面
	Child_ColorBar m_colorBar;//色标刻度尺
	G_SliceMap()
	{
		m_nodeType = NodeType::NODE_G_SLICEMAP_TYPE;
	}
	G_SliceMap(const G_SliceMap& s):SuperNodeStruct(s)
	{
		this->m_putIn = s.m_putIn;
		this->m_putIndex = s.m_putIndex;
		this->m_lightOn = s.m_lightOn;
		this-> m_opacityValue= s.m_opacityValue;
		this->m_colorMethod = s.m_colorMethod;
		this->m_gradientMap = s.m_gradientMap;
		this->m_cutPlane = s.m_cutPlane;
		this->m_colorBar = s.m_colorBar;
	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("putIndex", m_putIndex);
		pParameter->SetAttribute("lightOn", m_lightOn);
		pParameter->SetAttribute("original_slice", m_original_data_slice);
		pParameter->SetDoubleAttribute("opacityValue", m_opacityValue);
		pParameter->SetAttribute("colorMethod", (int)m_colorMethod);
		pParameter->SetAttribute("gradientMap", m_gradientMap);
		std::shared_ptr<TiXmlElement> pCuttingPlane = std::make_shared<TiXmlElement>("CuttingPlane");
		m_cutPlane.toXml(pCuttingPlane);
		pParameter->InsertEndChild(*pCuttingPlane);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pParameter->InsertEndChild(*pColorBar);
	}
	//cpl 7.4
	int fromXml(TiXmlElement* pParameter)
	{
		m_putIn = pParameter->Attribute("putIn");
		int index = 0;
		int ret = pParameter->QueryIntAttribute("putIndex",&index);
		m_putIndex = index;
		ret = pParameter->QueryBoolAttribute("lightOn", &m_lightOn);
		ret = pParameter->QueryBoolAttribute("original_slice", &m_original_data_slice);
		ret = pParameter->QueryDoubleAttribute("opacityValue", &m_opacityValue);
		ret = pParameter->QueryIntAttribute("colorMethod",(int *)&m_colorMethod);
		ret = pParameter->QueryIntAttribute("gradientMap", &index);
		m_gradientMap = index;
		TiXmlElement* pCuttingPlane = pParameter->FirstChildElement("CuttingPlane");
		ret = m_cutPlane.fromXml(pCuttingPlane);
		TiXmlElement* pColorBar = pParameter->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret) return ret;
		return TIXML_SUCCESS;
	}
};

enum RenderMethod
{
	Alpha,
	MaxiMum_strength,
	MiniMum_strength,
	Average_strength,
	Additive_strength,
};

enum Child_ConstituentColor
{
	ALPHA_MIXTURE
};

enum Child_InsertMethod_VRender
{
	NEARESTNEIGHBOR,
	LINEAR_METHOD,
	THREE_LINEAR_METHOD
};

enum RenderMode
{
	Default = 0,
	RayCast,
	GPU,
	OSPRay,
};


struct ChildRender_Alpha
{
	ChildRender_Alpha() {}
    Child_InsertMethod_VRender m_insertMethod;//插值方法
    RenderMode m_renderMode; //渲染模式
    float m_sampleDistance; //采样距离
    ColorMethod m_colormethod;//着色类型，自定义，渐变色谱
    uint16_t m_gradientMap;//渐变色谱序列号
    float m_opacity;//不透明度
    Child_ColorBar m_colorBar; // 标刻度尺
	ChildRender_Alpha(const ChildRender_Alpha& s)
	{
		this->m_insertMethod = s.m_insertMethod;
		this->m_renderMode = s.m_renderMode;
		this->m_sampleDistance = s.m_sampleDistance;
		this->m_colormethod = s.m_colormethod;
		this->m_gradientMap = s.m_gradientMap;
		this->m_opacity = s.m_opacity;
		this->m_colorBar = s.m_colorBar;
	}
	void toXml(std::shared_ptr<TiXmlElement> pAlphaMethod)
	{
		pAlphaMethod->SetAttribute("insertMethod", (int)m_insertMethod);
		pAlphaMethod->SetAttribute("rendMode", m_insertMethod);
		pAlphaMethod->SetAttribute("sampleDistance", m_sampleDistance);
		pAlphaMethod->SetAttribute("colorMethod", (int)m_colormethod);
		pAlphaMethod->SetAttribute("gradientMap", m_gradientMap);
		pAlphaMethod->SetAttribute("opacity", m_opacity);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pAlphaMethod->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pAlphaMethod)
	{
		int ret;
		if (!pAlphaMethod)
			return TIXML_NO_ATTRIBUTE;
		ret = pAlphaMethod->QueryIntAttribute("insertMethod", (int*)&m_insertMethod);
		ret = pAlphaMethod->QueryIntAttribute("rendMode", (int*)&m_renderMode);
		ret = pAlphaMethod->QueryFloatAttribute("sampleDistance", &m_sampleDistance);
		ret = pAlphaMethod->QueryIntAttribute("colorMethod", (int*)&m_colormethod);
		ret = pAlphaMethod->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		ret = pAlphaMethod->QueryFloatAttribute("opacity", &m_opacity);
		TiXmlElement * pColorBar = pAlphaMethod->FirstChildElement("ColorBar");
		ret=m_colorBar.fromXml(pColorBar);
		return ret;
	}
};

struct ChildRender_MaxiMum_strength
{
	ChildRender_MaxiMum_strength() {}
    Child_InsertMethod_VRender m_insertMethod;//插值方法
    RenderMode m_renderMode; //渲染模式
    float m_sampleDistance; //采样距离
    ColorMethod m_colormethod;//着色类型，自定义，渐变色谱
    uint16_t m_gradientMap;//渐变色谱序列号
    float m_opacity;//不透明度
    Child_ColorBar m_colorBar; // 标刻度尺
	ChildRender_MaxiMum_strength(const  ChildRender_MaxiMum_strength& s)
	{
		this->m_insertMethod = s.m_insertMethod;
		this->m_renderMode = s.m_renderMode;
		this->m_sampleDistance = s.m_sampleDistance;
		this->m_colormethod = s.m_colormethod;
		this->m_gradientMap = s.m_gradientMap;
		this->m_opacity = s.m_opacity;
		this->m_colorBar = s.m_colorBar;
	}
	void toXml(std::shared_ptr<TiXmlElement> pMaxiMumStrengthMethod)
	{
		pMaxiMumStrengthMethod->SetAttribute("insertMethod", (int)m_insertMethod);
		pMaxiMumStrengthMethod->SetAttribute("renderMode", (int)m_renderMode);
		pMaxiMumStrengthMethod->SetAttribute("sampleDistance", m_sampleDistance);
		pMaxiMumStrengthMethod->SetAttribute("colorMethod", (int)m_colormethod);
		pMaxiMumStrengthMethod->SetAttribute("gradientMap", m_gradientMap);
		pMaxiMumStrengthMethod->SetAttribute("opacity", m_opacity);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pMaxiMumStrengthMethod->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pAlphaMethod)
	{
		int ret;
		if (!pAlphaMethod)
			return TIXML_NO_ATTRIBUTE;
		ret = pAlphaMethod->QueryIntAttribute("insertMethod", (int*)&m_insertMethod);
		ret = pAlphaMethod->QueryIntAttribute("rendMode", (int*)&m_renderMode);
		ret = pAlphaMethod->QueryFloatAttribute("sampleDistance", &m_sampleDistance);
		ret = pAlphaMethod->QueryIntAttribute("colorMethod", (int*)&m_colormethod);
		ret = pAlphaMethod->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		ret = pAlphaMethod->QueryFloatAttribute("opacity", &m_opacity);
		TiXmlElement * pColorBar = pAlphaMethod->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		return ret;
	}
};

struct ChildRender_MiniMum_strength
{
	ChildRender_MiniMum_strength() {}
    Child_InsertMethod_VRender m_insertMethod;//插值方法
    RenderMode m_renderMode; //渲染模式
    float m_sampleDistance; //采样距离
    ColorMethod m_colormethod;//着色类型，自定义，渐变色谱
    uint16_t m_gradientMap;//渐变色谱序列号
    float m_opacity;//不透明度
    Child_ColorBar m_colorBar; // 标刻度尺
	ChildRender_MiniMum_strength(const  ChildRender_MiniMum_strength& s)
	{
		this->m_insertMethod = s.m_insertMethod;
		this->m_renderMode = s.m_renderMode;
		this->m_sampleDistance = s.m_sampleDistance;
		this->m_colormethod = s.m_colormethod;
		this->m_gradientMap = s.m_gradientMap;
		this->m_opacity = s.m_opacity;
		this->m_colorBar = s.m_colorBar;
	}
	void toXml(std::shared_ptr<TiXmlElement> pMiniMunStrengthMethod)
	{
		pMiniMunStrengthMethod->SetAttribute("insertMethod", (int)m_insertMethod);
		pMiniMunStrengthMethod->SetAttribute("renderMode", (int)m_renderMode);
		pMiniMunStrengthMethod->SetAttribute("sampleDistance", m_sampleDistance);
		pMiniMunStrengthMethod->SetAttribute("colorMethod", (int)m_colormethod);
		pMiniMunStrengthMethod->SetAttribute("gradientMap", m_gradientMap);
		pMiniMunStrengthMethod->SetAttribute("opacity", m_opacity);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pMiniMunStrengthMethod->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pAlphaMethod)
	{
		int ret;
		if (!pAlphaMethod)
			return TIXML_NO_ATTRIBUTE;
		ret = pAlphaMethod->QueryIntAttribute("insertMethod", (int*)&m_insertMethod);
		ret = pAlphaMethod->QueryIntAttribute("rendMode", (int*)&m_renderMode);
		ret = pAlphaMethod->QueryFloatAttribute("sampleDistance", &m_sampleDistance);
		ret = pAlphaMethod->QueryIntAttribute("colorMethod", (int*)&m_colormethod);
		ret = pAlphaMethod->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		ret = pAlphaMethod->QueryFloatAttribute("opacity", &m_opacity);
		TiXmlElement * pColorBar = pAlphaMethod->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		return ret;
	}
};

struct ChildRender_Average_strength
{
	ChildRender_Average_strength() {}
    Child_InsertMethod_VRender m_insertMethod;//插值方法
    RenderMode m_renderMode; //渲染模式
    float m_sampleDistance; //采样距离
    double m_averageIPScalarRange[2]; //感兴趣标量值范围
    ColorMethod m_colormethod;//着色类型，自定义，渐变色谱
    uint16_t m_gradientMap;//渐变色谱序列号
    float m_opacity;//不透明度
    Child_ColorBar m_colorBar; // 标刻度尺
	ChildRender_Average_strength(const  ChildRender_Average_strength& s)
	{
		this->m_insertMethod = s.m_insertMethod;
		this->m_renderMode = s.m_renderMode;
		this->m_sampleDistance = s.m_sampleDistance;
		this->m_averageIPScalarRange[0] = s.m_averageIPScalarRange[0];
		this->m_averageIPScalarRange[1] = s.m_averageIPScalarRange[1];
		this->m_colormethod = s.m_colormethod;
		this->m_gradientMap = s.m_gradientMap;
		this->m_opacity = s.m_opacity;
		this->m_colorBar = s.m_colorBar;
	}
	void toXml(std::shared_ptr<TiXmlElement> pAverageStrengthMethod)
	{
		pAverageStrengthMethod->SetAttribute("insertMethod", (int)m_insertMethod);
		pAverageStrengthMethod->SetAttribute("renderMode", (int)m_renderMode);
		pAverageStrengthMethod->SetAttribute("sampleDistance", m_sampleDistance);
		pAverageStrengthMethod->SetDoubleAttribute("averageIPScalarRange_1", m_averageIPScalarRange[0]);
		pAverageStrengthMethod->SetDoubleAttribute("averageIPScalarRange_2", m_averageIPScalarRange[1]);
		pAverageStrengthMethod->SetAttribute("colorMethod", (int)m_colormethod);
		pAverageStrengthMethod->SetAttribute("gradientMap", m_gradientMap);
		pAverageStrengthMethod->SetAttribute("opacity", m_opacity);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pAverageStrengthMethod->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pAlphaMethod)
	{
		int ret;
		if (!pAlphaMethod)
			return TIXML_NO_ATTRIBUTE;
		ret = pAlphaMethod->QueryIntAttribute("insertMethod", (int*)&m_insertMethod);
		ret = pAlphaMethod->QueryIntAttribute("rendMode", (int*)&m_renderMode);
		ret = pAlphaMethod->QueryFloatAttribute("sampleDistance", &m_sampleDistance);
		ret = pAlphaMethod->QueryDoubleAttribute("averageIPScalarRange_1", &m_averageIPScalarRange[0]);
		ret = pAlphaMethod->QueryDoubleAttribute("averageIPScalarRange_2", &m_averageIPScalarRange[1]);
		ret = pAlphaMethod->QueryIntAttribute("colorMethod", (int*)&m_colormethod);
		ret = pAlphaMethod->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		ret = pAlphaMethod->QueryFloatAttribute("opacity", &m_opacity);
		TiXmlElement * pColorBar = pAlphaMethod->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		return ret;
	}
};

struct ChildRender_Additive_strength
{
	ChildRender_Additive_strength() {}
    Child_InsertMethod_VRender m_insertMethod;//插值方法
    RenderMode m_renderMode; //渲染模式
    float m_sampleDistance; //采样距离
    ColorMethod m_colormethod;//着色类型，自定义，渐变色谱
    uint16_t m_gradientMap;//渐变色谱序列号
    float m_opacity;//不透明度
    Child_ColorBar m_colorBar; // 标刻度尺
	ChildRender_Additive_strength(const  ChildRender_Additive_strength& s)
	{
		this->m_insertMethod = s.m_insertMethod;
		this->m_renderMode = s.m_renderMode;
		this->m_sampleDistance = s.m_sampleDistance;
		this->m_colormethod = s.m_colormethod;
		this->m_gradientMap = s.m_gradientMap;
		this->m_opacity = s.m_opacity;
		this->m_colorBar = s.m_colorBar;
	}
	void toXml(std::shared_ptr<TiXmlElement> pAdditivestrengthMethod)
	{
		pAdditivestrengthMethod->SetAttribute("insertMethod", (int)m_insertMethod);
		pAdditivestrengthMethod->SetAttribute("renderMode", (int)m_renderMode);
		pAdditivestrengthMethod->SetAttribute("sampleDistance", m_sampleDistance);
		pAdditivestrengthMethod->SetAttribute("colorMethod", (int)m_colormethod);
		pAdditivestrengthMethod->SetAttribute("gradientMap", m_gradientMap);
		pAdditivestrengthMethod->SetAttribute("opacity", m_opacity);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pAdditivestrengthMethod->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pAlphaMethod)
	{
		int ret;
		if (!pAlphaMethod)
			return TIXML_NO_ATTRIBUTE;
		ret = pAlphaMethod->QueryIntAttribute("insertMethod", (int*)&m_insertMethod);
		ret = pAlphaMethod->QueryIntAttribute("rendMode", (int*)&m_renderMode);
		ret = pAlphaMethod->QueryFloatAttribute("sampleDistance", &m_sampleDistance);
		ret = pAlphaMethod->QueryIntAttribute("colorMethod", (int*)&m_colormethod);
		ret = pAlphaMethod->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		ret = pAlphaMethod->QueryFloatAttribute("opacity", &m_opacity);
		TiXmlElement * pColorBar = pAlphaMethod->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		return ret;
	}
};

//union
struct UnionRenderMethod
{
	ChildRender_Alpha alpha_method;//阿尔法混合
    ChildRender_MaxiMum_strength maxiMum_strength_method;//最大强度
    ChildRender_MiniMum_strength miniMum_strength_method;//最小强度
    ChildRender_Average_strength average_strength_method;//平均强度
    ChildRender_Additive_strength additive_strength_method;//叠加强度
	UnionRenderMethod(){}
	UnionRenderMethod(const UnionRenderMethod& s)
	{
		this->alpha_method = s.alpha_method;
		this->maxiMum_strength_method = s.maxiMum_strength_method;
		this->miniMum_strength_method = s.miniMum_strength_method;
		this->average_strength_method = s.average_strength_method;
		this->additive_strength_method = s.additive_strength_method;
	}
	~UnionRenderMethod()
	{

	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pParameters)
	{
		std::shared_ptr<TiXmlElement> pAlphaMethod = std::make_shared<TiXmlElement>("AlphaMethod");
		alpha_method.toXml(pAlphaMethod);
		pParameters->InsertEndChild(*pAlphaMethod);
		std::shared_ptr<TiXmlElement> pMaxiMumStrengthMethod = std::make_shared<TiXmlElement>("MaxiMumStrengthMethod");
		maxiMum_strength_method.toXml(pMaxiMumStrengthMethod);
		pParameters->InsertEndChild(*pMaxiMumStrengthMethod);
		std::shared_ptr<TiXmlElement> pMiniMunStrengthMethod = std::make_shared<TiXmlElement>("MiniMunStrengthMethod");
		miniMum_strength_method.toXml(pMiniMunStrengthMethod);
		pParameters->InsertEndChild(*pMiniMunStrengthMethod);
		std::shared_ptr<TiXmlElement> pAverageStrengthMethod = std::make_shared<TiXmlElement>("AverageStrengthMethod");
		average_strength_method.toXml(pAverageStrengthMethod);
		pParameters->InsertEndChild(*pAverageStrengthMethod);
		std::shared_ptr<TiXmlElement> pAdditivestrengthMethod = std::make_shared<TiXmlElement>("AdditiveStrengthMethod");
		additive_strength_method.toXml(pAdditivestrengthMethod);
		pParameters->InsertEndChild(*pAdditivestrengthMethod);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		TiXmlElement * pAlphaMethod = pParameters->FirstChildElement("AlphaMethod");
		ret = alpha_method.fromXml(pAlphaMethod);
		TiXmlElement * pMaxiMumStrengthMethod = pParameters->FirstChildElement("MaxiMumStrengthMethod");
		ret = maxiMum_strength_method.fromXml(pMaxiMumStrengthMethod);
		TiXmlElement *  pMiniMunStrengthMethod= pParameters->FirstChildElement("MiniMunStrengthMethod");
		ret = miniMum_strength_method.fromXml(pMiniMunStrengthMethod);
		TiXmlElement * pAverageStrengthMethod = pParameters->FirstChildElement("AverageStrengthMethod");
		ret = average_strength_method.fromXml(pAverageStrengthMethod);
		TiXmlElement *pAdditivestrengthMethod  = pParameters->FirstChildElement("AdditiveStrengthMethod");
		ret = additive_strength_method.fromXml(pAdditivestrengthMethod);
		return ret;
	}
};

struct G_VolumeRender : public SuperNodeStruct//形体渲染
{
	/*
	m_changetype = 3:改变渲染方法
	*/
	std::string m_putIn;//输入源
	uint16_t m_putIndex;//输入分量
	//UnionRenderMethod renderMethod;//渲染方法参数
	ChildRender_Average_strength renderMethod;
	RenderMethod m_method;
	G_VolumeRender()
	{
		m_nodeType = NodeType::NODE_G_VOLUMERENDERING_TYPE;
	}
	G_VolumeRender(const G_VolumeRender& s) :SuperNodeStruct(s)//复制构造函数
	{
		m_putIn = s.m_putIn;
		m_putIndex= s.m_putIndex;
		renderMethod= s.renderMethod;
		m_method= s.m_method;
	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pParameters)
	{
		pParameters->SetAttribute("putIn", m_putIn.data());
		pParameters->SetAttribute("putIndex", m_putIndex);
		pParameters->SetAttribute("method", (int)m_method);
		std::shared_ptr<TiXmlElement> pRender = std::make_shared<TiXmlElement>("Render");
			renderMethod.toXml(pRender);
		pParameters->InsertEndChild(*pRender);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryUnsignedAttribute("putIndex", (unsigned int*)&m_putIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("method", (int*)&m_method);
				if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement *pRender = pParameters->FirstChildElement("Render");
		ret= renderMethod.fromXml(pRender);
		return ret;
	}
};

enum Child_insertMethod_ScatterMap
{
	FIXED_VALUE
};

enum Child_Symbol_ScatterMap
{
	DIAMONDS,
	BALL
};


struct Child_labelMark_Scatter
{
	Child_labelMark_Scatter() {}
	bool m_showOn;//是否显示标注
	int m_labelMarkIndex;//标注的字段
	int m_densityIndex;//密度按密度序列号分辨即可，100%，50%，33%~1%
	bool m_showLine;//是否显示引线
	int m_markType;//标注类型，常规属性，规定值，指数
	int m_numCount;//有效数字
	CONSTRUCTOR_MEMCPY(Child_labelMark_Scatter)
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pLabelMark_Scatter)
	{
		pLabelMark_Scatter->SetAttribute("showOn", m_showOn);
		pLabelMark_Scatter->SetAttribute("labelMarkIndex", m_labelMarkIndex);
		pLabelMark_Scatter->SetAttribute("densityIndex", m_densityIndex);
		pLabelMark_Scatter->SetAttribute("showLine", m_showLine);
		pLabelMark_Scatter->SetAttribute("markType", m_markType);
		pLabelMark_Scatter->SetAttribute("numCount", m_numCount);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryBoolAttribute("showOn", &m_showOn);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("labelMarkIndex", &m_labelMarkIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("densityIndex", &m_densityIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryBoolAttribute("showLine", &m_showLine);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("markType", &m_markType);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("numCount", &m_numCount);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Render_ScatterMap
{
	Child_Symbol_ScatterMap m_symbol;//符号，两种
	float m_Symbol_size = 0;//符号大小
	bool m_useRGB = false;//false 则不使用rgb渲染颜色，使用渐变色谱，true则反之
	Child_colorRGB m_rgb;
	uint16_t m_gradientMap;//渐变色谱序列号
	Child_labelMark_Scatter m_labelMark;//标注
	Child_ColorBar m_colorBar;//色标刻度尺
	Child_Render_ScatterMap()
	{
		m_symbol = Child_Symbol_ScatterMap::BALL;
	}
	Child_Render_ScatterMap(const Child_Render_ScatterMap& s) {
		this->m_symbol = s.m_symbol;
		this->m_Symbol_size = s.m_Symbol_size;
		this->m_useRGB = s.m_useRGB;
		this->m_rgb = s.m_rgb;
		this->m_gradientMap = s.m_gradientMap;
		this->m_labelMark = s.m_labelMark;
		this->m_colorBar = s.m_colorBar;
	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pRender)
	{
		pRender->SetAttribute("symbol", (int)m_symbol);
		pRender->SetAttribute("symbolSize", m_Symbol_size);
		pRender->SetAttribute("useRgb", m_useRGB);
		pRender->SetAttribute("color_r", m_rgb.m_r);
		pRender->SetAttribute("color_g", m_rgb.m_g);
		pRender->SetAttribute("color_b", m_rgb.m_b);
		pRender->SetAttribute("gradientMap", m_gradientMap);
		std::shared_ptr<TiXmlElement> pLabelMark_Scatter = std::make_shared<TiXmlElement>("Label_mark_Scatter");
		m_labelMark.toXml(pLabelMark_Scatter);
		pRender->InsertEndChild(*pLabelMark_Scatter);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pRender->InsertEndChild(*pColorBar);
	}
	//cpl 7.5
	int fromXml(TiXmlElement* pParameters)
	{
		int ret;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryIntAttribute("symbol", (int *)&m_symbol);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("symbolSize", &m_Symbol_size);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryBoolAttribute("useRgb", &m_useRGB);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_rgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_rgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_rgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pLabelMark = pParameters->FirstChildElement("Label_mark_Scatter");
		ret = m_labelMark.fromXml(pLabelMark);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pColorBar = pParameters->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_ScatterMap : public SuperNodeStruct  //散点图参数结构体
{
	/*
	m_changetype == 1: 改变符号类型
	m_changetype == 2：改变符号大小
	m_changetype == 3：标注相关改变
	m_changetype == 4：点颜色相关
	m_changetype == 5: colorbar改变相关
	*////

	std::string m_putIn;//输入源
	int m_dataIndex;
	float m_density;//密度，抽稀比例
	Child_insertMethod_ScatterMap m_insertMethod;//插值方式，就一种FIXED_VALUE
	Child_Render_ScatterMap m_render;//渲染参数
	G_ScatterMap()
	{
		m_nodeType = NodeType::NODE_G_SCATTERMAP_TYPE;
		m_dataIndex = 0;
	}
	G_ScatterMap(const G_ScatterMap& s) :SuperNodeStruct(s) 
	{
		this->m_putIn = s.m_putIn;
		this->m_dataIndex = s.m_dataIndex;
		this->m_density = s.m_density;
		this-> m_insertMethod= s.m_insertMethod;
		this->m_render = s.m_render;
	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("dataIndex", m_dataIndex);
		pParameter->SetAttribute("density", m_density);
		pParameter->SetAttribute("insertMethodScatterMap", (int)m_insertMethod);
		std::shared_ptr<TiXmlElement> pRender = std::make_shared<TiXmlElement>("RenderscatterMap");
		m_render.toXml(pRender);
		pParameter->InsertEndChild(*pRender);
	}
	//cpl 7.5
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryIntAttribute("dataIndex", &m_dataIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("density", &m_density);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("insertMethodScatterMap", (int*)&m_insertMethod);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pRender = pParameters->FirstChildElement("RenderscatterMap");
		ret = m_render.fromXml(pRender);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

enum child_draw_style
{
	Shadow_shape,//阴影形状
	Line_shape,//线框形状
	Scatter_shape//绘制点
};

enum Child_Suited_type//未写全，可适当补充
{
	Stretching_On_Surface,
	Clip_To_Surface
};

enum Child_Color_Modulation//未写全，适当补充
{
	Overlay_Color_On_Surface,
        OverlayColorOnly,
	BlendSurfaceandOverlayColors,
	ColorMapOnly
};

//	CONSTRUCTOR_MEMCPY()
struct Child_Render_HighLowMap
{
	Child_Render_HighLowMap() {}
	CONSTRUCTOR_MEMCPY(Child_Render_HighLowMap)
	child_draw_style m_drawStyle;//绘制样式
	float m_opacity;//不透明度
	uint16_t m_gradientMap;//渐变色谱
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pLabelMark_Scatter)
	{
		pLabelMark_Scatter->SetAttribute("drawStyle", m_drawStyle);
		pLabelMark_Scatter->SetAttribute("opacity", m_opacity);
		pLabelMark_Scatter->SetAttribute("gradientMap", m_gradientMap);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryIntAttribute("drawStyle", (int*)&m_drawStyle);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("opacity", &m_opacity);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Map_Overlapped
{
	Child_Map_Overlapped() {}
	CONSTRUCTOR_MEMCPY(Child_Map_Overlapped)
		bool m_showCover;//是否显示覆盖层
	Child_Suited_type m_suitedType;//适合的
	Child_Color_Modulation m_modulation;//颜色调制
	float m_ImageQuality;//图像质量
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pLabelMark_Scatter)
	{
		pLabelMark_Scatter->SetAttribute("showCover", m_showCover);
		pLabelMark_Scatter->SetAttribute("suitedType", m_suitedType);
		pLabelMark_Scatter->SetAttribute("modulation", m_modulation);
		pLabelMark_Scatter->SetAttribute("ImageQuality", m_ImageQuality);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryBoolAttribute("showCover", &m_showCover);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("suitedType", (int*)&m_suitedType);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("modulation", (int*)&m_modulation);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("ImageQuality", &m_ImageQuality);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Vector_Overlay
{
Vector_Overlay() {}
	CONSTRUCTOR_MEMCPY(Vector_Overlay)
	bool m_showLayer;//是否显示图层
	int m_offset;//偏移
	Child_colorRGB m_rgb;//颜色
	float m_opacity;//不透明度
	float m_width;//宽度
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pLabelMark_Scatter)
	{
		pLabelMark_Scatter->SetAttribute("showCover", m_showLayer);
		pLabelMark_Scatter->SetAttribute("offset", m_offset);
		pLabelMark_Scatter->SetAttribute("color_r", m_rgb.m_r);
		pLabelMark_Scatter->SetAttribute("color_g", m_rgb.m_g);
		pLabelMark_Scatter->SetAttribute("color_b", m_rgb.m_b);
		pLabelMark_Scatter->SetAttribute("opacity", m_opacity);
		pLabelMark_Scatter->SetAttribute("width", m_width);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryBoolAttribute("showCover", &m_showLayer);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("offset", &m_offset);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_rgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_rgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_rgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("opacity", &m_opacity);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("width", &m_width);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_Text : public SuperNodeStruct//文本参数
{
	//此结构体参数需全部传递
	std::string m_text;//显示内容
	uint16_t m_fontType;//字体
	uint16_t m_fontSize;//字体大小
	Child_colorRGB m_rgb;//颜色
	PosXYZ m_pos;//位置
	G_Text()
	{
		m_nodeType = NodeType::NODE_G_TEXT_TYPE;
	}
	G_Text(const G_Text& s) :SuperNodeStruct(s)
	{
		//memcpy_struct(this, &s, G_Text);
		m_text = s.m_text;
		m_fontType = s.m_fontType;
		m_fontSize = s.m_fontSize;
		m_rgb= s.m_rgb;
		m_pos= s.m_pos;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("text", m_text.data());
		pParameter->SetAttribute("fontType", m_fontType);
		pParameter->SetAttribute("fontSize", m_fontSize);
		pParameter->SetAttribute("color_r", m_rgb.m_r);
		pParameter->SetAttribute("color_g", m_rgb.m_g);
		pParameter->SetAttribute("color_b", m_rgb.m_b);
		std::shared_ptr<TiXmlElement> pPos = std::make_shared<TiXmlElement>("pos");
		m_pos.toXml(pPos);
		pParameter->InsertEndChild(*pPos);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_text = pParameters->Attribute("text");
		ret = pParameters->QueryUnsignedAttribute("fontType", (unsigned int*)&m_fontType);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("fontType", (unsigned int*)&m_fontSize);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_rgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_rgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_rgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pPos = pParameters->FirstChildElement("pos");
		ret = m_pos.fromXml(pPos);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Render_Orthodogonal
{
Child_Render_Orthodogonal(){}
	CONSTRUCTOR_MEMCPY(Child_Render_Orthodogonal)
	bool m_lightOn;
	float m_opacity;
};

struct G_Orthogonal_Factor : public SuperNodeStruct//正交参数
{
	/*
	m_changeType = 0:输入源//省略
	= 1:输入分量
	= 2:摆放方向    
	= 3: 切片编号
	= 4:着色方法
	= 5:色标刻度尺
	tips: 仅测试上述2，3两项
	*/
	std::string m_putIn;//输入源
	uint16_t m_putIndex;//输入分量
	CutPlaneDirection m_direction;//摆放方向
	int m_setId;
	ColorMethod m_colorMehod;////着色方法
	uint16_t m_gradientMap;//渐变色谱序号，默认一个就行
	Child_ColorBar m_colorBar;//色标刻度尺
	G_Orthogonal_Factor()
	{
		m_nodeType = NodeType::NODE_G_ORTHOGONALFACTOR_TYPE;
	}
	G_Orthogonal_Factor(const G_Orthogonal_Factor& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, G_Orthogonal_Factor);
		this->m_colorBar = s.m_colorBar;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("putIndex", m_putIndex);
		pParameter->SetAttribute("fontSize", m_direction);
		pParameter->SetAttribute("setId", m_setId);
		pParameter->SetAttribute("colorMehod", m_colorMehod);
		pParameter->SetAttribute("gradientMap", m_gradientMap);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pParameter->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryUnsignedAttribute("putIndex", (unsigned int*)&m_putIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("fontSize", (int*)&m_direction);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("setId", &m_setId);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("colorMehod", (int*)&m_colorMehod);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pColorBar = pParameters->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

enum Child_Text_Align
{
	CENTER,
	RIGHT,
	LEFT
};

enum Center_Pos_Align
{
	Left_top,
	Left_bottom,
	Right_top,
	Right_bottom
};

struct G_Comment : public SuperNodeStruct//注解参数
{
	std::string m_text;//文本
	uint16_t m_fontType;//字体
	uint16_t m_fontSize;//字体大小
	Child_Text_Align m_align;//文本对齐
	Child_colorRGB m_rgb;//颜色
	float m_XPos;//X位置
	float m_YPos;//Y位置
	G_Comment()
	{
		m_nodeType = NodeType::NODE_G_COMMENT_TYPE;
	}
	G_Comment(const G_Comment& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, G_Comment);
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("text", m_text.data());
		pParameter->SetAttribute("fontType", m_fontType);
		pParameter->SetAttribute("fontSize", m_fontSize);
		pParameter->SetAttribute("setId", m_align);
		pParameter->SetAttribute("color_r", m_rgb.m_r);
		pParameter->SetAttribute("color_g", m_rgb.m_g);
		pParameter->SetAttribute("color_b", m_rgb.m_b);
		pParameter->SetAttribute("XPos", m_XPos);
		pParameter->SetAttribute("YPos", m_YPos);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_text = pParameters->Attribute("text");
		ret = pParameters->QueryUnsignedAttribute("fontType", (unsigned int*)&m_fontType);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("fontSize", (unsigned int*)&m_fontSize);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("setId", (int*)&m_align);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_rgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_rgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_rgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("XPos", &m_XPos);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("YPos", &m_YPos);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

enum Level_Calculate
{
	AUTO_CALCULATE,//自动计算
	MIN_MAX_CALCULATE,//最小最大值计数
	MIN_MAX_INTERVAL,//最小最大值间隔
	EXPLICIT//显式
};

struct Child_Render_IsoLine
{
	Child_Render_IsoLine(){}
	uint16_t m_lineWidth;//线条宽度
	ColorMethod m_colorMehtod;//着色方法
	uint16_t m_gradientMap;//渐变色谱序列号
	bool display_bound;//显式边框
	uint16_t m_boundWidth;//边框宽度
	Child_colorRGB m_boundRgb;//边框颜色
	//Child_CutingPlane m_cutPlane;//切割平面
	Child_ColorBar m_colorBar;//色标刻度尺
	Child_Render_IsoLine(const Child_Render_IsoLine& s) 
	{
		memcpy_struct(this, &s, Child_Render_IsoLine);
		this->m_colorBar = s.m_colorBar;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("lineWidth", m_lineWidth);
		pParameter->SetAttribute("colorMehtod", m_colorMehtod);
		pParameter->SetAttribute("gradientMap", m_gradientMap);
		pParameter->SetAttribute("display_bound", display_bound);
		pParameter->SetAttribute("boundWidth", m_boundWidth);
		pParameter->SetAttribute("color_r", m_boundRgb.m_r);
		pParameter->SetAttribute("color_g", m_boundRgb.m_g);
		pParameter->SetAttribute("color_b", m_boundRgb.m_b);
		std::shared_ptr<TiXmlElement> pCutPlane = std::make_shared<TiXmlElement>("CutPlane");
		//m_cutPlane.toXml(pCutPlane);
		//pParameter->InsertEndChild(*pCutPlane);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pParameter->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryUnsignedAttribute("lineWidth", (unsigned int*)&m_lineWidth);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("colorMehtod", (int*)&m_colorMehtod);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryBoolAttribute("display_bound", &display_bound);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("boundWidth", (unsigned int*)&m_boundWidth);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_boundRgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_boundRgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_boundRgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		//TiXmlElement * pCutPlane = pParameters->FirstChildElement("CutPlane");
		//ret = m_cutPlane.fromXml(pCutPlane);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pColorBar = pParameters->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct IsoLineTypeAuto {
	double m_minLevel;
	double m_maxLevel;
	int    m_count;
};

struct IsoLineTypeInterval {
	double m_minLevel;
	double m_maxLevel;
	double m_interval;
};

struct IsoLineTypeExplicit {
	double m_levelValue;
};

struct IsoLineType {
	IsoLineTypeAuto m_typeAuto;
	IsoLineTypeInterval m_typeInterval;
	IsoLineTypeAuto     m_typeCount;
	IsoLineTypeExplicit m_typeExplicit;
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		std::shared_ptr<TiXmlElement> pAuto = std::make_shared<TiXmlElement>("auto");
		pAuto->SetDoubleAttribute("minLevel", m_typeAuto.m_minLevel);
		pAuto->SetDoubleAttribute("maxLevel", m_typeAuto.m_maxLevel);
		pAuto->SetAttribute("count", m_typeAuto.m_count);
		pParameter->InsertEndChild(*pAuto);
		std::shared_ptr<TiXmlElement> pInterval = std::make_shared<TiXmlElement>("interval");
		pInterval->SetDoubleAttribute("minLevel", m_typeInterval.m_minLevel);
		pInterval->SetDoubleAttribute("maxLevel", m_typeInterval.m_maxLevel);
		pInterval->SetDoubleAttribute("interval", m_typeInterval.m_interval);
		pParameter->InsertEndChild(*pInterval);
		std::shared_ptr<TiXmlElement> pCount = std::make_shared<TiXmlElement>("count");
		pCount->SetDoubleAttribute("minLevel", m_typeCount.m_minLevel);
		pCount->SetDoubleAttribute("maxLevel", m_typeCount.m_maxLevel);
		pCount->SetDoubleAttribute("count", m_typeCount.m_count);
		pParameter->InsertEndChild(*pCount);
		std::shared_ptr<TiXmlElement> pExplicit = std::make_shared<TiXmlElement>("explicit");
		pExplicit->SetDoubleAttribute("value", m_typeExplicit.m_levelValue);
		pParameter->InsertEndChild(*pExplicit);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		TiXmlElement * pEle;
		pEle = pParameters->FirstChildElement("auto");
		ret=pEle->QueryDoubleAttribute("minLevel", &m_typeAuto.m_minLevel);
		ret=pEle->QueryDoubleAttribute("maxLevel", &m_typeAuto.m_maxLevel);
		ret=pEle->QueryIntAttribute("count", &m_typeAuto.m_count);

		pEle = pParameters->FirstChildElement("interval");
		ret=pEle->QueryDoubleAttribute("minLevel", &m_typeInterval.m_minLevel);
		ret=pEle->QueryDoubleAttribute("maxLevel", &m_typeInterval.m_maxLevel); 
		ret=pEle->QueryDoubleAttribute("interval", &m_typeInterval.m_interval);
		pEle = pParameters->FirstChildElement("count");
		ret=pEle->QueryDoubleAttribute("minLevel", &m_typeCount.m_minLevel);
		ret=pEle->QueryDoubleAttribute("maxLevel", &m_typeCount.m_maxLevel);
		ret=pEle->QueryIntAttribute("count", &m_typeCount.m_count);
		pEle = pParameters->FirstChildElement("explicit");
		ret=pEle->QueryDoubleAttribute("value", &m_typeExplicit.m_levelValue);
		return ret;
	}
};

// m_changetype magic number
#define	G_IsoLine_MN_CT	0x00cc0000
#define	G_Picture_MN_CT	0x00c40000
#define	G_Slice_MN_CT	0x00c50000
struct G_IsoLine : public SuperNodeStruct//等值线参数
{
	std::string m_putIn;//输入源
	Level_Calculate m_level;//层次
	Child_CutingPlane m_cutPlane;
	IsoLineType       m_type;
	Child_Render_IsoLine m_render;//渲染
	G_IsoLine()
	{
		m_nodeType = NodeType::NODE_G_ISOLINE_TYPE;
	}
	G_IsoLine(const G_IsoLine& s) :SuperNodeStruct(s)
	{
		this->m_putIn = s.m_putIn;
		this->m_level = s.m_level;
		this->m_render = s.m_render;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("level", m_level);
		std::shared_ptr<TiXmlElement> pType = std::make_shared<TiXmlElement>("type");
		m_type.toXml(pType);
		pParameter->InsertEndChild(*pType);
		std::shared_ptr<TiXmlElement> pCuttingPlane = std::make_shared<TiXmlElement>("CuttingPlane");
		m_cutPlane.toXml(pCuttingPlane);
		pParameter->InsertEndChild(*pCuttingPlane);
		std::shared_ptr<TiXmlElement> pRender = std::make_shared<TiXmlElement>("m_render");
		m_render.toXml(pRender);
		pParameter->InsertEndChild(*pRender);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryIntAttribute("level", (int*)&m_level);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pType = pParameters->FirstChildElement("type");
		ret = m_type.fromXml(pType);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement* pCuttingPlane = pParameters->FirstChildElement("CuttingPlane");
		ret = m_cutPlane.fromXml(pCuttingPlane);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pRender = pParameters->FirstChildElement("m_render");
		ret = m_render.fromXml(pRender);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_render_isoSurface
{
	Child_render_isoSurface() {}
	ColorMethod m_colorMethod;//着色方式
	uint16_t m_gradientMap;//渐变色谱序列号
	CONSTRUCTOR_MEMCPY(Child_render_isoSurface)
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("colorMethod", m_colorMethod);
		pParameter->SetAttribute("gradientMap", m_gradientMap);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryIntAttribute("colorMethod", (int*)&m_colorMethod);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Materials
{
	Child_Materials(){}
	Child_colorRGB m_HighLight_rgb;//颜色
	float m_HighLight_strength;//高光强度
	float m_glossiness;//光泽度
	float m_opacity;//不透明度
	Child_ColorBar m_colorBar;//色标刻度尺
	Child_Materials(const Child_Materials& s)
	{
		this->m_HighLight_rgb = s.m_HighLight_rgb;
		this->m_HighLight_strength = s.m_HighLight_strength;
		this->m_glossiness = s.m_glossiness;
		this->m_opacity = s.m_opacity;
		this->m_colorBar = s.m_colorBar;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("color_r", m_HighLight_rgb.m_r);
		pParameter->SetAttribute("color_g", m_HighLight_rgb.m_g);
		pParameter->SetAttribute("color_b", m_HighLight_rgb.m_b);
		pParameter->SetAttribute("HighLight_strength", m_HighLight_strength);
		pParameter->SetAttribute("glossiness", m_glossiness);
		pParameter->SetAttribute("opacity", m_opacity);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pParameter->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_HighLight_rgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_HighLight_rgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_HighLight_rgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("HighLight_strength", &m_HighLight_strength);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("glossiness", &m_glossiness);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("opacity", &m_opacity);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pColorBar = pParameters->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_IsoSurface : public SuperNodeStruct//等值面参数
{
	/*
	m_changetype = 1: 临界值
	= 2: 等值面渲染参数
	= 3: 材料
	*/
	std::string m_putIn;//输入源
	uint16_t m_putIndex;//输入分量
	float m_CriticalValue;//临界值
	bool m_isCalculateVolume;//是否计算体积
	float m_volumeBigerThanCriticalValue;//体积大于等于临界值
	float m_volumeSmallerThanCriticalValue;//体积小于等于临界值
	Child_render_isoSurface m_render;//等值面渲染参数
	Child_Materials m_materials;//材料
	G_IsoSurface()
	{
		m_nodeType = NodeType::NODE_G_ISOSURFACE_TYPE;
	}
	G_IsoSurface(const G_IsoSurface& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, G_IsoSurface);
		this->m_materials = s.m_materials;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("putIndex", m_putIndex);
		pParameter->SetAttribute("CriticalValue", m_CriticalValue);
		pParameter->SetAttribute("isCalculateVolume", m_isCalculateVolume);
		pParameter->SetAttribute("volumeBigerThanCriticalValue", m_volumeBigerThanCriticalValue);
		pParameter->SetAttribute("volumeSmallerThanCriticalValue", m_volumeSmallerThanCriticalValue);
		std::shared_ptr<TiXmlElement> pRender = std::make_shared<TiXmlElement>("render");
		m_render.toXml(pRender);
		pParameter->InsertEndChild(*pRender);
		std::shared_ptr<TiXmlElement> pMaterials = std::make_shared<TiXmlElement>("materials");
		m_materials.toXml(pMaterials);
		pParameter->InsertEndChild(*pMaterials);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryUnsignedAttribute("putIndex", (unsigned int*)&m_putIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("CriticalValue", &m_CriticalValue);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryBoolAttribute("isCalculateVolume", &m_isCalculateVolume);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("volumeBigerThanCriticalValue", &m_volumeBigerThanCriticalValue);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("volumeSmallerThanCriticalValue", &m_volumeSmallerThanCriticalValue);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pRender = pParameters->FirstChildElement("render");
		ret = m_render.fromXml(pRender);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pMaterials = pParameters->FirstChildElement("materials");
		ret = m_materials.fromXml(pMaterials);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Render_Surface
{
	Child_Render_Surface() {}
	bool m_displayEdge;//是否显示边缘
	Child_colorRGB m_edgeColor;//边缘颜色
	ColorMethod m_colorMetod;//////着色方法
	uint16_t m_gradientMap;//渐变色谱序列号
	CONSTRUCTOR_MEMCPY(Child_Render_Surface);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("displayEdge", m_displayEdge);
		pParameter->SetAttribute("color_r", m_edgeColor.m_r);
		pParameter->SetAttribute("color_g", m_edgeColor.m_g);
		pParameter->SetAttribute("color_b", m_edgeColor.m_b);
		pParameter->SetAttribute("colorMetod", m_colorMetod);
		pParameter->SetAttribute("gradientMap", m_gradientMap);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryBoolAttribute("displayEdge", &m_displayEdge);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_edgeColor.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_edgeColor.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_edgeColor.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("colorMetod", (int*)&m_colorMetod);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("gradientMap", (unsigned int*)&m_gradientMap);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_coordinate_limit
{
	Child_coordinate_limit() {}
	int m_firstNodeOfAxis;//首节点
	int m_lastNodeOfAxis;//末节点
	bool m_singleSlice;//单个切片
	CONSTRUCTOR_MEMCPY(Child_coordinate_limit);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("firstNodeOfAxis", m_firstNodeOfAxis);
		pParameter->SetAttribute("lastNodeOfAxis", m_lastNodeOfAxis);
		pParameter->SetAttribute("singleSlice", m_singleSlice);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryIntAttribute("firstNodeOfAxis", &m_firstNodeOfAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("lastNodeOfAxis", &m_lastNodeOfAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryBoolAttribute("singleSlice", &m_singleSlice);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_geometric_attribute
{
	Child_geometric_attribute() {}
	Child_coordinate_limit m_xAxis;//X坐标限制
	Child_coordinate_limit m_yAxis;//Y坐标限制
	Child_coordinate_limit m_zAxis;//Z坐标限制
	CONSTRUCTOR_MEMCPY(Child_geometric_attribute);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		std::shared_ptr<TiXmlElement> pxAxis = std::make_shared<TiXmlElement>("xAxis");
		m_xAxis.toXml(pxAxis);
		pParameter->InsertEndChild(*pxAxis);
		std::shared_ptr<TiXmlElement> pyAxis = std::make_shared<TiXmlElement>("yAxis");
		m_yAxis.toXml(pyAxis);
		pParameter->InsertEndChild(*pyAxis);
		std::shared_ptr<TiXmlElement> pzAxis = std::make_shared<TiXmlElement>("zAxis");
		m_zAxis.toXml(pzAxis);
		pParameter->InsertEndChild(*pzAxis);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		TiXmlElement * pxAxis = pParameters->FirstChildElement("xAxis");
		ret = m_xAxis.fromXml(pxAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pyAxis = pParameters->FirstChildElement("yAxis");
		ret = m_yAxis.fromXml(pyAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pzAxis = pParameters->FirstChildElement("zAxis");
		ret = m_zAxis.fromXml(pzAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_SurfaceRender : public SuperNodeStruct//表面渲染参数
{
	/*
	m_changetype = 1:更改输入分量
	= 2：更改渲染参数
	= 3：更改几何参数
	= 4：更改色标刻度尺
	*/
	std::string m_putIn;//输入源
	uint16_t m_putIndex;//输入分量
	int m_blockNum;//块数量
	float m_meanBlock;//平均块数
	bool m_isCalculateVolume;//是否计算体积
	float m_volume;//体积
	Child_Render_Surface m_render_surface;//渲染参数
	Child_geometric_attribute m_geometric;//几何属性
	Child_ColorBar m_colorBar;//色标刻度尺
	G_SurfaceRender()
	{
		m_nodeType = NodeType::NODE_G_SURFACERENDER_TYPE;
	}
	G_SurfaceRender(const G_SurfaceRender& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, G_IsoSurface);
		this->m_colorBar = s.m_colorBar;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("putIndex", m_putIndex);
		pParameter->SetAttribute("blockNum", m_blockNum);
		pParameter->SetAttribute("meanBlock", m_meanBlock);
		pParameter->SetAttribute("isCalculateVolume", m_isCalculateVolume);
		pParameter->SetAttribute("volume", m_volume);
		std::shared_ptr<TiXmlElement> pRender_surface = std::make_shared<TiXmlElement>("render_surface");
		m_render_surface.toXml(pRender_surface);
		pParameter->InsertEndChild(*pRender_surface);
		std::shared_ptr<TiXmlElement> pGeometric = std::make_shared<TiXmlElement>("geometric");
		m_geometric.toXml(pGeometric);
		pParameter->InsertEndChild(*pGeometric);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pParameter->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryUnsignedAttribute("putIndex", (unsigned int*)&m_putIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("blockNum", &m_blockNum);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("meanBlock", &m_meanBlock);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryBoolAttribute("isCalculateVolume", &m_isCalculateVolume);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("volume", &m_volume);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pRender_surface = pParameters->FirstChildElement("render_surface");
		ret = m_render_surface.fromXml(pRender_surface);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pGeometric = pParameters->FirstChildElement("geometric");
		ret = m_geometric.fromXml(pGeometric);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pColorBar = pParameters->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

enum Child_type_LabelMark_Axis
{

};
struct Child_labelMark_Axis
{
	Child_labelMark_Axis(){}
	Child_labelMark_Axis(bool p1,int p2, int p3, int p4, std::string p5, uint16_t p6)
	{
		m_showLabelMark = p1; m_minValue = p2; m_maxValue = p3;
		m_intervalValue = p4; m_format = p5; m_significant_digits = p6;
	}
	bool m_showLabelMark;//显示标注
	int m_minValue;//最小值
	int m_maxValue;//最大值
	int m_intervalValue;//间隔增量
	std::string m_format;//标注格式
	uint16_t m_significant_digits;//有效数字
	CONSTRUCTOR_MEMCPY(Child_labelMark_Axis);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("showLabelMark", m_showLabelMark);
		pParameter->SetAttribute("minValue", m_minValue);
		pParameter->SetAttribute("maxValue", m_maxValue);
		pParameter->SetAttribute("intervalValue", m_intervalValue);
		pParameter->SetAttribute("format", m_format.data());
		pParameter->SetAttribute("significant_digits", m_significant_digits);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryBoolAttribute("showLabelMark", &m_showLabelMark);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("minValue", &m_minValue);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("maxValue", &m_maxValue);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("intervalValue", &m_intervalValue);
		if (TIXML_SUCCESS != ret)	return ret;
		m_format = pParameters->Attribute("format");
		ret = pParameters->QueryUnsignedAttribute("significant_digits", (unsigned int*)&m_significant_digits);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Axis_Axis
{
	Child_Axis_Axis() {}
	Child_Axis_Axis(bool p1, std::string p2, int p3, int p4, Child_colorRGB p5, Child_labelMark_Axis p6)
	{
		m_showAxis = p1; m_textTitle = p2; 
		m_minValue = p3;m_maxValue = p4;
		m_rgb = p5; m_labelmark = p6;
	}
	bool m_showAxis;//显示轴
	std::string m_textTitle;//标题
	int m_minValue;//最小值
	int m_maxValue;//最大值
	Child_colorRGB m_rgb;//颜色
	Child_labelMark_Axis m_labelmark;//标注
	CONSTRUCTOR_MEMCPY(Child_Axis_Axis);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("showAxis", m_showAxis);
		pParameter->SetAttribute("textTitle", m_textTitle.data());
		pParameter->SetAttribute("minValue", m_minValue);
		pParameter->SetAttribute("maxValue", m_maxValue);
		pParameter->SetAttribute("color_r", m_rgb.m_r);
		pParameter->SetAttribute("color_g", m_rgb.m_g);
		pParameter->SetAttribute("color_b", m_rgb.m_b);
		std::shared_ptr<TiXmlElement> pLabelmark = std::make_shared<TiXmlElement>("labelmark");
		m_labelmark.toXml(pLabelmark);
		pParameter->InsertEndChild(*pLabelmark);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryBoolAttribute("showAxis", &m_showAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		m_textTitle = pParameters->Attribute("textTitle");
		ret = pParameters->QueryIntAttribute("minValue", &m_minValue);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("maxValue", &m_maxValue);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_rgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_rgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_rgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pLabelmark = pParameters->FirstChildElement("labelmark");
		ret = m_labelmark.fromXml(pLabelmark);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_Axis : public SuperNodeStruct//轴参数
{
	std::string m_putIn;//输入
	bool m_display_gridLine;//显示网格线
	Child_colorRGB m_LineRgb;//网格线颜色
	uint16_t m_lineWidth;//网格线宽度
	uint16_t m_font;//字体
	bool m_showArrows;//显示箭头
	float m_axisDiameter;//轴粗细（直径）
	uint16_t m_labelMarkSize;//标注大小
	uint16_t m_textSize;//标题尺寸
	Child_Axis_Axis m_xAxis;//X轴参数
	Child_Axis_Axis m_yAxis;//Y轴参数
	Child_Axis_Axis m_zAxis;//Z轴参数
	G_Axis()
	{
		m_nodeType = NodeType::NODE_G_AXIS_TYPE;
	}
	G_Axis(const G_Axis& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, G_Axis);
		this->m_xAxis = s.m_xAxis;
		this->m_yAxis = s.m_yAxis;
		this->m_zAxis = s.m_zAxis;
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("display_gridLine", m_display_gridLine);
		pParameter->SetAttribute("color_r", m_LineRgb.m_r);
		pParameter->SetAttribute("color_g", m_LineRgb.m_g);
		pParameter->SetAttribute("color_b", m_LineRgb.m_b);
		pParameter->SetAttribute("lineWidth", m_lineWidth);
		pParameter->SetAttribute("font", m_font);
		pParameter->SetAttribute("showArrows", m_showArrows);
		pParameter->SetAttribute("axisDiameter", m_axisDiameter);
		pParameter->SetAttribute("labelMarkSize", m_labelMarkSize);
		pParameter->SetAttribute("textSize", m_textSize);
		std::shared_ptr<TiXmlElement> pxAxis = std::make_shared<TiXmlElement>("xAxis");
		m_xAxis.toXml(pxAxis);
		pParameter->InsertEndChild(*pxAxis);
		std::shared_ptr<TiXmlElement> pyAxis = std::make_shared<TiXmlElement>("yAxis");
		m_yAxis.toXml(pyAxis);
		pParameter->InsertEndChild(*pyAxis);
		std::shared_ptr<TiXmlElement> pzAxis = std::make_shared<TiXmlElement>("zAxis");
		m_zAxis.toXml(pzAxis);
		pParameter->InsertEndChild(*pzAxis);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryBoolAttribute("display_gridLine", &m_display_gridLine);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_LineRgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_LineRgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_LineRgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("lineWidth", (unsigned int*)&m_lineWidth);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("font", (unsigned int*)&m_font);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryBoolAttribute("showArrows", &m_showArrows);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("axisDiameter", &m_axisDiameter);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("labelMarkSize", (unsigned int*)&m_labelMarkSize);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("textSize", (unsigned int*)&m_textSize);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pxAxis = pParameters->FirstChildElement("xAxis");
		ret = m_xAxis.fromXml(pxAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pyAxis = pParameters->FirstChildElement("yAxis");
		ret = m_yAxis.fromXml(pyAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pzAxis = pParameters->FirstChildElement("zAxis");
		ret = m_zAxis.fromXml(pzAxis);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Render_boundBox
{
	Child_Render_boundBox() {}
	int m_lineWidth;//线宽
	Child_colorRGB m_rgb;//颜色
	CONSTRUCTOR_MEMCPY(Child_Render_boundBox);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("lineWidth", m_lineWidth);
		pParameter->SetAttribute("color_r", m_rgb.m_r);
		pParameter->SetAttribute("color_g", m_rgb.m_g);
		pParameter->SetAttribute("color_b", m_rgb.m_b);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryIntAttribute("lineWidth", &m_lineWidth);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_r", (unsigned int*)&m_rgb.m_r);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_g", (unsigned int*)&m_rgb.m_g);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryUnsignedAttribute("color_b", (unsigned int*)&m_rgb.m_b);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_BoundingBox : public SuperNodeStruct//边界框参数
{
	std::string m_putIn;//输入源
	PosXYZ m_leftBottom;//左下
	PosXYZ m_rightTop;//右上
	Child_Render_boundBox m_render;//渲染
	Child_labelMark m_labelMark;//标注
	G_BoundingBox()
	{
		m_nodeType = NodeType::NODE_G_BOUNDINGBOX_TYPE;
	}
	G_BoundingBox(const G_BoundingBox& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, G_BoundingBox);
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		std::shared_ptr<TiXmlElement> pLeftBottom = std::make_shared<TiXmlElement>("leftBottom");
		m_leftBottom.toXml(pLeftBottom);
		pParameter->InsertEndChild(*pLeftBottom);
		std::shared_ptr<TiXmlElement> pRightTop = std::make_shared<TiXmlElement>("rightTop");
		m_rightTop.toXml(pRightTop);
		pParameter->InsertEndChild(*pRightTop);
		std::shared_ptr<TiXmlElement> pRender = std::make_shared<TiXmlElement>("render");
		m_render.toXml(pRender);
		pParameter->InsertEndChild(*pRender);
		std::shared_ptr<TiXmlElement> pLabelMark = std::make_shared<TiXmlElement>("labelMark");
		m_labelMark.toXml(pLabelMark);
		pParameter->InsertEndChild(*pLabelMark);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		TiXmlElement * pLeftBottom = pParameters->FirstChildElement("leftBottom");
		ret = m_leftBottom.fromXml(pLeftBottom);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pRightTop = pParameters->FirstChildElement("rightTop");
		ret = m_rightTop.fromXml(pRightTop);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pRender = pParameters->FirstChildElement("render");
		ret = m_render.fromXml(pRender);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pLabelMark = pParameters->FirstChildElement("labelMark");
		ret = m_labelMark.fromXml(pLabelMark);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

enum Child_origin_Point
{
	CENTERPOINT,
	LEFT_BOTTOM,
	RIGHT_TOP
};

struct Child_scaleFactor_conversion
{
	Child_scaleFactor_conversion() {}
	float m_xScaleFactor;
	float m_yScaleFactor;
	float m_zScaleFactor;
	CONSTRUCTOR_MEMCPY(Child_scaleFactor_conversion);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("xScaleFactor", m_xScaleFactor);
		pParameter->SetAttribute("yScaleFactor", m_yScaleFactor);
		pParameter->SetAttribute("zScaleFactor", m_zScaleFactor);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryFloatAttribute("xScaleFactor", &m_xScaleFactor);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("yScaleFactor", &m_yScaleFactor);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("zScaleFactor", &m_zScaleFactor);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Rotate_Conversion
{
	Child_Rotate_Conversion() {}
	float m_xRotate;
	float m_yRotate;
	float m_zRotate;
	float m_angle;
	CONSTRUCTOR_MEMCPY(Child_Rotate_Conversion);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("xRotate", m_xRotate);
		pParameter->SetAttribute("yRotate", m_yRotate);
		pParameter->SetAttribute("zRotate", m_zRotate);
		pParameter->SetAttribute("angle", m_angle);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryFloatAttribute("xRotate", &m_xRotate);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("yRotate", &m_yRotate);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("zRotate", &m_zRotate);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("angle", &m_angle);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Exchange_Conversion
{
	Child_Exchange_Conversion() {}
	float m_xExchange;
	float m_yExchange;
	float m_zExchange;
	CONSTRUCTOR_MEMCPY(Child_Exchange_Conversion);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("xExchange", m_xExchange);
		pParameter->SetAttribute("yExchange", m_yExchange);
		pParameter->SetAttribute("zExchange", m_zExchange);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryFloatAttribute("xExchange", &m_xExchange);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("yExchange", &m_yExchange);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("zExchange", &m_zExchange);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Conversion_Attribute
{
	Child_Conversion_Attribute() {}
	Child_scaleFactor_conversion m_scaleFactor;
	Child_Rotate_Conversion m_rotate;
	Child_Exchange_Conversion m_exchange;
	CONSTRUCTOR_MEMCPY(Child_Conversion_Attribute);
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		std::shared_ptr<TiXmlElement> pScaleFactor = std::make_shared<TiXmlElement>("scaleFactor");
		m_scaleFactor.toXml(pScaleFactor);
		pParameter->InsertEndChild(*pScaleFactor);
		std::shared_ptr<TiXmlElement> protate = std::make_shared<TiXmlElement>("rotate");
		m_rotate.toXml(protate);
		pParameter->InsertEndChild(*protate);
		std::shared_ptr<TiXmlElement> pexchange = std::make_shared<TiXmlElement>("exchange");
		m_exchange.toXml(pexchange);
		pParameter->InsertEndChild(*pexchange);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		TiXmlElement * pScaleFactor = pParameters->FirstChildElement("scaleFactor");
		ret = m_scaleFactor.fromXml(pScaleFactor);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * protate = pParameters->FirstChildElement("rotate");
		ret = m_rotate.fromXml(protate);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pexchange = pParameters->FirstChildElement("exchange");
		ret = m_exchange.fromXml(pexchange);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct C_Conversion : public SuperNodeStruct
{
	C_Conversion() {
		m_nodeType = NodeType::NODE_C_CONVERSION_TYPE;
	}
	std::string m_putIn;
	Child_origin_Point m_origin;
	PosXYZ m_pos;
	Child_Conversion_Attribute m_conversionAttribute;
	C_Conversion(const C_Conversion& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, C_Conversion);
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("origin", m_origin);
		std::shared_ptr<TiXmlElement> pPos = std::make_shared<TiXmlElement>("pos");
		m_pos.toXml(pPos);
		pParameter->InsertEndChild(*pPos);
		std::shared_ptr<TiXmlElement> pConversionAttribute = std::make_shared<TiXmlElement>("conversionAttribute");
		m_conversionAttribute.toXml(pConversionAttribute);
		pParameter->InsertEndChild(*pConversionAttribute);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryIntAttribute("origin", (int*)&m_origin);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pPos = pParameters->FirstChildElement("pos");
		ret = m_pos.fromXml(pPos);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pConversionAttribute = pParameters->FirstChildElement("conversionAttribute");
		ret = m_conversionAttribute.fromXml(pConversionAttribute);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct C_EliminationFiler : public SuperNodeStruct
{
	C_EliminationFiler() {
		m_nodeType = NodeType::NODE_C_ELIMINATIONFILTER_TYPE;
	}
	std::string m_putIn;
	std::string m_eliminationStr;
	C_EliminationFiler(const C_EliminationFiler& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, C_EliminationFiler);
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("eliminationStr", m_eliminationStr.data());
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		m_eliminationStr = pParameters->Attribute("eliminationStr");
		return ret;
	}
};

/*enum Child_InsertMethod_Mesh
{
	INVERSE_DISTANCE,
	MASS_DATA,
	LOCAL_POLYNOMIAL
};*/

enum Child_Anisotropic
{
	ANISOTROPIC,
	ISOTROPY,
	GENERAL_PROPERTIES
};

/*struct Child_Inverse_distance_Mesh
{
	Child_Anisotropic m_anisotropic;
	float m_weightedNum;
	float m_evennessNum;
};*/

/*struct Child_MassData_Mesh
{
	uint16_t m_MeasureType;
};*/

/*struct Child_LocalPolyNomail_Mesh
{
	uint16_t m_order;
	uint16_t m_weightValue;
};*/

/*union InsertMethodParameter
{
	Child_Inverse_distance_Mesh m_InverseDistance;
	Child_MassData_Mesh m_MassData;
	Child_LocalPolyNomail_Mesh m_localPolyNomail;
};*/

enum Child_insert_Method_Mesh
{
	INVERSEDISTANCE,
	DATAMETRIC,
	KRIGING,
};

struct InverseDistance
{
	InverseDistance() {}
	CONSTRUCTOR_MEMCPY(InverseDistance)
	float smooth;
	int order;
	//这里是不是少了各向异性
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("smooth", smooth);
		pParameter->SetAttribute("order", order);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryFloatAttribute("smooth", &smooth);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("order", &order);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct DataMetric//这个不做
{
	//这个就是找最近点做统计学操作 wei实现
};

struct Kriging
{
	Kriging() {}
	CONSTRUCTOR_MEMCPY(Kriging)
	int mode; //变差函数模型 1 2 3
	double c0; //块金
	double c1; //基台值
	double a; //变程
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("mode", mode);
		pParameter->SetDoubleAttribute("c0", c0);
		pParameter->SetDoubleAttribute("c1", c1);
		pParameter->SetDoubleAttribute("a", a);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryIntAttribute("mode", &mode);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("c0", &c0);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("c1", &c1);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("a", &a);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Child_Method_Type
{
	Child_Method_Type() {}
	CONSTRUCTOR_MEMCPY(Child_Method_Type)
	InverseDistance m_idw;//反距离
	DataMetric m_metric;//
	Kriging m_kriging;//克里金
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		std::shared_ptr<TiXmlElement> pIdw = std::make_shared<TiXmlElement>("idw");
		m_idw.toXml(pIdw);
		pParameter->InsertEndChild(*pIdw);
		std::shared_ptr<TiXmlElement> pKriging = std::make_shared<TiXmlElement>("kriging");
		m_kriging.toXml(pKriging);
		pParameter->InsertEndChild(*pKriging);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		TiXmlElement * pIdw = pParameters->FirstChildElement("idw");
		ret = m_idw.fromXml(pIdw);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pKriging = pParameters->FirstChildElement("kriging");
		ret = m_kriging.fromXml(pKriging);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Geometry_mesh
{
	Geometry_mesh() {}
	CONSTRUCTOR_MEMCPY(Geometry_mesh)
	double limits[6]; //Xmin,Ymin,Zmin,Xmax,Ymax,Zmax 

	int resolution[3]; //解析度

	double space[3]; //网格点间隔
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetDoubleAttribute("limits0", limits[0]);
		pParameter->SetDoubleAttribute("limits1", limits[1]);
		pParameter->SetDoubleAttribute("limits2", limits[2]);
		pParameter->SetDoubleAttribute("limits3", limits[3]);
		pParameter->SetDoubleAttribute("limits4", limits[4]);
		pParameter->SetDoubleAttribute("limits5", limits[5]);
		pParameter->SetAttribute("resolution0", resolution[0]);
		pParameter->SetAttribute("resolution1", resolution[1]);
		pParameter->SetAttribute("resolution2", resolution[2]);
		pParameter->SetDoubleAttribute("space0", space[0]);
		pParameter->SetDoubleAttribute("space1", space[1]);
		pParameter->SetDoubleAttribute("space2", space[2]);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryDoubleAttribute("limits0", &limits[0]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("limits1", &limits[1]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("limits2", &limits[2]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("limits3", &limits[3]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("limits4", &limits[4]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("limits5", &limits[5]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("resolution0", &resolution[0]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("resolution1", &resolution[1]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("resolution2", &resolution[2]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("space0", &space[0]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("space1", &space[1]);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryDoubleAttribute("space2", &space[2]);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

enum SearchType
{
	SEARCH_AllDATA,
	SEARCH_SIMPLE,
	SEARCH_ANISOTROPIC,
};

struct Search_AllData
{

};

struct Search_Simple
{
	Search_Simple(){}
	CONSTRUCTOR_MEMCPY(Search_Simple)
	float Radius;
	int MaxCount;
	int MinCount;
	//是不是少了最小数量
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("Radius", Radius);
		pParameter->SetAttribute("MaxCount", MaxCount);
		pParameter->SetAttribute("MinCount", MinCount);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryFloatAttribute("Radius", &Radius);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("MaxCount", &MaxCount);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("MinCount", &MinCount);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Search_AnisoTropic
{
	Search_AnisoTropic(){}
	CONSTRUCTOR_MEMCPY(Search_AnisoTropic)
	float SearchEllipse_x;
	float SearchEllipse_y;
	float SearchEllipse_z;
	int MaxCount;
	int MinCount;
	//是不是少了最小数量
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("SearchEllipse_x", SearchEllipse_x);
		pParameter->SetAttribute("SearchEllipse_y", SearchEllipse_y);
		pParameter->SetAttribute("SearchEllipse_z", SearchEllipse_z);
		pParameter->SetAttribute("MaxCount", MaxCount);
		pParameter->SetAttribute("MinCount", MinCount);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		ret = pParameters->QueryFloatAttribute("SearchEllipse_x", &SearchEllipse_x);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("SearchEllipse_y", &SearchEllipse_y);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("SearchEllipse_z", &SearchEllipse_z);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("MaxCount", &MaxCount);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("MinCount", &MinCount);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct Search_TYpe
{
	Search_TYpe(){}
	CONSTRUCTOR_MEMCPY(Search_TYpe)
	Search_AllData m_alldata;
	Search_Simple m_simple;
	Search_AnisoTropic m_anisotropic;
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		std::shared_ptr<TiXmlElement> pKriging = std::make_shared<TiXmlElement>("m_simple");
		m_simple.toXml(pKriging);
		pParameter->InsertEndChild(*pKriging);
		std::shared_ptr<TiXmlElement> pAnisotropic = std::make_shared<TiXmlElement>("anisotropic");
		m_anisotropic.toXml(pAnisotropic);
		pParameter->InsertEndChild(*pAnisotropic);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		TiXmlElement * pKriging = pParameters->FirstChildElement("m_simple");
		ret = m_simple.fromXml(pKriging);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pAnisotropic = pParameters->FirstChildElement("anisotropic");
		ret = m_anisotropic.fromXml(pAnisotropic);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct C_Mesh : public SuperNodeStruct
{
	std::string m_putIn; //输入
	int m_dataIndex;
	int m_putInPointNum;//输入点数
	Child_insert_Method_Mesh m_MethosType;//插值方法
	Child_Method_Type m_method_type;//对应插值方法参数
	Geometry_mesh m_geometry;//几何属性
	SearchType m_SearchType;//搜索类型
	Search_TYpe m_search_type;//搜索类型参数
	C_Mesh()
	{
		m_nodeType = NodeType::NODE_C_MESH_TYPE;
		m_dataIndex = 0;
	}
	C_Mesh(const C_Mesh& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, C_Mesh);
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("dataIndex", m_dataIndex);
		pParameter->SetAttribute("putInPointNum", m_putInPointNum);
		pParameter->SetAttribute("MethosType", m_MethosType);
		pParameter->SetAttribute("SearchType", m_SearchType);
		std::shared_ptr<TiXmlElement> pMethod_type = std::make_shared<TiXmlElement>("method_type");
		m_method_type.toXml(pMethod_type);
		pParameter->InsertEndChild(*pMethod_type);
		std::shared_ptr<TiXmlElement> pGeometry = std::make_shared<TiXmlElement>("geometry");
		m_geometry.toXml(pGeometry);
		pParameter->InsertEndChild(*pGeometry);
		std::shared_ptr<TiXmlElement> pSearch_type = std::make_shared<TiXmlElement>("search_type");
		m_search_type.toXml(pSearch_type);
		pParameter->InsertEndChild(*pSearch_type);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryIntAttribute("dataIndex", &m_dataIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("putInPointNum", &m_putInPointNum);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("MethosType", (int*)&m_MethosType);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("SearchType", (int*)&m_SearchType);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pMethod_type = pParameters->FirstChildElement("method_type");
		ret = m_method_type.fromXml(pMethod_type);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pgeometry = pParameters->FirstChildElement("geometry");
		ret = m_geometry.fromXml(pgeometry);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pSearch_type = pParameters->FirstChildElement("search_type");
		ret = m_search_type.fromXml(pSearch_type);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct C_RepeatFilter : public SuperNodeStruct
{
	C_RepeatFilter() {
		m_nodeType = NodeType::NODE_C_REPEATFILER;
	}
	std::string m_putIn;
	float m_Xtolerance;
	float m_Ytolerance;
	float m_Ztolerance;
	C_RepeatFilter(const C_RepeatFilter& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, C_RepeatFilter);
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("Xtolerance", m_Xtolerance);
		pParameter->SetAttribute("Ytolerance", m_Ytolerance);
		pParameter->SetAttribute("Ztolerance", m_Ztolerance);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryFloatAttribute("Xtolerance", &m_Xtolerance);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("Ytolerance", &m_Ytolerance);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryFloatAttribute("Ztolerance", &m_Ztolerance);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct C_SubsetFilter : public SuperNodeStruct
{
	C_SubsetFilter() {
		m_nodeType = NodeType::NODE_C_SUBSET_TYPE;
	}
	std::string m_putIn;
	int m_xStart;
	int m_xEnd;
	int m_xStep;
	int m_yStart;
	int m_yEnd;
	int m_yStep;
	int m_zStart;
	int m_zEnd;
	int m_zStep;
	C_SubsetFilter(const C_SubsetFilter& s) :SuperNodeStruct(s)
	{
		memcpy_struct(this, &s, C_SubsetFilter);
	}
	//cpl 7.5
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());

		pParameter->SetAttribute("xStart", m_xStart);
		pParameter->SetAttribute("xEnd", m_xEnd);
		pParameter->SetAttribute("xStep", m_xStep);
		pParameter->SetAttribute("yStart", m_yStart);
		pParameter->SetAttribute("yEnd", m_yEnd);
		pParameter->SetAttribute("yStep", m_yStep);
		pParameter->SetAttribute("zStart", m_zStart);
		pParameter->SetAttribute("zEnd", m_zEnd);
		pParameter->SetAttribute("zStep", m_zStep);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		
		ret = pParameters->QueryIntAttribute("xStart", &m_xStart);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("xEnd", &m_xEnd);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("xStep", &m_xStep);
		if (TIXML_SUCCESS != ret)	return ret;

		ret = pParameters->QueryIntAttribute("yStart", &m_yStart);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("yEnd", &m_yEnd);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("yStep", &m_yStep);
		if (TIXML_SUCCESS != ret)	return ret;

		ret = pParameters->QueryIntAttribute("zStart", &m_zStart);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("zEnd", &m_zEnd);
		if (TIXML_SUCCESS != ret)	return ret;
		ret = pParameters->QueryIntAttribute("zStep", &m_zStep);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_HighLowZoneMap : public SuperNodeStruct
{
	std::string m_putIn;//输入网格点阵
	//xxx   输入覆盖叠加图？//这个没明白啥意思，你看下补充下
	uint16_t m_putIndex;//输入分量
	//CutPlaneDirection m_direction;//摆放方向  三维数据输入特有
	//int m_setionId;//切片id  三维数据输入特有
	float m_scaleFactor;//比例因子
	std::string      demFile;            
	std::string      textureFile;        //纹理照片
	Child_Render_HighLowMap m_renderInfo;//高低区域图渲染信息
	Child_Map_Overlapped m_mapOverlapped;//图像叠加
	Vector_Overlay m_verctorOverlay;//矢量图覆盖
	Child_ColorBar m_colorBar;//色标刻度尺
	G_HighLowZoneMap()
	{
		m_nodeType = NodeType::NODE_G_HIGHANDLOWDIAGRAM_TYPE;
	}
	G_HighLowZoneMap(const G_HighLowZoneMap& other) :SuperNodeStruct(other){
		m_nodeType = NodeType::NODE_G_HIGHANDLOWDIAGRAM_TYPE;
		m_putIn = other.m_putIn;
		m_putIndex = other.m_putIndex;
		//m_direction = other.m_direction;
		//m_setionId = other.m_setionId;
		m_scaleFactor = other.m_scaleFactor;

		demFile = other.demFile;
		textureFile = other.textureFile;

		m_renderInfo.m_drawStyle = other.m_renderInfo.m_drawStyle;
		m_renderInfo.m_opacity = other.m_renderInfo.m_opacity;

		m_mapOverlapped.m_showCover = other.m_mapOverlapped.m_showCover;
		m_mapOverlapped.m_modulation = other.m_mapOverlapped.m_modulation;
		m_mapOverlapped.m_suitedType = other.m_mapOverlapped.m_suitedType;
	}

	G_HighLowZoneMap& operator=(const G_HighLowZoneMap& other) {
		m_putIn = other.m_putIn;
		m_putIndex = other.m_putIndex;
		//m_direction = other.m_direction;
		//m_setionId = other.m_setionId;
		m_scaleFactor = other.m_scaleFactor;

		demFile = other.demFile;
		textureFile = other.textureFile;

		m_renderInfo.m_drawStyle = other.m_renderInfo.m_drawStyle;
		m_renderInfo.m_opacity = other.m_renderInfo.m_opacity;

		m_mapOverlapped.m_showCover = other.m_mapOverlapped.m_showCover;
		m_mapOverlapped.m_modulation = other.m_mapOverlapped.m_modulation;
		m_mapOverlapped.m_suitedType = other.m_mapOverlapped.m_suitedType;
		return *this;
	}
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("putIndex", m_putIndex);
		pParameter->SetAttribute("demFile", demFile.data());
		pParameter->SetAttribute("textureFile", textureFile.data());
		pParameter->SetAttribute("scaleFactor", m_scaleFactor);
		std::shared_ptr<TiXmlElement> pRenderInfo = std::make_shared<TiXmlElement>("renderInfo");
		m_renderInfo.toXml(pRenderInfo);
		pParameter->InsertEndChild(*pRenderInfo);
		std::shared_ptr<TiXmlElement> pMapOverlapped = std::make_shared<TiXmlElement>("mapOverlapped");
		m_mapOverlapped.toXml(pMapOverlapped);
		pParameter->InsertEndChild(*pMapOverlapped);
		std::shared_ptr<TiXmlElement> pVerctorOverlay = std::make_shared<TiXmlElement>("verctorOverlay");
		m_verctorOverlay.toXml(pVerctorOverlay);
		pParameter->InsertEndChild(*pVerctorOverlay);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pParameter->InsertEndChild(*pColorBar);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		ret = pParameters->QueryUnsignedAttribute("putIndex", (unsigned int*)&m_putIndex);
		if (TIXML_SUCCESS != ret)	return ret;
		demFile = pParameters->Attribute("demFile");
		textureFile = pParameters->Attribute("textureFile");
		ret = pParameters->QueryFloatAttribute("scaleFactor", &m_scaleFactor);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pRenderInfo = pParameters->FirstChildElement("renderInfo");
		ret = m_renderInfo.fromXml(pRenderInfo);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pMapOverlapped = pParameters->FirstChildElement("mapOverlapped");
		ret = m_mapOverlapped.fromXml(pMapOverlapped);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pVerctorOverlay = pParameters->FirstChildElement("verctorOverlay");
		ret = m_verctorOverlay.fromXml(pVerctorOverlay);
		if (TIXML_SUCCESS != ret)	return ret;
		TiXmlElement * pColorBar = pParameters->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret)	return ret;
		return ret;
	}
};

struct G_Picture : public SuperNodeStruct
{          
	std::string      picFilePath;        //图片文件路径
	//int			picScale;			// /100
	int  xy_range[6];
	float  xyz_offset[4];
	float  xyz_scale[3];
	bool apply;
	G_Picture()
	{
		m_nodeType = NodeType::NODE_G_PICTURE_TYPE;
	}
	G_Picture(const G_Picture& other) :SuperNodeStruct(other)
	{
		memcpy_struct(this, &other, G_Picture);
	}

	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("picFilePath", picFilePath.data());
		//pParameter->SetAttribute("picScale", picScale);
		pParameter->SetAttribute("xStart", xy_range[0]);
		pParameter->SetAttribute("xEnd", xy_range[1]);
		pParameter->SetAttribute("yStart", xy_range[2]);
		pParameter->SetAttribute("yEnd", xy_range[3]);
		pParameter->SetAttribute("xOffset", xyz_offset[0]);
		pParameter->SetAttribute("yOffset", xyz_offset[1]);
		pParameter->SetAttribute("zOffset", xyz_offset[2]);
		pParameter->SetAttribute("zRoate", xyz_offset[3]);
		pParameter->SetAttribute("xScale", xyz_scale[0]);
		pParameter->SetAttribute("yScale", xyz_scale[1]);
		pParameter->SetAttribute("zScale", xyz_scale[2]);
		pParameter->SetAttribute("apply", apply);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		picFilePath = pParameters->Attribute("picFilePath");
		//ret = pParameters->QueryIntAttribute("picScale", &picScale);
		ret = pParameters->QueryIntAttribute("xStart", &xy_range[0]);
		ret = pParameters->QueryIntAttribute("xEnd", &xy_range[1]);
		ret = pParameters->QueryIntAttribute("yStart", &xy_range[2]);
		ret = pParameters->QueryIntAttribute("yEnd", &xy_range[3]);
		ret = pParameters->QueryFloatAttribute("xOffset", &xyz_offset[0]);
		ret = pParameters->QueryFloatAttribute("yOffset", &xyz_offset[1]);
		ret = pParameters->QueryFloatAttribute("zOffset", &xyz_offset[2]);
		ret = pParameters->QueryFloatAttribute("zRotate", &xyz_offset[3]);
		ret = pParameters->QueryFloatAttribute("xScale", &xyz_scale[0]);
		ret = pParameters->QueryFloatAttribute("yScale", &xyz_scale[1]);
		ret = pParameters->QueryFloatAttribute("zScale", &xyz_scale[2]);
		ret = pParameters->QueryBoolAttribute("apply", &apply);
		return ret;
	}
};

struct G_Geotiff : public SuperNodeStruct
{
	std::string      picFilePath;        //图片文件路径
	int			adopt;
	int  xy_range[6];
	float  xyz_offset[4];
	float  xyz_scale[3];
	bool apply;
	G_Geotiff()
	{
		m_nodeType = NodeType::NODE_G_GEOTIFF_TYPE;
	}
	G_Geotiff(const G_Geotiff& other) :SuperNodeStruct(other)
	{
		memcpy_struct(this, &other, G_Geotiff);
	}

	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("picFilePath", picFilePath.data());
		pParameter->SetAttribute("adopt", adopt);
		pParameter->SetAttribute("xStart", xy_range[0]);
		pParameter->SetAttribute("xEnd", xy_range[1]);
		pParameter->SetAttribute("yStart", xy_range[2]);
		pParameter->SetAttribute("yEnd", xy_range[3]);
		pParameter->SetAttribute("opacity", xy_range[5]);
		pParameter->SetAttribute("xOffset", xyz_offset[0]);
		pParameter->SetAttribute("yOffset", xyz_offset[1]);
		pParameter->SetAttribute("zOffset", xyz_offset[2]);
		pParameter->SetAttribute("zRoate", xyz_offset[3]);
		pParameter->SetAttribute("xScale", xyz_scale[0]);
		pParameter->SetAttribute("yScale", xyz_scale[1]);
		pParameter->SetAttribute("zScale", xyz_scale[2]);
		pParameter->SetAttribute("apply", apply);
	}
	int fromXml(TiXmlElement* pParameters)
	{
		int ret = 0;
		if (!pParameters)
			return TIXML_NO_ATTRIBUTE;
		m_putIn = pParameters->Attribute("putIn");
		picFilePath = pParameters->Attribute("picFilePath");
		ret = pParameters->QueryIntAttribute("adopt", &adopt);
		ret = pParameters->QueryIntAttribute("xStart", &xy_range[0]);
		ret = pParameters->QueryIntAttribute("xEnd", &xy_range[1]);
		ret = pParameters->QueryIntAttribute("yStart", &xy_range[2]);
		ret = pParameters->QueryIntAttribute("yEnd", &xy_range[3]);
		ret = pParameters->QueryIntAttribute("opacity", &xy_range[5]);
		ret = pParameters->QueryFloatAttribute("xOffset", &xyz_offset[0]);
		ret = pParameters->QueryFloatAttribute("yOffset", &xyz_offset[1]);
		ret = pParameters->QueryFloatAttribute("zOffset", &xyz_offset[2]);
		ret = pParameters->QueryFloatAttribute("zRotate", &xyz_offset[3]);
		ret = pParameters->QueryFloatAttribute("xScale", &xyz_scale[0]);
		ret = pParameters->QueryFloatAttribute("yScale", &xyz_scale[1]);
		ret = pParameters->QueryFloatAttribute("zScale", &xyz_scale[2]);
		ret = pParameters->QueryBoolAttribute("apply", &apply);
		return ret;
	}
};

struct G_Curve_Slice : public SuperNodeStruct
{
	std::string m_putIn;//输入源
	uint16_t m_putIndex;//输入分量
	bool m_lightOn;//照明
	std::string      picFilePath;        //图片文件路径
	double m_opacityValue;//不透明度
	ColorMethod m_colorMethod;//着色方法
	uint16_t m_gradientMap;//渐变色谱序列号
	Child_CutingPlane m_cutPlane;//切割平面
	Child_ColorBar m_colorBar;//色标刻度尺
	G_Curve_Slice()
	{
		m_nodeType = NodeType::NODE_G_CURVE_SLICE_TYPE;
	}
	G_Curve_Slice(const G_Curve_Slice& s) :SuperNodeStruct(s)
	{
		this->m_putIn = s.m_putIn;
		this->m_putIndex = s.m_putIndex;
		this->m_lightOn = s.m_lightOn;
		this->m_opacityValue = s.m_opacityValue;
		this->m_colorMethod = s.m_colorMethod;
		this->m_gradientMap = s.m_gradientMap;
		this->m_cutPlane = s.m_cutPlane;
		this->m_colorBar = s.m_colorBar;
	}
	// yzw 0701
	void toXml(std::shared_ptr<TiXmlElement> pParameter)
	{
		pParameter->SetAttribute("putIn", m_putIn.data());
		pParameter->SetAttribute("putIndex", m_putIndex);
		pParameter->SetAttribute("lightOn", m_lightOn);
		pParameter->SetAttribute("picFilePath", picFilePath.data());
		pParameter->SetDoubleAttribute("opacityValue", m_opacityValue);
		pParameter->SetAttribute("colorMethod", (int)m_colorMethod);
		pParameter->SetAttribute("gradientMap", m_gradientMap);
		std::shared_ptr<TiXmlElement> pCuttingPlane = std::make_shared<TiXmlElement>("CuttingPlane");
		m_cutPlane.toXml(pCuttingPlane);
		pParameter->InsertEndChild(*pCuttingPlane);
		std::shared_ptr<TiXmlElement> pColorBar = std::make_shared<TiXmlElement>("ColorBar");
		m_colorBar.toXml(pColorBar);
		pParameter->InsertEndChild(*pColorBar);
	}
	//cpl 7.4
	int fromXml(TiXmlElement* pParameter)
	{
		m_putIn = pParameter->Attribute("putIn");
		int index = 0;
		int ret = pParameter->QueryIntAttribute("putIndex", &index);
		m_putIndex = index;
		ret = pParameter->QueryBoolAttribute("lightOn", &m_lightOn);
		picFilePath = pParameter->Attribute("picFilePath");
		ret = pParameter->QueryDoubleAttribute("opacityValue", &m_opacityValue);
		ret = pParameter->QueryIntAttribute("colorMethod", (int *)&m_colorMethod);
		ret = pParameter->QueryIntAttribute("gradientMap", &index);
		m_gradientMap = index;
		TiXmlElement* pCuttingPlane = pParameter->FirstChildElement("CuttingPlane");
		ret = m_cutPlane.fromXml(pCuttingPlane);
		TiXmlElement* pColorBar = pParameter->FirstChildElement("ColorBar");
		ret = m_colorBar.fromXml(pColorBar);
		if (TIXML_SUCCESS != ret) return ret;
		return TIXML_SUCCESS;
	}
};

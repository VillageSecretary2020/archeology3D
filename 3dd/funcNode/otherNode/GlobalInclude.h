/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
//
// 文件功能描述：
// GlobalInclude.h
//
// 创建者：tangAo
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once
#include <iostream>
#include <list>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <tinyxml.h>
#include <memory>
#include <mutex>

#include "str_lang.h"

typedef int ProjectError;
typedef int LogicError;

#define memcpy_struct(pd, ps, T) memcpy_s(pd, sizeof(T), ps, sizeof(T))
// 浅拷贝构造函数
#define	CONSTRUCTOR_MEMCPY(T)	T(const T& s) {	memcpy_s(this, sizeof(T), &s,  sizeof(T)) ;	}

#define RETURN_NORMAL  0
#define RETURN_ERROR   -1


enum LogicErrorType
{
	ERROR_INIT_PROJECT_FROMXML = -10001,
};

enum ProjectErrorType
{
	ERROR_PARSE_XML_PREID_LIST = -900,
	ERROR_PARSE_XML_NEXTID_LIST,
	ERROR_PARSE_MESH_PARAMETER,
	ERROR_WRITE_MESH_PARAMETER,
	ERROR_NO_PATH_TO_SAVE,
	ERROR_XML_PARSE_FAILED,
	ERROR_XML_SAVE_FAILED,
	ERROR_XML_NO_NODE,
	ERROR_XML_PARSE_NODE,
};

enum NodeType
{
	NODE_NODEFINE_TYPE,
	NODE_D_DATASOURCE_TYPE,
	NODE_D_RESERVED_2,
	NODE_D_RESERVED_3,
	NODE_D_RESERVED_4,
	NODE_D_RESERVED_5,
	NODE_D_RESERVED_6,
	NODE_D_RESERVED_7,
	NODE_D_RESERVED_8,
	NODE_D_RESERVED_9,
	NODE_G_TYPE =20,
	NODE_G_SLICEMAP_TYPE,
	NODE_G_VOLUMERENDERING_TYPE,
	NODE_G_SCATTERMAP_TYPE,
	NODE_G_TEXT_TYPE,
	NODE_G_ORTHOGONALFACTOR_TYPE,
	NODE_G_COMMENT_TYPE,
	NODE_G_ISOLINE_TYPE,
	NODE_G_ISOSURFACE_TYPE,
	NODE_G_SURFACERENDER_TYPE,
	NODE_G_AXIS_TYPE,
	NODE_G_BOUNDINGBOX_TYPE,
	NODE_G_HIGHANDLOWDIAGRAM_TYPE,
	NODE_G_PICTURE_TYPE,
	NODE_G_DXF_TYPE,
	NODE_G_GEOTIFF_TYPE,
	NODE_G_CURVE_SLICE_TYPE,
	NODE_G_RESERVED_37,
	NODE_G_RESERVED_38,
	NODE_G_RESERVED_39,
	NODE_C_TYPE= 60,
	NODE_C_CONVERSION_TYPE,
	NODE_C_MERGE_TYPE,
	NODE_C_SUBSET_TYPE,
	NODE_C_ELIMINATIONFILTER_TYPE,
	NODE_C_MESH_TYPE,
	NODE_C_FILTER,
	NODE_C_REPEATFILER,
	NODE_C_RESERVED_68,
	NODE_O_TYPE= 80,
	NODE_O_COLORBAR_TYPE,
	NODE_O_CUTTINGPLANE_TYPE,
	NODE_O_RESERVED_83
};

enum InsertMethodType
{
	INSERT_NODEFAINE,
	INSERT_INVERSEDISDANCE,
	INSERT_CRIGINING,
	INSERT_MASSDATA
};

enum IsotropyType
{
	ISOTROPY_SAME,
	ISOTROPY_UNSAME
};

struct pointOriginal
{
	double x, y, z;
	std::vector<std::string> scalarNameVec;
	std::vector<double> scalar;
};

struct pointFinal
{
	double x;
	double y;
	double z;
	double scalar;
	pointFinal(double _x, double _y, double _z, double _scalar)
	{
		x = _x;
		y = _y;
		z = _z;
		scalar = _scalar;
	}
	pointFinal()
	{
		x = 0;
		y = 0;
		z = 0;
		scalar = 0;
	}
	~pointFinal()
	{

	}
};
void  pointFinalGetRange(std::vector<pointFinal> &data, double* range);

struct LogicMsgHeader
{
	enum msgTytpe
	{
		xmlProjectType,
		otherType
	};
	msgTytpe m_type;
	bool m_runResult;
};

struct XmlProjectRunResult : public LogicMsgHeader
{
	int8_t m_xmlProjectId;
};

struct XmlOtherRunReuslt : public LogicMsgHeader
{

};

uint16_t getKeyById(char ProjectId, char NodeId);
char getProjectId(uint16_t key);
char getNodeId(uint16_t key);

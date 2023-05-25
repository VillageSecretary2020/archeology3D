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

#include "SurperNode.h"

class OCuttingPlane : public SurperNode
{
public:
	OCuttingPlane(std::shared_ptr< XmlProject> project);
	~OCuttingPlane();
};

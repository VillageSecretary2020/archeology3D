#include "OCuttingPlane.h"

OCuttingPlane::OCuttingPlane(std::shared_ptr< XmlProject> project) :SurperNode(project)
{
	m_nodeType = NodeType::NODE_O_CUTTINGPLANE_TYPE;
}

OCuttingPlane::~OCuttingPlane()
{
}

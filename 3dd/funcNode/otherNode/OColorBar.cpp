#include "OColorBar.h"

OColorBar::OColorBar(std::shared_ptr< XmlProject> project) :SurperNode(project)
{
	m_nodeType = NodeType::NODE_O_COLORBAR_TYPE;
}

OColorBar::~OColorBar()
{
}

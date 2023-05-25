#include "globalinclude.h"

uint16_t getKeyById(char ProjectId, char NodeId)
{
	uint16_t high = ProjectId << 8;
	high += NodeId;
	return high;
}

char getProjectId(uint16_t key)
{
	return key >> 8;
}

char getNodeId(uint16_t key)
{
	return key - ((key >> 8) << 8);
}

void  pointFinalGetRange(std::vector<pointFinal> &data, double* range)
{
	std::vector<pointFinal>::iterator itor = data.begin();
	while (itor != data.end())
	{
		double scalar = (*itor).scalar;
		if (scalar < range[0])
			range[0] = scalar;
		if (range[1] < scalar)
			range[1] = scalar;
		itor++;
	}
}

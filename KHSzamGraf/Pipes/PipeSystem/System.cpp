#include "System.h"

System::System(int size, int pipeCount): elements(0), pipeCount(pipeCount)
{
	availability.reserve(size * size * size);
	for (int i = 0; i < size*size*size; ++i)
	{
		availability.push_back(true);
	}

	builders.reserve(pipeCount);
	for (int i = 0; i < pipeCount; ++i)
	{
		builders.push_back(new Builder(availability, size, freshElements));
	}
}

System::~System()
{
	//TODO clear heap
}
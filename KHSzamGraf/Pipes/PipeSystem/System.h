#pragma once
#include <vector>
#include "Element.h"
#include "Builder.h"

class System
{
public:
	System(int size, int pipeCount);
	~System();

	std::vector<Element*> elements;
	std::vector<Element*> freshElements;

protected:
	int pipeCount;

	std::vector<bool> availability;

	std::vector<Builder*> builders;
};

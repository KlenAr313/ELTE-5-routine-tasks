#pragma once
#include <vector>
#include <ctime>
#include "Element.h"
#include "Builder.h"

class System
{
public:
	System(int size, int pipeCount);
	~System();

	void next();
	glm::vec3 colorRandomiser();

	std::vector<Element*> elements;
	std::vector<Element*> freshElements;

	int closedBuilderCount = 0;

	int gridSize;

protected:
	int pipeCount;

	std::vector<bool> availability;

	std::vector<Builder*> builders;
};

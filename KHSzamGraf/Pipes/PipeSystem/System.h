#pragma once
#include <vector>
#include <ctime>
#include <string>
#include "Element.h"
#include "Builder.h"

class System
{
public:
	System(int size, int pipeCount);
	~System();

	void next(int builderCount);
	void reColorAt(int at, glm::vec3 color);

	std::vector<Element*> elements;
	std::vector<Element*> freshElements;
	std::vector<int> elementIDs;

	int closedBuilderCount = 0;

	int gridSize;

protected:
	glm::vec3 colorRandomiser();
	int pipeCount;

	std::vector<bool> availability;

	std::vector<Builder*> builders;
	int builderCount;
};

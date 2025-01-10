#include "Builder.h"
#include <stdlib.h>
#include <ctime>
#include <glm/glm.hpp>
#include <iostream>

Builder::Builder(std::vector<bool>& availability, int gridSize, std::vector<Element*>& freshElements)
	:availability(availability), gridSize(gridSize), freshElements(freshElements)
{
	std::vector<int> possAvail;
	for (int i = 0; i < availability.size(); ++i)
	{
		if (availability[i])
		{
			possAvail.push_back(i);
		}
	}

	std::srand(std::time(nullptr));
	int ind = possAvail[std::rand() % possAvail.size()];

	px = (ind - (ind % (gridSize * gridSize))) / (gridSize * gridSize);
	ind -= px * gridSize * gridSize;
	py = (ind - (ind % gridSize)) / gridSize;
	pz = ind - py * gridSize;
	
	availability[ind] = false;

	std::vector<glm::vec3> ways;
	if (isIn(px + 1) && availability[toIndex(px + 1, py, pz)])
		ways.push_back(glm::vec3(px + 1, py, pz));

	if (isIn(px - 1) && availability[toIndex(px - 1, py, pz)])
		ways.push_back(glm::vec3(px - 1, py, pz));

	if (isIn(py + 1) && availability[toIndex(px, py + 1, pz)])
		ways.push_back(glm::vec3(px, py + 1, pz));

	if (isIn(py - 1) && availability[toIndex(px, py - 1, pz)])
		ways.push_back(glm::vec3(px, py - 1, pz));

	if (isIn(pz + 1) && availability[toIndex(px, py, pz + 1)])
		ways.push_back(glm::vec3(px, py, pz + 1));

	if (isIn(pz - 1) && availability[toIndex(px, py, pz - 1)])
		ways.push_back(glm::vec3(px, py, pz - 1));

	//TODO if size 0
	glm::vec3 way = ways[std::rand() % ways.size()];

	fx = way.x;
	fy = way.y;
	fz = way.z;

	availability[toIndex(fx, fy, fz)];

	Element* e = new Element(px, py, pz, fx - px, fy - py, fz - pz, true);
	freshElements.push_back(e);

	px = fx;
	py = fy;
	pz = fz;

}

Builder::~Builder(){}

int Builder::toIndex(int x, int y, int z)
{
	return gridSize * gridSize * x + gridSize * y + z;
}

bool Builder::isIn(int a)
{
	return a >= 0 && a < gridSize;
}
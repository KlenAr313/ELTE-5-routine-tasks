#pragma once
#include <vector>
#include "Element.h"

class Builder
{
public:
	Builder(std::vector<bool>& availability, int gridSize, std::vector<Element*>& freshElements, glm::vec3 col, bool& canContinue, bool& noMoreSpace);

	~Builder();

	int toIndex(int x, int y, int z);
	bool isIn(int a);
	bool next();
	const int ID;

private:
	static int allID;
	//current position
	int px = 0;
	int py = 0;
	int pz = 0;

	//faceing direction
	int fx = 0;
	int fy = 0;
	int fz = 0;

	int gridSize;

	std::vector<bool>& availability;
	std::vector<Element*>& freshElements;

	glm::vec3 color;
};

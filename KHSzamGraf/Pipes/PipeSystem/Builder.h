#pragma once
#include <vector>
#include "Element.h"

class Builder
{
public:
	Builder(std::vector<bool>& availability, int gridSize, std::vector<Element*>& freshElements);

	~Builder();

	int toIndex(int x, int y, int z);
	bool isIn(int a);

private:
	//current position
	int px;
	int py;
	int pz;

	//faceing direction
	int fx;
	int fy;
	int fz;

	int gridSize;

	std::vector<bool>& availability;
	std::vector<Element*>& freshElements;
};

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Element
{
public:
	Element(int px,int py,int pz,int fx, int fy, int fz, bool isSphere);
	~Element();

	glm::mat4 posRot = glm::identity<glm::mat4>();
private:
	int px;
	int py;
	int pz;

	int fx;
	int fy;
	int fz;

	bool isSphere;
};

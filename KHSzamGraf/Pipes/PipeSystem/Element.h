#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Element
{
public:
	Element(int px,int py,int pz,int fx, int fy, int fz, glm::vec3 color, int ID, bool isSphere, bool isBegin = false, bool isEnd = false, int pfx = 0, int pfy = 0, int pfz = 0);
	~Element();

	glm::mat4 posRot = glm::identity<glm::mat4>();
	glm::mat4 prevPosRot = glm::identity<glm::mat4>();
	glm::vec3 color;
	const bool isSphere;
	const bool isBegin;
	const bool isEnd;
	const int ID;


private:
	glm::vec3 direction;
	glm::vec3 position;
};

#include "Element.h"

Element::Element(int px, int py, int pz, int fx, int fy, int fz, glm::vec3 color, int ID, bool isSphere, bool isBegin, bool isEnd, int pfx, int pfy, int pfz)
	: isSphere(isSphere), isBegin(isBegin), isEnd(isEnd), ID(ID)
{
	direction = glm::vec3((float)fx, (float)fy, (float)fz);
	position = glm::vec3((float)px, (float)py, (float)pz);
	this->color = glm::vec3(color);
	posRot *= glm::translate(position);

	if (!isBegin)
	{
		prevPosRot *= glm::translate(position);
		if (pfy == 0) //if the rotation isnt on the y axes
		{
			prevPosRot *= glm::rotate(glm::half_pi<float>(), glm::vec3((float)pfz, (float)pfy, (float)-pfx));
		}
		else if (pfy == -1)
		{
			prevPosRot *= glm::rotate(glm::pi<float>(), glm::vec3(1, 0, 0));
		}
	}

	if (fy == 0) //if the rotation isnt on the y axes
	{
		posRot *= glm::rotate(glm::half_pi<float>(), glm::vec3((float)fz, (float)fy, (float)-fx));
	}
	else if(fy == -1)
	{
		posRot *= glm::rotate(glm::pi<float>(), glm::vec3(1, 0, 0));
	}
}

Element::~Element(){}
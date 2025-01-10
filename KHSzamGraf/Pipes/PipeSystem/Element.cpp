#include "Element.h"

Element::Element(int px, int py, int pz, int fx, int fy, int fz, bool isSphere)
	: px(px), py(py), pz(pz), fx(fx), fy(fy), fz(fz), isSphere(isSphere)
{
	//TODO set rotation
	posRot *= glm::translate(glm::vec3(px, py, pz));// *glm::rotate(glm::half_pi<float>(), glm::vec3(0));
}

Element::~Element(){}
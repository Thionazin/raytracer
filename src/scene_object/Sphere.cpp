#include <iostream>
#include "Sphere.h"

vector<Hit> Sphere::intersection(Ray& input_ray) {
	std::vector<Hit> output;
	return output;
}

void Sphere::convertCoords(std::shared_ptr<MatrixStack>& MV) {
	// only translations for now
	center = (MV->topMatrix() * glm::vec4(center, 1.0));
}

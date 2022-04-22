#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "SceneOBJ.h"

class Plane : public SceneOBJ {
	public:
		Plane(glm::vec3 po, glm::vec3 nor) : point(po), normal(nor) {};
		vector<Hit> intersection(Ray& input_ray);
	private:
		glm::vec3 point;
		glm::vec3 normal;
};

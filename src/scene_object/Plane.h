#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "SceneOBJ.h"

class Plane : public SceneOBJ {
	public:
		Plane(glm::vec3 po, glm::vec3 nor) : point(po), normal(nor) {};
		void intersection(Ray& input_ray, double& distance, glm::vec3& hit_point, glm::vec3& hit_normal);
	private:
		glm::vec3 point;
		glm::vec3 normal;
};

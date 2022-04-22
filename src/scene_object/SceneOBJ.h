#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "../ray/Ray.h"

class SceneOBJ {
	public:
		virtual void intersection(Ray& input_ray, double& distance, glm::vec3& hit_point, glm::vec3& hit_normal);
};

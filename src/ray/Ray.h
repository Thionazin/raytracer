#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Ray
{
	public:
		Ray(glm::vec3 ori, glm::vec3 dir) : origin(ori), direction(dir) {};
		glm::vec3 origin;
		glm::vec3 direction;
};

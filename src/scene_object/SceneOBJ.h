#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

struct Hit {
	double distance;
	glm::vec3 hit_point;
	glm::vec3 hit_normal;
	Hit(double dist, glm::vec3 hp, glm::vec3 hn) : distance(dist), hit_point(hp), hit_normal(hn) {}
};

class SceneOBJ {
	public:
		virtual std::vector<Hit> intersection(Ray& input_ray);
};

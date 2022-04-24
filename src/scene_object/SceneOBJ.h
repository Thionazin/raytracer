#pragma once

#include <vector>

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
		SceneOBJ() {};
		virtual ~SceneOBJ() {};
		virtual std::vector<Hit> intersection(Ray& input_ray);
		virtual double getRad() { return -100.0; };
	private:
		glm::vec3 center;
		double radius;
		glm::vec3 scale;
		glm::vec4 rotation;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		double exponent;
};

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "SceneOBJ.h"

using std::vector;

class Sphere : public SceneOBJ {
	public:
		Sphere(glm::vec3 cen, double rad, glm::vec3 sca, glm::vec4 rot, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, double exp) {
			center = cen; 
			radius = rad;
			scale = sca;
			rotation = rot;
			ambient = amb;
			diffuse = diff;
			specular = spec;
			exponent = exp;
		};
		~Sphere() {};
		std::vector<Hit> intersection(Ray& input_ray);
		double getRad() { return radius; }
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
#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "SceneOBJ.h"

using std::vector;

class Plane : public SceneOBJ {
	public:
		Plane(glm::vec3 nor, glm::vec3 cen, glm::vec3 sca, glm::vec4 rot, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, double exp) {
			normal = nor;
			center = cen; 
			scale = sca;
			rotation = rot;
			ambient = amb;
			diffuse = diff;
			specular = spec;
			exponent = exp;
		};
		~Plane() {};
		std::vector<Hit> intersection(Ray& input_ray);
		void convertCoords(std::shared_ptr<MatrixStack>& MV);
		glm::vec3 doBPShading(Hit& hit, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int depth);
		float getRad() { return -200.0f; }
	private:
		glm::vec3 normal;
		glm::vec3 center;
		glm::vec3 scale;
		glm::vec4 rotation;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		double exponent;
};

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "SceneOBJ.h"

using std::vector;

class Ellipsoid : public SceneOBJ {
	public:
		Ellipsoid(glm::vec3 cen, float rad, glm::vec3 sca, glm::vec4 rot, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, double exp) {
			center = cen; 
			radius = 1.0;
			scale = sca;
			rotation = rot;
			ambient = amb;
			diffuse = diff;
			specular = spec;
			exponent = exp;
		};
		~Ellipsoid() {};
		std::vector<Hit> intersection(Ray& input_ray);
		void convertCoords(std::shared_ptr<MatrixStack>& MV);
		glm::vec3 doBPShading(Hit& hit, Ray& hit_ray, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int depth);
		float getRad() { return radius; }
	private:
		glm::mat4 transformation;
		glm::mat4 inverse_transform;
		glm::mat4 inverse_transpose_transform;
		glm::vec3 center;
		float radius;
		glm::vec3 scale;
		glm::vec4 rotation;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		double exponent;
		glm::vec3 normal;
};

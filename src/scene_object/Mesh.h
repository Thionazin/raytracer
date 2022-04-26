#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "SceneOBJ.h"

using std::vector;

class Mesh : public SceneOBJ {
	public:
		Mesh(glm::vec3 cen, float rad, glm::vec3 sca, glm::vec4 rot, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, double exp, std::vector<float>& posB, std::vector<float>& norB, std::vector<float>& texB, glm::vec3 translate) {
			center = cen; 
			radius = rad;
			scale = sca;
			rotation = rot;
			ambient = amb;
			diffuse = diff;
			specular = spec;
			exponent = exp;
			posBuf = posB;
			norBuf = norB;
			texBuf = texB;
			translation = translate;
		};
		~Mesh() {};
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
		std::vector<float> posBuf;
		std::vector<float> norBuf;
		std::vector<float> texBuf;
		glm::vec3 translation;
};

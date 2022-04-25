#pragma once

#include <vector>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "../matrix_stack/MatrixStack.h"
#include "../image/Image.h"

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

struct Hit {
	double distance;
	glm::vec3 hit_point;
	glm::vec3 hit_normal;
	Hit() {};
	Hit(double dist, glm::vec3 hp, glm::vec3 hn) : distance(dist), hit_point(hp), hit_normal(hn) {};
};

struct Light {
	float color;
	glm::vec3 position;
	Light() {};
	Light(float col, glm::vec3 pos) : color(col), position(pos) {};
};

class SceneOBJ {
	public:
		SceneOBJ() {};
		virtual ~SceneOBJ() {};
		virtual std::vector<Hit> intersection(Ray& input_ray);
		virtual void convertCoords(std::shared_ptr<MatrixStack>& MV) {};
		virtual float getRad() { return -100.0f; };
		virtual glm::vec3 doBPShading(Hit& hit, Ray& hit_ray, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int depth) { return glm::vec3(0.0f); };
	private:
		glm::mat4 transform;
		glm::mat4 inverse_transform;
		glm::mat4 inverse_transpose_transform;
		glm::vec3 normal;
		glm::vec3 center;
		double radius;
		glm::vec3 scale;
		glm::vec4 rotation;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		double exponent;
};

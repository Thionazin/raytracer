#include <iostream>
#include <algorithm>
#include "Plane.h"

void Plane::convertCoords(std::shared_ptr<MatrixStack>& MV) {
	// only translations for now
	center = (MV->topMatrix() * glm::vec4(center, 1.0));
}

std::vector<Hit> Plane::intersection(Ray& input_ray) {
	std::vector<Hit> output;
	if(!(glm::dot(normal, input_ray.direction) == 0)) {
		float t = glm::dot(normal, (center - input_ray.origin))/glm::dot(normal, input_ray.direction);
		if(t > 0) {
			glm::vec3 x = input_ray.origin + t * input_ray.direction;
			output.emplace_back(t, x, normal);
		}
	}
	return output;
}

glm::vec3 Plane::doBPShading(Hit& hit, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int depth) {
	glm::vec3 color(0.0f);
	color += ambient;
	glm::vec3 cpos(0.0f);
	for(size_t i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - hit.hit_point);
		Ray ra;
		ra.origin = hit.hit_point;
		ra.direction = l;
		bool shadowed = false;
		for(size_t j = 0; j < objs.size(); j++) {
			if(objs[j] != this) {
				std::vector<Hit> can_hit = objs[j]->intersection(ra);
				if(can_hit.size() > 0) {
					shadowed = true;
				}
			}
		}
		if(!shadowed) {
			glm::vec3 h = glm::normalize(glm::normalize(cpos - hit.hit_point) + l);
			glm::vec3 col = lights[i]->color * ((diffuse*std::max(0.0f, glm::dot(l, hit.hit_normal))) + (specular*((float)std::pow(std::max(0.0f, glm::dot(h, hit.hit_normal)), exponent))));
			color += col;
		}
	}
	color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));
	return color;
}

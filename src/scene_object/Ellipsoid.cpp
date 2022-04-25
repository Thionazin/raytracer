#include <iostream>
#include <algorithm>
#include "Sphere.h"

void Sphere::convertCoords(std::shared_ptr<MatrixStack>& MV) {
	// only translations for now
	center = (MV->topMatrix() * glm::vec4(center, 1.0));
}

std::vector<Hit> Sphere::intersection(Ray& input_ray) {
	std::vector<Hit> output;
	glm::vec3 pc = input_ray.origin - center;
	float a = glm::dot(input_ray.direction, input_ray.direction);
	float b = 2.0f * glm::dot((pc), input_ray.direction);
	float c = glm::dot((pc), (pc)) - (radius * radius);
	float d = b*b - 4.0f * a * c;
	if(d <= 0.0f) {
		return output;
	}
	float t1 = (-b + std::sqrt(d))/(2.0f*a);
	glm::vec3 x1 = (input_ray.origin + t1 * input_ray.direction);
	glm::vec3 n1 = (x1 - center)/((float)radius);
	n1 = glm::clamp(n1, glm::vec3(-1.0f), glm::vec3(1.0f));
	if(!(t1 < 0.0f)) {
		output.emplace_back(t1, x1, n1);
	}
	float t2 = (-b - std::sqrt(d))/(2.0f*a);
	glm::vec3 x2 = (input_ray.origin + t2 * input_ray.direction);
	glm::vec3 n2 = (x2 - center)/((float)radius);
	n2 = glm::clamp(n2, glm::vec3(-1.0f), glm::vec3(1.0f));
	if(!(t2 < 0.0f)) {
		output.emplace_back(t2, x2, n2);
	}
	return output;
}

void Sphere::doBPShading(Hit& hit, Image& im, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int x, int y) {
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
	im.setPixel(x, y, 255*color.x, 255*color.y, 255*color.z);
}

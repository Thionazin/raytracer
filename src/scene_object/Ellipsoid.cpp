#include <iostream>
#include <algorithm>
#include "Ellipsoid.h"

void Ellipsoid::convertCoords(std::shared_ptr<MatrixStack>& MV) {
	//center = (MV->topMatrix() * glm::vec4(center, 1.0));
	// need to generate transformation matrixes instead
	MV->translate(center);
	MV->scale(scale);
	transformation = MV->topMatrix();
	inverse_transform = glm::inverse(transformation);
	inverse_transpose_transform = glm::inverse(glm::transpose(transformation));
}

std::vector<Hit> Ellipsoid::intersection(Ray& input_ray) {
	std::vector<Hit> output;
	// new variables
	glm::vec3 new_cen(0.0f);
	Ray ra = input_ray;
	ra.origin = inverse_transform * glm::vec4(ra.origin, 1.0f);
	ra.direction = inverse_transform * glm::vec4(ra.direction, 0.0f);
	ra.direction = glm::normalize(ra.direction);

	glm::vec3 pc = ra.origin - new_cen;
	float a = glm::dot(ra.direction, ra.direction);
	float b = 2.0f * glm::dot((pc), ra.direction);
	float c = glm::dot((pc), (pc)) - (radius * radius);
	float d = b*b - 4.0f * a * c;
	if(d <= 0.0f) {
		return output;
	}
	float t1 = (-b + std::sqrt(d))/(2.0f*a);
	if(!(t1 <= 0.0f)) {
		glm::vec3 xt = ra.origin + t1 * ra.direction;
		glm::vec3 x1 = transformation * glm::vec4(xt, 1.0);
		glm::vec3 n1 = inverse_transpose_transform * glm::vec4(xt, 0.0);
		n1 = normalize(n1);
		t1 = glm::length(x1-input_ray.origin);
		output.emplace_back(t1, x1, n1);
	}

	float t2 = (-b - std::sqrt(d))/(2.0f*a);
	if(!(t2 <= 0.0f)) {
		glm::vec3 xt = ra.origin + t2 * ra.direction;
		glm::vec3 x2 = transformation * glm::vec4(xt, 1.0);
		glm::vec3 n2 = inverse_transpose_transform * glm::vec4(xt, 0.0);
		n2 = normalize(n2);
		t2 = glm::length(x2-input_ray.origin);
		output.emplace_back(t2, x2, n2);
	}
	return output;
}

glm::vec3 Ellipsoid::doBPShading(Hit& hit, Ray& hit_ray, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int depth) {
	glm::vec3 color(0.0f);
	color += ambient;
	glm::vec3 cpos(hit_ray.origin);
	for(size_t i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - hit.hit_point);
		Ray ra;
		ra.origin = hit.hit_point;
		ra.direction = l;
		bool shadowed = false;
		for(size_t j = 0; j < objs.size(); j++) {
			// Self intersection impossible, just ignore yourself instead
			if(objs[j] != this) {
				std::vector<Hit> can_hit = objs[j]->intersection(ra);
				if(can_hit.size() > 0) {
					for(size_t k = 0; k < can_hit.size(); k++) {
						if(can_hit[k].distance < glm::length(lights[i]->position-hit.hit_point)) {
							shadowed = true;
						}
					}
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

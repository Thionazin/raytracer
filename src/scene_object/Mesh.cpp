#include <iostream>
#include <algorithm>
#include <glm/gtx/intersect.hpp>
#include "Mesh.h"

void Mesh::convertCoords(std::shared_ptr<MatrixStack>& MV) {
	MV->translate(translation);
	MV->rotate(rotation.x, rotation.y, rotation.z, rotation.w);
	MV->scale(scale);
	transformation = MV->topMatrix();
	inverse_transform = glm::inverse(transformation);
	inverse_transpose_transform = glm::transpose(inverse_transform);
	center = MV->topMatrix() * glm::vec4(center, 1.0);
	/*
	for(size_t i = 0; i < posBuf.size(); i += 3) {
		glm::vec3 new_coord = MV->topMatrix() * glm::vec4(posBuf[i], posBuf[i+1], posBuf[i+2], 1.0);
		posBuf[i] = new_coord.x;
		posBuf[i+1] = new_coord.y;
		posBuf[i+2] = new_coord.z;
	}
	*/
}

std::vector<Hit> Mesh::intersection(Ray& input_ray) {
	std::vector<Hit> output;
	glm::vec3 pc = input_ray.origin - center;
	float a = glm::dot(input_ray.direction, input_ray.direction);
	float b = 2.0f * glm::dot((pc), input_ray.direction);
	float c = glm::dot((pc), (pc)) - (radius * radius);
	float d = b*b - 4.0f * a * c;
	if(d <= 0.0f) {
		return output;
	}
	// There is an intersection with this bounding sphere, proceed with rendering the bunny
	/*
	float t1 = (-b + std::sqrt(d))/(2.0f*a);
	glm::vec3 x1 = (input_ray.origin + t1 * input_ray.direction);
	glm::vec3 n1 = (x1 - center)/((float)radius);
	n1 = glm::clamp(n1, glm::vec3(-1.0f), glm::vec3(1.0f));
	if(!(t1 <= 0.0f)) {
		output.emplace_back(t1, x1, n1);
	}
	float t2 = (-b - std::sqrt(d))/(2.0f*a);
	glm::vec3 x2 = (input_ray.origin + t2 * input_ray.direction);
	glm::vec3 n2 = (x2 - center)/((float)radius);
	n2 = glm::clamp(n2, glm::vec3(-1.0f), glm::vec3(1.0f));
	if(!(t2 <= 0.0f)) {
		output.emplace_back(t2, x2, n2);
	}
	*/
	Ray ra = input_ray;
	ra.origin = inverse_transform * glm::vec4(ra.origin, 1.0f);
	ra.direction = inverse_transform * glm::vec4(ra.direction, 0.0f);
	ra.direction = normalize(ra.direction);
	Hit closest;
	float closest_hit = FLT_MAX;
	for(size_t i = 0; i < posBuf.size();) {
		glm::vec3 vertex1(posBuf[i], posBuf[i+1], posBuf[i+2]);
		glm::vec3 vertex2(posBuf[i+3], posBuf[i+4], posBuf[i+5]);
		glm::vec3 vertex3(posBuf[i+6], posBuf[i+7], posBuf[i+8]);
		glm::vec3 nor1(norBuf[i], norBuf[i+1], norBuf[i+2]);
		glm::vec3 nor2(norBuf[i+3], norBuf[i+4], norBuf[i+5]);
		glm::vec3 nor3(norBuf[i+6], norBuf[i+7], norBuf[i+8]);
		glm::vec2 bary;
		float bary_dist;
		if(glm::intersectRayTriangle(ra.origin, ra.direction, vertex1, vertex2, vertex3, bary, bary_dist)) {
			if(bary_dist > 0 && bary_dist < closest_hit) {
				closest_hit = bary_dist;
				glm::vec3 posi = vertex1 * (1.0f - bary.x - bary.y) + vertex2 * bary.x + bary.y * vertex3;
				posi = transformation * glm::vec4(posi, 1.0f);
				glm::vec3 norm = glm::normalize(nor1 * (1.0f - bary.x - bary.y) + nor2 * bary.x + bary.y * nor3);
				norm = inverse_transpose_transform * glm::vec4(norm, 0.0f);
				norm = glm::normalize(norm);
				closest = Hit(bary_dist, posi, norm);
			}
		}
		i += 9;
	}
	if(closest_hit != FLT_MAX) {
		output.push_back(closest);
	}
	return output;
}

glm::vec3 Mesh::doBPShading(Hit& hit, Ray& hit_ray, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int depth) {
	glm::vec3 color(0.0f);
	color += ambient;
	glm::vec3 cpos(hit_ray.origin);
	for(size_t i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - hit.hit_point);
		Ray ra;
		// Self intersection possible, need to add epsilon
		ra.origin = hit.hit_point + (hit.hit_normal * 1e-5f);
		ra.direction = l;
		bool shadowed = false;
		for(size_t j = 0; j < objs.size(); j++) {
			std::vector<Hit> can_hit = objs[j]->intersection(ra);
			if(can_hit.size() > 0) {
				for(size_t k = 0; k < can_hit.size(); k++) {
					if(can_hit[k].distance < glm::length(lights[i]->position-hit.hit_point)) {
						shadowed = true;
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

#include <iostream>
#include <algorithm>
#include "RFSphere.h"

void RFSphere::convertCoords(std::shared_ptr<MatrixStack>& MV) {
	// only translations for now
	center = (MV->topMatrix() * glm::vec4(center, 1.0));
}

std::vector<Hit> RFSphere::intersection(Ray& input_ray) {
	std::vector<Hit> output;
	glm::vec3 pc = input_ray.origin - center;
	// Calculate values for quadratic
	float a = glm::dot(input_ray.direction, input_ray.direction);
	float b = 2.0f * glm::dot((pc), input_ray.direction);
	float c = glm::dot((pc), (pc)) - (radius * radius);
	float d = b*b - 4.0f * a * c;
	// If d is less than 0, meaning the quadratic equation will go into imaginary land,
	// then there is no hit
	// Also we consider 1 hit as 0 hit because we're lazy and also it's really rare
	if(d <= 0.0f) {
		return output;
	}
	// first hit
	float t1 = (-b + std::sqrt(d))/(2.0f*a);
	glm::vec3 x1 = (input_ray.origin + t1 * input_ray.direction);
	glm::vec3 n1 = (x1 - center)/((float)radius);
	n1 = glm::clamp(n1, glm::vec3(-1.0f), glm::vec3(1.0f));
	if(!(t1 < 0.0f)) {
		output.emplace_back(t1, x1, n1);
	}
	// second hit
	float t2 = (-b - std::sqrt(d))/(2.0f*a);
	glm::vec3 x2 = (input_ray.origin + t2 * input_ray.direction);
	glm::vec3 n2 = (x2 - center)/((float)radius);
	n2 = glm::clamp(n2, glm::vec3(-1.0f), glm::vec3(1.0f));
	if(!(t2 < 0.0f)) {
		output.emplace_back(t2, x2, n2);
	}
	return output;
}

glm::vec3 RFSphere::doBPShading(Hit& hit, Ray& hit_ray, std::vector<Light*>& lights, std::vector<SceneOBJ*>& objs, int depth) {
	// Special case, color is reflected
	glm::vec3 color(0.0f);
	// Checks if recursion depth limit reached. If so, return the color
	if(depth > 4) {
		return color;
	}
	
	// generate new reflection ray
	glm::vec3 ref_dir = glm::reflect(hit_ray.direction, hit.hit_normal);
	Ray ref_ray;
	ref_ray.origin = hit.hit_point;
	ref_ray.direction = normalize(ref_dir);


	SceneOBJ* closest = nullptr;
	double closest_dist = DBL_MAX;
	Hit closest_hit;
	// Impossible for self intersection
	if(objs[0] != this)
	{
		closest = objs[0];
		std::vector<Hit> hit_vec = objs[0]->intersection(ref_ray);
		for(size_t j = 0; j < hit_vec.size(); j++) {
			if(hit_vec[j].distance < closest_dist) {
				closest_dist = hit_vec[j].distance;
				closest_hit = hit_vec[j];
			}
		}
	}
	for(unsigned int i = 1; i < objs.size(); i++) {
		// Impossible for self intersection
		if(objs[i] != this) {
			std::vector<Hit> hit_vec = objs[i]->intersection(ref_ray);
			for(size_t j = 0; j < hit_vec.size(); j++) {
				if(hit_vec[j].distance < closest_dist) {
					closest = objs[i];
					closest_dist = hit_vec[j].distance;
					closest_hit = hit_vec[j];
				}
			}
		}
	}
	if(closest_dist != DBL_MAX && closest != nullptr) {
		return closest->doBPShading(closest_hit, ref_ray, lights, objs, depth+1);
	}
	return color;
	


	// blinn phong not considered for now. Instead recurse on to next nearest object
	/*
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
	*/
	return color;
}

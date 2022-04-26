#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "../matrix_stack/MatrixStack.h"
#include "../scene/Scene.h"

Camera::Camera(int _width, int _height, double _fov, glm::vec3 _pos, glm::vec4 _rot) :
	aspect(((double)_width)/((double)_height)),
	fovy((double)(_fov*M_PI/180.0)),
	width(_width),
	height(_height),
	position(_pos),
	rotation(_rot)
{
}

Camera::~Camera()
{
}

void Camera::debugTestRays()
{
	for(int r = 0; r < height; r++) {
		for(int c = 0; c < width; c++) {
			float xx = rays[r][c].direction.x;
			float yy = rays[r][c].direction.y;
			float zz = rays[r][c].direction.z;
			std::cout << "Ray " << r + c << " x:" << xx << " y:" << yy << " z:" << zz << std::endl; 
		}
	}
}


void Camera::applyViewMatrix(std::shared_ptr<MatrixStack> MV) const
{
	MV->multMatrix(glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Camera::drawScene(Scene& scene, std::string output_name) {
	Image image(width, height);

	// Convert coords of all items to camera space
	std::shared_ptr<MatrixStack> MV = std::make_shared<MatrixStack>();
	MV->pushMatrix();
	applyViewMatrix(MV);
	for(unsigned int i = 0; i < scene.objs.size(); i++) {
		MV->pushMatrix();
		scene.objs[i]->convertCoords(MV);
		MV->popMatrix();
	}
	for(unsigned int i = 0; i < scene.lights.size(); i++) {
		scene.lights[i]->position = MV->topMatrix() * glm::vec4(scene.lights[i]->position, 1.0f);
	}
	MV->popMatrix();


	// draw
	for(int r = 0; r < height; r++) {
		for(int c = 0; c < width; c++) {
			double pixelX = (2.0 * ((((double)c) + 0.5)/width) - 1.0) * aspect * std::tan(fovy/2);
			double pixelY = (1.0 - (2.0 * ((((double)r) + 0.5)/height))) * std::tan(fovy/2);
			glm::vec3 ray_origin(0.0f);
			glm::vec3 ray_direction = glm::normalize(glm::vec3(pixelX, pixelY, -1.0f) - ray_origin);
			Ray new_ray;
			new_ray.origin = ray_origin;
			new_ray.direction = ray_direction;
			// assumes every scene has at least one object. Otherwise there wouldn't be any point would there
			SceneOBJ* closest = scene.objs[0];
			double closest_dist = DBL_MAX;
			Hit closest_hit;
			{
				std::vector<Hit> hit_vec = scene.objs[0]->intersection(new_ray);
				for(size_t j = 0; j < hit_vec.size(); j++) {
					if(hit_vec[j].distance < closest_dist) {
						closest_dist = hit_vec[j].distance;
						closest_hit = hit_vec[j];
					}
				}
			}
			for(unsigned int i = 1; i < scene.objs.size(); i++) {
				std::vector<Hit> hit_vec = scene.objs[i]->intersection(new_ray);
				for(size_t j = 0; j < hit_vec.size(); j++) {
					if(hit_vec[j].distance < closest_dist) {
						closest = scene.objs[i];
						closest_dist = hit_vec[j].distance;
						closest_hit = hit_vec[j];
					}
				}
			}
			if(closest_dist != DBL_MAX) {
				glm::vec3 color = closest->doBPShading(closest_hit, new_ray, scene.lights, scene.objs, 0);
				image.setPixel(c, height-r-1, 255*color.x, 255*color.y, 255*color.z);
			} else {
				image.setPixel(c, height-r-1, 0, 0, 0);
			}
		}
	}
	image.writeToFile(output_name);
}

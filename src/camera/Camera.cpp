#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "../matrix_stack/MatrixStack.h"
#include "../scene/Scene.h"
#include "../image/Image.h"

Camera::Camera(int _width, int _height, double _fov, glm::vec3 _pos, glm::vec4 _rot) :
	aspect(((double)_width)/((double)_height)),
	fovy((double)(_fov*M_PI/180.0)),
	width(_width),
	height(_height),
	position(_pos),
	rotation(_rot)
{
	rays = std::vector<std::vector<Ray>>(height);
	for(int r = 0; r < height; r++) {
		std::vector<Ray> row(width);
		for(int c = 0; c < width; c++) {
			double pixelX = (2.0 * ((((double)c) + 0.5)/width) - 1.0) * aspect * std::tan(fovy/2);
			double pixelY = (1.0 - (2.0 * ((((double)r) + 0.5)/height))) * std::tan(fovy/2);
			glm::vec3 ray_origin(0.0f);
			glm::vec3 ray_direction = glm::normalize(glm::vec3(pixelX, pixelY, -1.0f) - ray_origin);
			Ray new_ray;
			new_ray.origin = ray_origin;
			new_ray.direction = ray_direction;
			row[c] = new_ray;
		}
		rays[r] = row;
	}
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
	MV->multMatrix(glm::lookAt(position, glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Camera::drawScene(Scene& scene, std::string output_name) {
	Image image(width, height);


	image.writeToFile(output_name);
}

#pragma  once
#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "../scene/Scene.h"

class MatrixStack;

class Camera
{
public:
	Camera(int _width, int _height, double _fov, glm::vec3 _pos, glm::vec4 _rot);
	virtual ~Camera();
	void applyViewMatrix(std::shared_ptr<MatrixStack> MV) const;
	void debugTestRays();
	void drawScene(Scene& scene, std::string output_name);
	
private:
	double aspect;
	double fovy;
	int width;
	int height;
	glm::vec3 position;
	glm::vec4 rotation;
	std::vector<std::vector<Ray>> rays;
};

#endif

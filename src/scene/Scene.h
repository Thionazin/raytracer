#pragma once
#include <memory>
#include <vector>

#include "../scene_object/SceneOBJ.h"

class Scene
{
	public:
		Scene() {};
		~Scene() {
			for(unsigned int i = 0; i < objs.size(); i++) {
					delete objs[i];
			}
			for(unsigned int i = 0; i < lights.size(); i++) {
					delete lights[i];
			}
		};
		std::vector<SceneOBJ*> objs;
		std::vector<Light*> lights;
};

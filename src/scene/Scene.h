#pragma once
#include <memory>
#include <vector>

#include "../scene_object/SceneOBJ.h"
#include "../light/Light.h"

class Scene
{
	public:
		Scene(std::vector<std::shared_ptr<SceneOBJ>> objs, std::vector<std::shared_ptr<Light>> lights);
	private:
		std::vector<std::shared_ptr<SceneOBJ>> objs;
		std::vector<std::shared_ptr<Light>> lights;
		
};

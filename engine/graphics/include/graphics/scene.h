#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "graphics/shader_program.h"
#include "graphics/window.h"
#include "mesh.h"

namespace bz::engine::graphics {

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;

	Scene(const Scene &) = delete;
	Scene &operator=(const Scene &) = delete;

	Scene(Scene &&) = default;
	Scene &operator=(Scene &&) = default;

	// Developer interface
public:
	void addMesh(const std::string &name, std::shared_ptr<Mesh> mesh);

	Mesh* getMesh(const std::string &name);

	std::vector<std::shared_ptr<Mesh>> meshes();

	// Engine interface
public:
	// virtual void render(Window *window, Scene *scene) = 0;

private:
	std::unordered_map<std::string, std::shared_ptr<Mesh>> _meshes;
	std::unique_ptr<Shader> _shader{nullptr};
};

} // namespace bz::engine::graphics
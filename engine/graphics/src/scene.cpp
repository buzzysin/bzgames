#include <utility>

#include "bz/graphics/scene.h"

namespace bz::engine::graphics {

void Scene::addMesh(const std::string &name, std::unique_ptr<Mesh> mesh) {
	_meshes[name] = std::move(mesh);
}

Mesh *Scene::getMesh(const std::string &name) { return _meshes[name].get(); }

std::vector<Mesh *> Scene::meshes() {
	std::vector<Mesh *> result;
	result.reserve(_meshes.size());

	for (const auto &pair : _meshes) {
		result.push_back(pair.second.get());
	}

	return result;
}

} // namespace bz::engine::graphics
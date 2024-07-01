#include "graphics/renderer.h"

namespace bz::engine::graphics {

void Renderer::setShader(std::unique_ptr<Shader> shader) {
	_shader = std::move(shader);
}

Shader *Renderer::shader() { return _shader.get(); }

} // namespace bz::engine::graphics
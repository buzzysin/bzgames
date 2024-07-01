#pragma once

#include "core/result.h"
#include "graphics/shader_data.h"
#include "graphics/shader_program.h"
#include <memory>

namespace bz::engine::errors {
struct GLShaderError : bz::core::Error {
	GLShaderError() = default;
	GLShaderError(const char *message) : Error(message) {}
	GLShaderError(const std::string_view &message) : Error(message) {}
	GLShaderError(const std::string &message) : Error(message) {}
};

struct GLShaderFileNotFound : public GLShaderError {
	GLShaderFileNotFound(const char *shaderPath)
		: GLShaderError("Cannot find shader file: " + std::string(shaderPath)) {
	}
};

struct GLShaderCreateProgramFailed : public GLShaderError {
	GLShaderCreateProgramFailed()
		: GLShaderError("Failed to create GL shader program") {}
};

struct GLShaderCreateShaderFailed : public GLShaderError {
	GLShaderCreateShaderFailed(const char *shaderPath)
		: GLShaderError("Failed to create GL vertex shader: " +
	                    std::string(shaderPath)) {}
};

struct GLShaderCompileFailed : public GLShaderError {
	GLShaderCompileFailed(const char *shaderPath, const char *log)
		: GLShaderError("Failed to compile shader: " + std::string(shaderPath) +
	                    "\n" + log) {}
};

struct GLShaderLinkFailed : public GLShaderError {
	GLShaderLinkFailed() : GLShaderError("Failed to link shader program") {}
};

struct GLShaderValidateFailed : public GLShaderError {
	GLShaderValidateFailed()
		: GLShaderError("Failed to validate shader program") {}
};

} // namespace bz::engine::errors
namespace bz::engine::graphics {

class GLShaderProgram : public Shader {
public:
	GLShaderProgram(int programId);
	virtual ~GLShaderProgram();

	GLShaderProgram(const GLShaderProgram &) = delete;
	GLShaderProgram &operator=(const GLShaderProgram &) = delete;

	GLShaderProgram(GLShaderProgram &&) noexcept;
	GLShaderProgram &operator=(GLShaderProgram &&) noexcept;

public:
	static core::Result<std::unique_ptr<GLShaderProgram>, errors::GLShaderError>
	create(const ShaderData &shaderData);

	[[nodiscard]] int id() const;

public:
	void bind() override;
	void unbind() override;

	// core::Result<core::Empty, errors::GLShaderError> link();

private:
	static core::Result<int, errors::GLShaderError>
	_createShader(int type, const std::string &shaderPath);

	static core::Fallible<errors::GLShaderError>
	_linkShader(int id, const std::vector<int> &shaderId);

private:
	int _programId{};
	// std::vector<int> _modules;
};

class GLShaderProgramCtx {
public:
	GLShaderProgramCtx(GLShaderProgram &shader) : _shader{shader} {
		_shader.bind();
	}
	~GLShaderProgramCtx() { _shader.unbind(); }

	GLShaderProgramCtx(const GLShaderProgramCtx &) = delete;
	GLShaderProgramCtx &operator=(const GLShaderProgramCtx &) = delete;

	GLShaderProgramCtx(GLShaderProgramCtx &&) = delete;
	GLShaderProgramCtx &operator=(GLShaderProgramCtx &&) = delete;

private:
	GLShaderProgram &_shader;
};

} // namespace bz::engine::graphics
#pragma once

#include "core/result.h"
#include "graphics/shader.h"
#include "graphics/shader_data.h"

namespace bz::engine::graphics {

namespace errors {
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

} // namespace errors

class GLShader : public Shader {
public:
	GLShader() = default;
	virtual ~GLShader();

	GLShader(const GLShader &) = delete;
	GLShader &operator=(const GLShader &) = delete;

	GLShader(GLShader &&) = default;
	GLShader &operator=(GLShader &&) = default;

private:
	GLShader(int programId, int vertexShaderId, int fragmentShaderId);

public:
	static core::Result<GLShader, errors::GLShaderError>
	create(const ShaderData &shaderData);

public:
	void bind() override;
	void unbind() override;

	core::Result<core::Empty, errors::GLShaderError> link();

private:
	static core::Result<int, errors::GLShaderError>
	_createShader(int id, int type, const std::string &shaderPath);

	static core::Fallible<errors::GLShaderError>
	_linkShader(int id, int vertexShaderId, int fragmentShaderId);

private:
	int _programId;
	int _vertexShaderId;
	int _fragmentShaderId;
};

} // namespace bz::engine::graphics
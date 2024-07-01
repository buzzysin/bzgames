#pragma once

#include <string>
#include <vector>
namespace bz::engine::graphics {

struct ShaderModule {
	int type;
	std::string path;
};

struct ShaderData {
	std::vector<ShaderModule> modules;
};

} // namespace bz::engine::graphics
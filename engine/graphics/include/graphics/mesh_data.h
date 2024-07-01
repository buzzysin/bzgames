#pragma once

#include <array>
#include <core/error.h>
#include <vector>

namespace bz::engine::graphics {

struct MeshData {
  std::vector<float> vertices;
  std::vector<float> colours;
  std::vector<unsigned int> indices;
  // std::vector<float> normals;
  // std::vector<float> texCoords;
};

} // namespace bz::engine::graphics
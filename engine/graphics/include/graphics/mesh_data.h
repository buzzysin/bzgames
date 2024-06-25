#pragma once

#include <array>
#include <core/error.h>
#include <vector>

namespace bz::engine::graphics {

struct MeshData {
  std::vector<unsigned int> indices;
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> tangents;
  std::vector<float> texCoords;
  std::vector<float> weights;
  unsigned int materialIndex;
  
  std::array<float, 3> aabbMin;
  std::array<float, 3> aabbMax;
  std::vector<float> biTangents;

  std::array<float, 3> colors;
};

} // namespace bz::engine::graphics
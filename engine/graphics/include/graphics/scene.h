#pragma once

namespace bz::engine::graphics {

class Scene {
public:
  Scene() = default;
  virtual ~Scene() = default;

  Scene(const Scene &) = delete;
  Scene &operator=(const Scene &) = delete;

  Scene(Scene &&) = default;
  Scene &operator=(Scene &&) = default;

public:
  virtual void render() = 0;
};

} // namespace bz::engine::graphics
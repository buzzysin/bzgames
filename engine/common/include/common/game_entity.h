#pragma once

namespace bz::engine::common {

class GameEntity {
public:
  GameEntity() = default;
  virtual ~GameEntity() = default;

  GameEntity(const GameEntity &) = delete;
  GameEntity &operator=(const GameEntity &) = delete;

  GameEntity(GameEntity &&) = default;
  GameEntity &operator=(GameEntity &&) = default;

public:
  virtual void update() = 0;
};

} // namespace bz::engine::common
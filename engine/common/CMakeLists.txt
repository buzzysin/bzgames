bz_lib(common # To avoid name collision with the core library
  PARENT 
    engine
  DEPENDS
    bz::core
  SOURCES
    src/game.cpp
)
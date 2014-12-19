include_directories(include/)

# =======================================
# Root Sources
# =======================================

set(ROOT_SOURCES
    src/Game.cpp
    src/sdl/SDLInput.cpp
    src/Physics.cpp
    src/Renderer.cpp
    src/Sound.cpp
    src/TextureFactory.cpp
    src/TileMap.cpp
    src/Timer.cpp)

set(ROOT_HEADERS
    include/Console.h
    include/Entity.h
	include/Filesystem.h
    include/FrameCounter.h
    include/Game.h
    include/Identity.h
    include/Input.h
    include/Updateable.h
    include/Renderable.h
    include/Log.h
    include/Physics.h
    include/Random.h
    include/Rect2.h
    include/Renderer.h
    include/Sound.h
    include/Texture.h
    include/TextureFactory.h
    include/TileEngine.h
    include/TileMap.h
    include/Timer.h
    include/Transform.h
    include/Vec2.h)

set(SDL_SOURCES
    src/sdl/SDLInput.cpp)

set(SDL_HEADERS
    include/SDL/SDLInput.h)

# =======================================
# Include Directories
# =======================================

include_directories(${SDL2_INCLUDE_DIR})

include_directories(${SDL2_IMAGE_INCLUDE_DIR})

include_directories(${SDL2_TTF_INCLUDE_DIR})

include_directories(${SDL2_MIXER_INCLUDE_DIR})

# =======================================
# Library
# =======================================

add_library(engine STATIC 
    ${ROOT_SOURCES}
    ${ROOT_HEADERS}

    ${SDL_SOURCES}
    ${SDL_HEADERS})

target_link_libraries(engine

    ${SDL2_MIXER_LIBRARY})
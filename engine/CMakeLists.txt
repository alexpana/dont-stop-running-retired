include_directories(include/)

# =======================================
# Root Sources
# =======================================

set(ROOT_SOURCES
    src/Bone2D.cpp
    src/Engine.cpp
    src/Identity.cpp
    src/InputSystem.cpp
    src/MathUtils.cpp
    src/Physics.cpp
    src/Sound.cpp
    src/SpriteSheetLoader.cpp
    src/StateInput.cpp
    src/TileMap.cpp
    src/win/Timer.cpp
    src/Version.cpp)

set(ROOT_HEADERS
    include/Bone2D.h
    include/CompileFlags.h
    include/Console.h
    include/Constants.h
    include/Entity.h
	include/Filesystem.h
    include/FPSCounter.h
    include/Engine.h
    include/Identity.h
    include/StateInput.h
    include/InputSystem.h
    include/Updateable.h
    include/Renderable.h
    include/Log.h
    include/MathUtils.h
    include/Memory.h
    include/Physics.h
    include/Random.h
    include/Rect2.h
    include/Renderer.h
    include/Sound.h
    include/Sprite.h
    include/SpriteSheet.h
    include/SpriteSheetLoader.h
    include/Texture.h
    include/TextureLoader.h
    include/TileEngine.h
    include/TileMap.h
    include/Timer.h
    include/Transform.h
    include/Vec2.h
    include/Version.h)

set(SDL_SOURCES
    src/sdl/SDLInputSystem.cpp
    src/sdl/SDLRenderer.cpp
    src/sdl/SDLTexture.cpp
    src/sdl/SDLTextureLoader.cpp
    src/sdl/SDLUtils.cpp)

set(SDL_HEADERS
    include/sdl/SDLInputSystem.h
    include/sdl/SDLRenderer.h
    include/sdl/SDLTexture.h
    include/sdl/SDLTextureLoader.h
    include/sdl/SDLUtils.h)

set(WIN_SOURCES
    src/win/Filesystem.cpp)

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
    ${SDL_HEADERS}

    ${WIN_SOURCES})

target_link_libraries(engine

    ${SDL2_MIXER_LIBRARY})
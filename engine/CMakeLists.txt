include_directories(include/)

# =======================================
# Root Sources
# =======================================

set(ROOT_SOURCES
    src/Game.cpp
    src/Sound.cpp
    src/TextureFactory.cpp
    src/TileMap.cpp)

set(ROOT_HEADERS
    include/Console.h
	include/Event.h
	include/Filesystem.h
    include/FrameCounter.h
    include/Game.h
    include/Log.h
    include/Random.h
    include/Rect2.h
    include/Sound.h
    include/Texture.h
    include/TextureFactory.h
    include/TileEngine.h
    include/TileMap.h
    include/Timer.h
    include/Vec2.h)

# =======================================
# Physics Sources
# =======================================

set(PHYSICS_SOURCES)

set(PHYSICS_HEADERS)

# =======================================
# Physics Sources
# =======================================

set(GRAPHICS_SOURCES
    src/Renderer.cpp)

set(GRAPHICS_HEADERS
    include/Renderer.h)

# =======================================
# Source groups
# =======================================

source_group(Physics FILES ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})
source_group(Engine FILES ${ROOT_SOURCES} ${ROOT_HEADERS})
source_group(Graphics FILES ${GRAPHICS_SOURCES} ${GRAPHICS_HEADERS})

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

    ${PHYSICS_SOURCES}
    ${PHYSICS_HEADERS}

    ${GRAPHICS_SOURCES}
    ${GRAPHICS_HEADERS})

target_link_libraries(engine

    ${SDL2_MIXER_LIBRARY})
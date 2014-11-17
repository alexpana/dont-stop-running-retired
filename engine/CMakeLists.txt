include_directories(include/)

# =======================================
# Root Sources
# =======================================

set(ROOT_SOURCES
    src/Game.cpp
    src/TextureFactory.cpp
    src/TileMap.cpp
    src/Filesystem.cpp)

set(ROOT_HEADERS
    include/Game.h
    include/Texture.h
    include/TextureFactory.h
    include/Log.h
    include/Random.h
    include/Rect2.h
    include/TileEngine.h
    include/TileMap.h
    include/Timer.h
    include/Vec2.h
    include/Filesystem.h)

# =======================================
# Physics Sources
# =======================================

set(PHYSICS_SOURCES
    src/physics/Physics.cpp
    src/physics/Particle.cpp)

set(PHYSICS_HEADERS
    include/physics/Physics.h
    include/physics/Particle.h
    include/physics/CollisionSystem.h
    include/physics/ParticleForceGenerator.h
    include/physics/GravityForceGenerator.h
    include/physics/ParticleGeneratorRegistry.h)

# =======================================
# Physics Sources
# =======================================

set(GRAPHICS_SOURCES
    src/graphics/Renderer.cpp)

set(GRAPHICS_HEADERS
    include/graphics/Renderer.h)

# =======================================
# Source groups
# =======================================

source_group(Physics FILES ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})
source_group(Engine FILES ${ROOT_SOURCES} ${ROOT_HEADERS})
source_group(Graphics FILES ${GRAPHICS_SOURCES} ${GRAPHICS_HEADERS})

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

# =======================================
# Game Sources
# =======================================

set(GAME_SOURCES
    Main.cpp
    Runner.cpp
    RunnerTrail.cpp
    World.cpp)

set(GAME_HEADERS
    RunnerTrail.h
    Runner.h
    Stats.h
    World.h)

# =======================================
# Source groups
# =======================================

source_group(Game FILES ${GAME_SOURCES} ${GAME_HEADERS})

# =======================================
# Executable
# =======================================

add_executable(game
    ${GAME_SOURCES}
    ${GAME_HEADERS})

# =======================================
# Include directories
# =======================================

include_directories(${SDL2_INCLUDE_DIR})

include_directories(${SDL2_IMAGE_INCLUDE_DIR})

include_directories(../engine/include/)

# =======================================
# Link libraries
# =======================================

target_link_libraries(game 
    engine

    ${SDL2MAIN_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}

    ${BOX2D_LIBRARY}

    winmm
    imm32 
    version)

# =======================================
# Dependencies libraries
# =======================================
add_dependencies(game engine)
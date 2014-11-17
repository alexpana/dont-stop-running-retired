# =======================================
# Game Sources
# =======================================

set(GAME_SOURCES
    main.cpp
    test_physics.cpp)

set(GAME_HEADERS
    world.h
    runner.h
    defines.h)

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

include_directories(../engine/include/)

# =======================================
# Link libraries
# =======================================

target_link_libraries(game 
    engine 

    ${SDL2_LIBRARY} 
    ${SDL2_IMAGE_LIBRARY} 
    winmm 
    imm32 
    version)

# =======================================
# Dependencies libraries
# =======================================
add_dependencies(game engine)
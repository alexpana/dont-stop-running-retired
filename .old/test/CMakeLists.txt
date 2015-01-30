# =======================================
# Test files
# =======================================

set(TEST_HEADERS
    TestingFramework.h)

set(TEST_SOURCES
    test.cpp
    TestBones.h
    TestIdentities.h
    TestMath.h
    TestSpriteSheetLoader.h)

# =======================================
# Executable
# =======================================

add_executable(test
    ${TEST_HEADERS}
    ${TEST_SOURCES})

# =======================================
# Include directories
# =======================================

include_directories(${SDL2_INCLUDE_DIR})

include_directories(${SDL2_IMAGE_INCLUDE_DIR})

include_directories(../engine/include)

# =======================================
# Link libraries
# =======================================

target_link_libraries(test
    engine

    ${SDL2MAIN_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}

    winmm
    imm32
    version)

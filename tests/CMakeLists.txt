# =======================================
# Test Sources
# =======================================

set(TEST_SOURCES
    tests.cpp)

set(TEST_HEADERS
    test_physics.h)

# =======================================
# Source groups
# =======================================

source_group(Tests FILES ${TEST_SOURCES} ${TEST_HEADERS})

# =======================================
# Include directories
# =======================================

include_directories(../engine/include/)

# =======================================
# Executable
# =======================================

add_executable(tests
    ${TEST_SOURCES}
    ${TEST_HEADERS})

# =======================================
# Link libraries
# =======================================

target_link_libraries(tests 
    engine 

    ${SDL2_LIBRARY} 
    ${SDL2_IMAGE_LIBRARY} 
    winmm 
    imm32 
    version)

# =======================================
# Dependencies libraries
# =======================================
add_dependencies(tests engine)
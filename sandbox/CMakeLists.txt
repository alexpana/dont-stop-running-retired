find_package(SDL2 REQUIRED)

find_package(BGFX REQUIRED)

# find_package(Nanovg REQUIRED)

# Configure BX and BGFX
add_definitions(-DBX_CONFIG_CRT_FILE_READER_WRITER)

set(SANDBOX_HEADERS
    bx2_math.h
    particles.h
    random.h
    settings.h
    types.h
    utils.h)

set(SANDBOX_SOURCES
    main.cpp
    utils.cpp
    particles.cpp)

add_executable(sandbox
    ${SANDBOX_SOURCES}
    ${SANDBOX_HEADERS})

include_directories(
#    ${NANOVG_INCLUDE_DIR}
    ${BGFX_INCLUDE_DIR}
    ${BX_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR})

target_link_libraries(sandbox
#    ${NANOVG_LIBRARY}
    ${BGFX_LIBRARY_DEBUG}
    ${SDL2_LIBRARY}
    pthread
    GL
    X11
    dl)
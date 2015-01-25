find_package(SDL2 REQUIRED)

find_package(BGFX REQUIRED)

find_package(RapidJSON REQUIRED)

find_package(Nanovg REQUIRED)

find_package(GLM REQUIRED)

# Configure BX and BGFX
add_definitions(-DBX_CONFIG_CRT_FILE_READER_WRITER)

set(SANDBOX_HEADERS
    engine/asset_loader.h
    engine/assets.h
    engine/game_object.h
    engine/json_reader.h
    engine/math.h
    engine/log.h
    engine/particles.h
    engine/random.h
    engine/renderer.h
    engine/settings.h
    engine/shader.h
    engine/sprite.h
    engine/texture.h
    engine/types.h
    engine/bgfx_utils.h)

set(SANDBOX_SOURCES
    engine/asset_loader.cpp
    engine/assets.cpp
    engine/bgfx_utils.cpp
    engine/particles.cpp
    engine/renderer.cpp
    main.cpp)

add_executable(sandbox
    ${SANDBOX_SOURCES}
    ${SANDBOX_HEADERS})

include_directories(
    ${GLM_INCLUDE_DIR}
    ${NANOVG_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIR}
    ${BGFX_INCLUDE_DIR}
    ${BX_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR})

target_link_libraries(sandbox
    ${NANOVG_LIBRARY}
    ${BGFX_LIBRARY_DEBUG}
    ${SDL2_LIBRARY}
    pthread
    GL
    X11
    dl)
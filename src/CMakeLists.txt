find_package(SDL2 REQUIRED)

find_package(BGFX REQUIRED)

find_package(RapidJSON REQUIRED)

find_package(Nanovg REQUIRED)

find_package(GLM REQUIRED)

# Configure BX and BGFX
add_definitions(-DBX_CONFIG_CRT_FILE_READER_WRITER)

set(UI_SOURCES
    # ImGUI
    engine/ui/imgui/imgui.cpp
    engine/ui/imgui/ocornut_imgui.cpp
    ${BGFX_3RDPARTY_INCLUDE_DIR}/ocornut-imgui/imgui.cpp

    # nanovg
    engine/ui/nanovg/nanovg.cpp
    engine/ui/nanovg/nanovg_bgfx.cpp

    engine/ui/gui.h
    engine/ui/gui.cpp)

set(EDITOR_SOURCES
    map_editor/context.h
    map_editor/editor.h
    map_editor/editor.cpp
    map_editor/editor_ui.h)

set(SANDBOX_HEADERS
    engine/input/input.h
    engine/asset_loader.h
    engine/assets.h
    engine/game_object.h
    engine/json_reader.h
    engine/level_map.h
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
    engine/input/input.cpp
    engine/asset_loader.cpp
    engine/assets.cpp
    engine/bgfx_utils.cpp
    engine/particles.cpp
    engine/renderer.cpp
    main.cpp)

add_executable(dont_stop_running
    ${SANDBOX_SOURCES}
    ${SANDBOX_HEADERS}

    ${UI_SOURCES}

    ${EDITOR_SOURCES})

include_directories(
    engine/ui/imgui
    engine/ui/nanovg
    ${GLM_INCLUDE_DIR}
    ${NANOVG_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIR}
    ${BGFX_INCLUDE_DIR}
    ${BGFX_3RDPARTY_INCLUDE_DIR}
    ${BX_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR})

target_link_libraries(dont_stop_running
    ${NANOVG_LIBRARY}
    ${BGFX_LIBRARY_DEBUG}
    ${SDL2_LIBRARY}
    pthread
    GL
    X11
    dl)
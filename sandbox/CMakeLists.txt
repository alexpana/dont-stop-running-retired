find_package(SDL2 REQUIRED)
find_package(BGFX REQUIRED)

# Configure BX and BGFX
add_definitions(-DBX_CONFIG_CRT_FILE_READER_WRITER)

set(SANDBOX_SOURCES
    main.cpp)

add_executable(sandbox
    ${SANDBOX_SOURCES})

include_directories(
    ${BGFX_INCLUDE_DIR}
    ${BX_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR})

target_link_libraries(sandbox
    ${BGFX_LIBRARY_DEBUG}
    ${SDL2_LIBRARY}
    pthread
    GL
    X11
    dl)
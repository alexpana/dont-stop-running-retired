find_package(SDL2 REQUIRED)

set(SANDBOX_SOURCES
    main.cpp)

add_executable(sandbox
    ${SANDBOX_SOURCES})

set(BGFX_LIBRARY bgfxDebug)
set(BGFX_INCLUDE_DIR ../extern/bgfx/include/)
set(BX_INCLUDE_DIR ../extern/bx/include/)

include_directories(
    ${BGFX_INCLUDE_DIR}
    ${BX_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR})

target_link_libraries(sandbox
    ${BGFX_LIBRARY}
    ${SDL2_LIBRARY}
    pthread
    GL
    X11
    dl)
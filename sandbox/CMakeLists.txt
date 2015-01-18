find_package(SDL2 REQUIRED)
find_package(BGFX REQUIRED)

set(SANDBOX_SOURCES
    main.cpp)

add_executable(sandbox
    ${SANDBOX_SOURCES})

#set(BGFX_INCLUDE_DIR ../extern/bgfx/include/)
#set(BX_INCLUDE_DIR ../extern/bx/include/)
#set(BGFX_LINK_DIR ../extern/bgfx/.build/linux-debug64/bin)
#set(BGFX_LIBRARY bgfxDebug)

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
set(SANDBOX_SOURCES
    main.cpp)

add_executable(sandbox
    ${SANDBOX_SOURCES})

#set(BGFX_LIBRARY ../extern/bgfx/.build/linux64_gcc/bin/bgfx)
set(BGFX_LIBRARY bgfx)
set(BGFX_INCLUDE_DIR ../extern/bgfx/include/)

include_directories(
    ${BGFX_INCLUDE_DIR})

target_link_libraries(sandbox
${BGFX_LIBRARY}
pthread
GL
X11
    )
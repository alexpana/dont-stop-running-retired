FIND_PATH(BGFX_INCLUDE_DIR bgfx.h HINTS ../extern/bgfx/include)

IF("${BGFX_INCLUDE_DIR}" STREQUAL "BGFX_INCLUDE_DIR-NOTFOUND")
    set(BGFX_3RDPARTY_INCLUDE_DIR "BGFX_3RDPARTY_INCLUDE_DIR-NOTFOUND")
ELSE()
    set(BGFX_3RDPARTY_INCLUDE_DIR "${BGFX_INCLUDE_DIR}/../3rdparty")
ENDIF()

FIND_PATH(BGFX_3RDPARTY_INCLUDE_DIR bgfx.h HINTS ../extern/bgfx/include)
FIND_PATH(BX_INCLUDE_DIR bx/bx.h HINTS ../extern/bx/include)

FIND_LIBRARY(BGFX_LIBRARY_DEBUG bgfxDebug HINTS ../extern/bgfx/.build/linux64_gcc/bin PATH_SUFFIXES lib)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(BGFX REQUIRED_VARS BGFX_LIBRARY_DEBUG BGFX_INCLUDE_DIR BX_INCLUDE_DIR)


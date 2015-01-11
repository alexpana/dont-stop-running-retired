# =======================================
# Source files
# =======================================

set(ASSET_VIEWER_HEADERS )

set(ASSET_VIEWER_SOURCES
    AssetViewer.cpp)

# =======================================
# Executable
# =======================================

add_executable(asset_viewer
    ${ASSET_VIEWER_HEADERS}
    ${ASSET_VIEWER_SOURCES})

# =======================================
# Include directories
# =======================================

include_directories(${SDL2_INCLUDE_DIR})

include_directories(${SDL2_IMAGE_INCLUDE_DIR})

include_directories(../engine/include/)

# =======================================
# Link libraries
# =======================================

target_link_libraries(asset_viewer
    engine

    ${SDL2MAIN_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}

    winmm
    imm32
    version)

#pragma once

#include <string>

namespace dsr {
    /**
    * Displays a platform-dependent save file dialog. Returns the name of the file
    * if one was selected, otherwise returns an empty string.
    */
    std::string saveFileDialog();

    /**
    * Displays a platform-dependent open file dialog. Returns the name of the file
    * if one was selected, otherwise returns an empty string.
    */
    std::string openFileDialog();
}
#include <string>
#include <iostream>
#include <bx/platform.h>

namespace dsr {

#if BX_PLATFORM_LINUX
    std::string exec(std::string cmd) {
        FILE *pipe = popen(cmd.c_str(), "r");

        if (!pipe) {
            return "";
        }

        std::string result = "";

        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }

        pclose(pipe);
        return result;
    }

    std::string saveFileDialog() {
        std::string filename = exec("kdialog --getsavefilename `pwd` \"*.json | Map files\"");
        if (filename.length() > 1) {
            return filename.substr(0, filename.length() - 1);
        }
        return "";
    }

    std::string openFileDialog() {
        std::string filename = exec("kdialog --getopenfilename `pwd` \"*.json | Map files\"");
        if (filename.length() > 1) {
            return filename.substr(0, filename.length() - 1);
        }
    }
#endif
}
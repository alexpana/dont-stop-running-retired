#include <string>

#include "map_editor/map_editor.h"
#include "particle_editor/particle_editor.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        if (std::string(argv[1]) == "map_editor") {
            return dsr::runMapEditor();
        }
        if (std::string(argv[1]) == "particle_editor") {
            return dsr::runParticleEditor();
        }
    }

    return 0;
};

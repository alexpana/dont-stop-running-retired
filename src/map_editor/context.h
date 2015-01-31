#pragma once

#include "../engine/level_map.h"

namespace dsr {
    struct EditorContext {
        enum Action {
            SELECT,
            SPAWN,
            DRAW_SHAPE
        };

        const U16 viewportWidth;
        const U16 viewportHeight;

        bool snapToGrid = true;
        bool gridVisible = true;
        bool cursorLocationVisible = false;
        S32 action = Action::SELECT;

        S32 selectedSpawnObject = -1;

        // Level related stuff
        LevelMap *levelMap;
        S32 selectedEntityIndex;


        // Drawing shape
        struct {
            bool isDrawing = false;
            glm::vec2 startPosition;
        } drawShape;

        // Spawning entity
        LevelMap::Entity currentlySpawningEntity;

        struct {
            bool editorHeaderVisible = true;
        } ui;

        std::string saveFilename = "";
        bool changedSinceLastSave = false;

        // Functions
        std::function<void()> fnActionChanged = nullptr;
        std::function<std::string()> fnActionLoad = nullptr;
        std::function<void(std::string)> fnActionSave = nullptr;
        std::function<void()> fnSpawnObjectSelected = nullptr;

        EditorContext(U16 w, U16 h) : viewportWidth(w), viewportHeight(h) {
        };

        LevelMap::Entity *selectedEntity() {
            if (selectedEntityIndex > 0) {
                return &levelMap->entities[selectedEntityIndex];
            } else {
                return nullptr;
            }
        }
    };
}
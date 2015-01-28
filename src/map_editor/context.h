#pragma once

#include "../engine/level_map.h"

namespace dsr {
    struct EditorContext {

        enum Action {
            SELECT,
            SPAWN
        };

        const U16 viewportWidth;
        const U16 viewportHeight;

        LevelMap::Entity *selectedEntity;

        bool snapToGrid = true;
        bool gridVisible = true;
        bool cursorLocationVisible = false;

        S32 selectedSpawnObject = -1;

        S32 action = Action::SELECT;

        struct {
            bool editorHeaderVisible = true;
        } ui;

        // Functions
        std::function<void()> fnActionChanged = nullptr;
        std::function<void()> fnActionLoad = nullptr;
        std::function<void()> fnActionSave = nullptr;
        std::function<void()> fnSpawnObjectSelected = nullptr;

        EditorContext(U16 w, U16 h) : viewportWidth(w), viewportHeight(h) {
        };
    };
}
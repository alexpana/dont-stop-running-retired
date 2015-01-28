#include "gui.h"

#include <SDL2/SDL.h>

#include "../types.h"
#include "../input/input.h"
#include "../asset_loader.h"

namespace dsr {
    namespace ui {

        void imGuiCharCallback(U8 ch, io::InputAction action) {
            if (action == io::InputAction::PRESSED) {
                ImGui::GetIO().AddInputCharacter(ch);
            }
        }

        void imGuiKeyCallback(U8 ch, io::InputAction action) {
            ImGui::GetIO().KeysDown[ch] = action == io::InputAction::PRESSED;
        }

        void imGuiMouseButtonCallback(io::MouseButton button, io::InputAction action) {
            if (button == io::MouseButton::LEFT) {
                ImGui::GetIO().MouseClicked[IMGUI_MBUT_LEFT] = action == io::InputAction::PRESSED;
            } else if (button == io::MouseButton::RIGHT) {
                ImGui::GetIO().MouseClicked[IMGUI_MBUT_RIGHT] = action == io::InputAction::PRESSED;
            }
        }

        void imGuiMouseWheelCallback(F32 wheelX, F32 /*wheelY*/) {
            ImGui::GetIO().MouseWheel = wheelX;
        }

        void init() {
            uint32_t size;
            void *data = loadRawFile("data/fonts/droidsans.ttf", &size);
            imguiCreate(data, size);
            free(data);

            ImGui::GetIO().KeyMap[ImGuiKey_Tab] = io::fromSdlKey(SDLK_TAB);
            ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow] = io::fromSdlKey(SDLK_LEFT);
            ImGui::GetIO().KeyMap[ImGuiKey_RightArrow] = io::fromSdlKey(SDLK_RIGHT);
            ImGui::GetIO().KeyMap[ImGuiKey_UpArrow] = io::fromSdlKey(SDLK_UP);
            ImGui::GetIO().KeyMap[ImGuiKey_DownArrow] = io::fromSdlKey(SDLK_DOWN);
            ImGui::GetIO().KeyMap[ImGuiKey_Home] = io::fromSdlKey(SDLK_HOME);
            ImGui::GetIO().KeyMap[ImGuiKey_End] = io::fromSdlKey(SDLK_END);
            ImGui::GetIO().KeyMap[ImGuiKey_Delete] = io::fromSdlKey(SDLK_DELETE);
            ImGui::GetIO().KeyMap[ImGuiKey_Backspace] = io::fromSdlKey(SDLK_BACKSPACE);
            ImGui::GetIO().KeyMap[ImGuiKey_Enter] = io::fromSdlKey(SDLK_RETURN);
            ImGui::GetIO().KeyMap[ImGuiKey_Escape] = io::fromSdlKey(SDLK_ESCAPE);

            io::setKeyCallback(imGuiKeyCallback);
            io::setCharCallback(imGuiCharCallback);
            io::setMouseButtonCallback(imGuiMouseButtonCallback);
            io::setMouseWheelCallback(imGuiMouseWheelCallback);
        }

        void shutdown() {
            imguiDestroy();
        }
    }
}

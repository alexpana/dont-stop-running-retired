#include "gui.h"

#include <SDL2/SDL.h>

#include "../types.h"
#include "../input/input.h"
#include "../asset_loader.h"

namespace dsr {
    namespace ui {

        void imGuiCharCallback(U8 ch, input::InputAction action) {
            if (action == input::InputAction::PRESSED) {
                ImGui::GetIO().AddInputCharacter(ch);
            }
        }

        void imGuiKeyCallback(U8 ch, input::InputAction action) {
            ImGui::GetIO().KeysDown[ch] = action == input::InputAction::PRESSED;
        }

        void imGuiMouseButtonCallback(input::MouseButton button, input::InputAction action) {
            if (button == input::MouseButton::LEFT) {
                ImGui::GetIO().MouseClicked[IMGUI_MBUT_LEFT] = action == input::InputAction::PRESSED;
            } else if (button == input::MouseButton::RIGHT) {
                ImGui::GetIO().MouseClicked[IMGUI_MBUT_RIGHT] = action == input::InputAction::PRESSED;
            }
        }

        void imGuiMouseWheelCallback(F32 wheelX, F32 /*wheelY*/) {
            ImGui::GetIO().MouseWheel = wheelX;
        }

        void init() {
            uint32_t size;
            void *data = loader::loadRawFile("data/fonts/droidsans.ttf", &size);
            imguiCreate(data, size);
            free(data);

            ImGui::GetIO().KeyMap[ImGuiKey_Tab] = input::fromSdlKey(SDLK_TAB);
            ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow] = input::fromSdlKey(SDLK_LEFT);
            ImGui::GetIO().KeyMap[ImGuiKey_RightArrow] = input::fromSdlKey(SDLK_RIGHT);
            ImGui::GetIO().KeyMap[ImGuiKey_UpArrow] = input::fromSdlKey(SDLK_UP);
            ImGui::GetIO().KeyMap[ImGuiKey_DownArrow] = input::fromSdlKey(SDLK_DOWN);
            ImGui::GetIO().KeyMap[ImGuiKey_Home] = input::fromSdlKey(SDLK_HOME);
            ImGui::GetIO().KeyMap[ImGuiKey_End] = input::fromSdlKey(SDLK_END);
            ImGui::GetIO().KeyMap[ImGuiKey_Delete] = input::fromSdlKey(SDLK_DELETE);
            ImGui::GetIO().KeyMap[ImGuiKey_Backspace] = input::fromSdlKey(SDLK_BACKSPACE);
            ImGui::GetIO().KeyMap[ImGuiKey_Enter] = input::fromSdlKey(SDLK_RETURN);
            ImGui::GetIO().KeyMap[ImGuiKey_Escape] = input::fromSdlKey(SDLK_ESCAPE);

            input::setKeyCallback(imGuiKeyCallback);
            input::setCharCallback(imGuiCharCallback);
            input::setMouseButtonCallback(imGuiMouseButtonCallback);
            input::setMouseWheelCallback(imGuiMouseWheelCallback);
        }

        void shutdown() {
            imguiDestroy();
        }
    }
}

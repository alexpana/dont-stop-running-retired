#pragma once

#include <imgui.h>

#include "../engine/input/input.h"

#include "context.h"

namespace dsr {

    template<typename T, typename... Args>
    void fnCall(T fn, Args... args) {
        if (fn) {
            fn(std::forward(args...));
        }
    }

    template<typename T>
    void fnCall(T fn) {
        if (fn) {
            fn();
        }
    }


    void uiUpdate(EditorContext &ctx) {

        imguiBeginFrame((int32_t) io::mouseX(), (int32_t) io::mouseY(), (uint8_t) (mouseButtonDown(io::MouseButton::LEFT) ? IMGUI_MBUT_LEFT : 0), (U32) io::mouseWheelX(), ctx.viewportWidth, ctx.viewportHeight, 0, 0);


        ImGui::SetNextWindowPos(ImVec2(ctx.viewportWidth - 200, 0), ImGuiSetCondition_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 1));

        ImGui::Begin("Editor", &ctx.ui.editorHeaderVisible, ImVec2(200, ctx.viewportHeight), 0.3, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

        // ==============================================================
        //  Action type
        // ==============================================================
        static const char *items[] = {"select", "spawn"};
        if (ImGui::Combo("mode", &ctx.action, items, 2)) {
            fnCall(ctx.fnActionChanged);
        }

        // ==============================================================
        //  Spawn Object
        // ==============================================================
        static const char *objects[] = {"grass_tile", "dirt_tile", "saw"};
        static int item = -1;
        if (ImGui::Combo("item", &ctx.selectedSpawnObject, objects, 3)) {
            fnCall(ctx.fnSpawnObjectSelected);
        }


        ImGui::Checkbox("snap to grid", &ctx.snapToGrid);
        ImGui::Checkbox("show grid", &ctx.gridVisible);

        float scale[2];
        float rotation;

        // ==============================================================
        //  Selection
        // ==============================================================
        if (ctx.selectedEntity != nullptr) {
            if (ImGui::CollapsingHeader("Selection")) {
                ImGui::InputFloat2("Pos", (float *) &ctx.selectedEntity->position, 1);
                ImGui::InputFloat2("Scale", scale, 1);
                ImGui::InputFloat("Rot", &rotation, 1);
            }
        }

        // ==============================================================
        //  Actions
        // ==============================================================
        if (ImGui::CollapsingHeader("Actions")) {
            if (ImGui::Button("Save", ImVec2(55, 20), true)) {
                fnCall(ctx.fnActionSave);
            }

            ImGui::SameLine();

            if (ImGui::Button("Save As", ImVec2(55, 20), true)) {
                // doSaveLevel
            }

            ImGui::SameLine();

            if (ImGui::Button("Load", ImVec2(55, 20), true)) {
                fnCall(ctx.fnActionLoad);
            }
        }

        ImGui::End();
        imguiEndFrame();
    }
}
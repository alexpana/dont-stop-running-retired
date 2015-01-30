#pragma once

#include <imgui.h>

#include "../engine/input/input.h"

#include "context.h"

namespace dsr {

    void uiUpdate(EditorContext &ctx) {

        imguiBeginFrame((int32_t) io::mouseX(), (int32_t) io::mouseY(), (uint8_t) (mouseButtonDown(io::MouseButton::LEFT) ? IMGUI_MBUT_LEFT : 0), (U32) io::wheelX(), ctx.viewportWidth, ctx.viewportHeight, 0, 0);


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
            if (ctx.fnActionChanged) {
                ctx.fnActionChanged();
            }
        }

        // ==============================================================
        //  Spawn Object
        // ==============================================================
        static const char *objects[] = {"grass_tile", "dirt_tile", "saw"};
        if (ImGui::Combo("item", &ctx.selectedSpawnObject, objects, 3)) {
            if (ctx.fnSpawnObjectSelected) {
                ctx.fnSpawnObjectSelected();
            }
        }

        ImGui::Checkbox("snap to grid", &ctx.snapToGrid);
        ImGui::Checkbox("show grid", &ctx.gridVisible);

        float scale[2];
        float rotation;

        // ==============================================================
        //  Selection
        // ==============================================================
        if (ctx.selectedEntity() != nullptr) {
            if (ImGui::CollapsingHeader("Selection")) {
                ImGui::InputFloat2("Pos", (float *) &ctx.selectedEntity()->position, 1);
                ImGui::InputFloat2("Scale", scale, 1);
                ImGui::InputFloat("Rot", &rotation, 1);
            }
        }

        // ==============================================================
        //  Actions
        // ==============================================================
        if (ImGui::CollapsingHeader("Actions")) {
            if (ImGui::Button("Save", ImVec2(55, 20), true)) {
                if (ctx.fnActionSave) {
                    if (ctx.saveFilename == "") {
                        ctx.saveFilename = saveFileDialog();
                    }

                    if (ctx.saveFilename != "") {
                        ctx.fnActionSave(ctx.saveFilename);
                    }
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Save As", ImVec2(55, 20), true)) {
                if (ctx.fnActionSave) {
                    ctx.saveFilename = saveFileDialog();

                    if (ctx.saveFilename != "") {
                        ctx.fnActionSave(ctx.saveFilename);
                    }
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Load", ImVec2(55, 20), true)) {
                if (ctx.fnActionLoad) {
                    auto filename = ctx.fnActionLoad();
                    if (filename != "") {
                        ctx.saveFilename = filename;
                    }
                }
            }
        }

        ImGui::End();
        imguiEndFrame();
    }
}
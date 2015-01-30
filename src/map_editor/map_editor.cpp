#include "map_editor.h"

#include <SDL2/SDL.h>
#include <bx/bx.h>
#include <bgfx.h>
#include <bgfxplatform.h>
#include <glm/gtx/rotate_vector.hpp>
#include <nanovg.h>
#include <sstream>
#include <fstream>
#include <bits/stl_algo.h>

#include "../engine/input/input.h"
#include "../engine/ui/gui.h"
#include "../engine/assets.h"
#include "../engine/asset_loader.h"
#include "../engine/bgfx_utils.h"
#include "../engine/particles.h"
#include "../engine/renderer.h"
#include "../engine/filesystem.h"

#include "context.h"
#include "editor_ui.h"
#include "export.h"

static dsr::Log _log{"map_editor"};

using namespace std;

static const U16 WIDTH = 1400;
static const U16 HEIGHT = 800;

dsr::ParticleGenerator createParticleGenerator();

void loadAssets();

static dsr::EditorContext sContext(WIDTH, HEIGHT);

namespace dsr {

    void loadShader(std::string shaderName) {
        auto shaderPtr = loader::loadShader("data/shaders/" + shaderName);
        if (shaderPtr) {
            dsr::Assets::instance().registerShader(shaderName, std::move(shaderPtr));
        }
    }

    void loadTexture(std::string textureName) {
        auto texturePtr = loader::loadTexture("data/textures/" + textureName + ".dds");
        if (texturePtr) {
            dsr::Assets::instance().registerTexture(textureName, std::move(texturePtr));
        }
    }

    void loadAssets() {
        auto &assets = dsr::Assets::instance();

        // load textures
        loadTexture("tiles");

        // load sprites
        auto sprites = loader::loadSprites("data/scripts/sprites.json");
        for (auto &sprite : sprites) {
            assets.registerSprite(sprite->name, std::move(sprite));
        }

        // load objects
        auto staticTiles = loader::loadStaticTiles("data/scripts/static_tiles.json");
        for (auto &staticTile : staticTiles) {
            assets.registerGameObject(staticTile->getName(), std::move(staticTile));
        }

        // load shaders
        loadShader("particles");
        loadShader("default");
        loadShader("sprite");
    }

    dsr::ParticleGenerator createParticleGenerator() {
        dsr::ParticleGenerator generator;
        generator.generatorSpawnFrequency = 32;

        generator.generatorPosition = glm::vec2(300, 300);

        generator.generatorSpawnRadius = 2.0;

        generator.generatorSpawnArc = 3.14;

        generator.generatorSpawnDirection = glm::vec2(1.0f, 0.0f);

        generator.params.lifetime = {dsr::TimeUnit::fromSeconds(3), dsr::TimeUnit::fromSeconds(5)};

        generator.params.startScale = {1, 2};
        generator.params.endScale = {6, 10};

        generator.params.startSpeed = {2, 4};
        generator.params.endSpeed = {0.1, 0.6};

        generator.params.startRotation = {0, 0};
        generator.params.endRotation = {4, 6};

        generator.params.startAlpha = {1, 1};
        generator.params.endAlpha = {0, 0};

        generator.params.startColor = {0x324050ff, 0x324050ff};
        generator.params.endColor = {0x600090ff, 0x604090ff};
        return generator;
    }

    void drawSelectionOutline() {
        auto *entity = sContext.selectedEntity();
        if (entity) {
            glm::vec2 position = entity->position;
            glm::vec2 scale = entity->scale;
            glm::vec2 size = entity->gameObject->getSize();

            F32 rotation = entity->rotation;

            auto c = nvgCtx();

            nvgRotate(c, rotation);
            nvgScale(c, scale.x, scale.y);
            nvgTranslate(c, position.x, position.y);
            nvgBeginPath(c);
            nvgMoveTo(c, -size.x / 2, -size.y / 2);
            nvgLineTo(c, size.x / 2, -size.y / 2);
            nvgLineTo(c, size.x / 2, size.y / 2);
            nvgLineTo(c, -size.x / 2, size.y / 2);
            nvgLineTo(c, -size.x / 2, -size.y / 2);
            nvgClosePath(c);
            nvgResetTransform(c);

            nvgStrokeWidth(c, 2);
            nvgStrokeColor(c, NVGcolor{0, 0, 0, 1.0});
            nvgStroke(c);

        }
    }

    void updateTitle();

    void renderEntity(const LevelMap::Entity &entity) {
        glm::mat4 transform = glm::translate(glm::vec3(entity.position, 0));
        bgfx::setTransform(&transform);
        entity.gameObject->render();
    }

    glm::vec2 getMouseActionPosition() {
        if (sContext.snapToGrid) {
            return glm::vec2((int) (input::mouseX() / 32.0) * 32, (int) (input::mouseY() / 32.0) * 32);
        } else {
            return glm::vec2(input::mouseX(), input::mouseY());
        }
    }

    int runMapEditor() {
        std::string windowName = "Don't Stop Running";

        dsr::initBgfx(WIDTH, HEIGHT, windowName);
        ui::init();

        updateTitle();

        int r = nvgCreateFont(nvgCtx(), "droid", "data/fonts/droidsans.ttf");
        nvgFontFaceId(nvgCtx(), r);

        loadAssets();

        Assets &assets = dsr::Assets::instance();

        dsr::ParticleSystem particleSystem;

        dsr::ParticleGenerator generator = createParticleGenerator();
        particleSystem.addGenerator(&generator);

        F32 emitterSpeed = 2.0;

        int viewType = 0;

        LevelMap levelMap{};

        levelMap.entities.push_back(LevelMap::Entity{
                glm::vec2(128, 128),
                glm::vec2(1.0, 1.0),
                0.0f,
                assets.findGameObject("saw")});

        auto spawningEntity = LevelMap::Entity{};
        spawningEntity.position = {0, 0};

        spawningEntity.gameObject = assets.findGameObject("saw");

        sContext.fnSpawnObjectSelected = [&spawningEntity, &assets]() {
            const char *objects[3] = {"grass_tile", "dirt_tile", "saw"};
            spawningEntity.gameObject = assets.findGameObject(objects[sContext.selectedSpawnObject]);
        };

        sContext.fnActionSave = [&levelMap](std::string filename) {
            if (filename != "") {
                exportLevelMap(filename, levelMap);
                sContext.changedSinceLastSave = false;
                updateTitle();
            }
        };

        sContext.selectedEntityIndex = 0;

        while (!input::frameCloseRequested()) {
            input::update();

            uiUpdate(sContext);

            bool canUseInput = !ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().WantCaptureKeyboard;

            bool hoveringMenu = input::mouseX() >= WIDTH - 200;

            nvgBeginFrame(nvgCtx(), sContext.viewportWidth, sContext.viewportHeight, 1.0f);

            if (sContext.gridVisible) {
                nvgStrokeWidth(nvgCtx(), 0.5f);

                nvgStrokeColor(nvgCtx(), {0.7, 0.7, 0.7, 1.0});

                nvgBeginPath(nvgCtx());

                for (int i = 0; i < sContext.viewportWidth / 32 + 1; ++i) {
                    nvgMoveTo(nvgCtx(), (float) ((i * 32) + 0.5), 0);

                    nvgLineTo(nvgCtx(), (float) ((i * 32) + 0.5), sContext.viewportHeight);
                }

                for (int i = 0; i < sContext.viewportHeight / 32 + 1; ++i) {
                    nvgMoveTo(nvgCtx(), 0, (float) ((i * 32) + 0.5));

                    nvgLineTo(nvgCtx(), sContext.viewportWidth, (float) ((i * 32) + 0.5));
                }

                nvgClosePath(nvgCtx());

                nvgStroke(nvgCtx());
            }

            if (sContext.cursorLocationVisible && !hoveringMenu) {
                std::stringstream ss;
                ss << input::mouseX() << " " << input::mouseY();

                float bounds[4];
                nvgTextBounds(nvgCtx(), input::mouseX() + 15, input::mouseY() + 15, ss.str().c_str(), nullptr, bounds);
                bounds[2] += 10.0;
                bounds[3] += 10.0;

                nvgFillColor(nvgCtx(), {0, 0, 0, 0.4});
                nvgBeginPath(nvgCtx());
                nvgRoundedRect(nvgCtx(), bounds[0], bounds[1], bounds[2] - bounds[0], bounds[3] - bounds[1], 2.0f);
                nvgClosePath(nvgCtx());
                nvgFill(nvgCtx());

                nvgFillColor(nvgCtx(), {0.8, 0.8, 0.8, 1.0});
                nvgText(nvgCtx(), input::mouseX() + 20, input::mouseY() + 20, ss.str().c_str(), nullptr);
            }

            drawSelectionOutline();
            nvgEndFrame(nvgCtx());

            if (canUseInput) {
                if (input::keyPressed(input::fromSdlKey(SDLK_1))) {
                    viewType = 1;
                }
                if (input::keyPressed(input::fromSdlKey(SDLK_2))) {
                    viewType = 2;
                }
                if (input::keyPressed(input::fromSdlKey(SDLK_3))) {
                    viewType = 3;
                }
            }

            glm::vec2 direction;
            direction = glm::vec2(input::mouseX(), input::mouseY()) - glm::vec2(generator.generatorPosition[0], generator.generatorPosition[1]);
            direction = glm::normalize(direction);
            direction *= emitterSpeed;

            if (viewType == 1) {
                generator.generatorPosition = direction;
            }

            if (viewType == 2) {
                generator.generatorPosition += direction;
            }

            if (viewType == 3) {
                generator.generatorSpawnDirection = glm::rotate(generator.generatorSpawnDirection, 0.2f);
            }

            // draw map
            for (auto &it : levelMap.entities) {
                renderEntity(it);

            }

            spawningEntity.position = getMouseActionPosition();

            if (!hoveringMenu) {
                if (sContext.action == EditorContext::Action::SPAWN) {
                    // render currently spawning entity
                    renderEntity(spawningEntity);

                    // spawn entity
                    if (input::mouseButtonPressed(input::MouseButton::LEFT)) {
                        levelMap.entities.push_back(spawningEntity);
                        sContext.changedSinceLastSave = true;
                        updateTitle();
                    }
                } else {
                    // TODO: handle selection
                }
            }

            bgfx::frame();
        }

        Assets::destroyInstance();

        ui::shutdown();
        destroyBgfx();

        return 0;
    }

    void updateTitle() {
        setWindowTitle("Map Editor: " + (sContext.saveFilename != "" ? sContext.saveFilename : "<untitled>") + (sContext.changedSinceLastSave ? "*" : ""));
    }
}
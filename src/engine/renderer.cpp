#include <glm/detail/type_vec.hpp>
#include <bgfx.h>
#include <glm/gtx/transform.hpp>

#include "renderer.h"
#include "types.h"
#include "sprite.h"
#include "assets.h"

namespace dsr {

    // vertex and index values for a quad
    static U16 sIndexData[] = {0, 1, 2, 0, 2, 3};

    void setVertex(F32 *vertexData, int index, glm::vec2 pos, glm::vec2 uv) {
        vertexData[index * 4 + 0] = pos.x;
        vertexData[index * 4 + 1] = pos.y;
        vertexData[index * 4 + 2] = uv.x;
        vertexData[index * 4 + 3] = uv.y;
    }

    static bgfx::VertexDecl getVertexDecl() {
        static bool initialized = false;
        static bgfx::VertexDecl decl;

        if (!initialized) {
            initialized = true;
            decl.begin();
            decl.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float);
            decl.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
            decl.end();
        }

        return decl;
    }

    void renderSprite(const glm::vec2 &position, const Sprite &sprite, const U32 depth) {
        using glm::vec2;

        auto texture = Assets::instance().findTexture(sprite.sheetName);

        if (texture == nullptr) {
            return;
        }

        float w = sprite.size.x;
        float h = sprite.size.y;

        float ox = sprite.offset.x;
        float oy = sprite.offset.y;

        int tw = texture->getInfo().width;
        int th = texture->getInfo().height;

        bgfx::UniformHandle u_texColor = bgfx::createUniform("u_texColor", bgfx::UniformType::Uniform1iv);

        F32 *sVertexData = new F32[16];
        setVertex(sVertexData, 0, vec2{0, 0}, {ox / tw, oy / th}); // uv: 0, 0
        setVertex(sVertexData, 1, vec2{0, h}, {(ox + w) / tw, oy / th}); // uv: 0, 1
        setVertex(sVertexData, 2, vec2{w, h}, {(ox + w) / tw, (oy + h) / th}); // uv: 1, 1
        setVertex(sVertexData, 3, vec2{w, 0}, {ox / tw, (oy + h) / th}); // uv: 1, 0

        auto vBuffer = bgfx::createVertexBuffer(bgfx::makeRef(sVertexData, 16 * sizeof(F32)), getVertexDecl());
        auto iBuffer = bgfx::createIndexBuffer(bgfx::makeRef(sIndexData, sizeof(sIndexData)));

        glm::mat4 transform = glm::translate(glm::vec3(position, 0));

        bgfx::setTransform(&transform);

        bgfx::setVertexBuffer(vBuffer);
        bgfx::setIndexBuffer(iBuffer);
        bgfx::setProgram(Assets::instance().findShader("sprite")->getHandle());
        bgfx::setTexture(0, u_texColor, texture->getHandle());
        bgfx::setState(0
                | BGFX_STATE_RGB_WRITE
                | BGFX_STATE_ALPHA_WRITE
                | BGFX_STATE_DEPTH_WRITE
                | BGFX_STATE_DEPTH_TEST_ALWAYS
                | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
                | BGFX_STATE_BLEND_EQUATION_ADD
        );

        bgfx::submit(0, depth);

        bgfx::destroyVertexBuffer(vBuffer);
        bgfx::destroyIndexBuffer(iBuffer);
        bgfx::destroyUniform(u_texColor);
    }

    void renderSprite(const Sprite &sprite, U32 const depth) {
        using glm::vec2;

        auto texture = Assets::instance().findTexture(sprite.sheetName);

        if (texture == nullptr) {
            return;
        }

        float w = sprite.size.x;
        float h = sprite.size.y;

        float ox = sprite.offset.x;
        float oy = sprite.offset.y;

        int tw = texture->getInfo().width;
        int th = texture->getInfo().height;

        bgfx::UniformHandle u_texColor = bgfx::createUniform("u_texColor", bgfx::UniformType::Uniform1iv);

        // TODO: fix memory leak!!
        F32 *sVertexData = new F32[16];
        setVertex(sVertexData, 0, vec2{0, 0}, {ox / tw, oy / th}); // uv: 0, 0
        setVertex(sVertexData, 1, vec2{0, h}, {(ox + w) / tw, oy / th}); // uv: 0, 1
        setVertex(sVertexData, 2, vec2{w, h}, {(ox + w) / tw, (oy + h) / th}); // uv: 1, 1
        setVertex(sVertexData, 3, vec2{w, 0}, {ox / tw, (oy + h) / th}); // uv: 1, 0

        auto vBuffer = bgfx::createVertexBuffer(bgfx::makeRef(sVertexData, 16 * sizeof(F32)), getVertexDecl());
        auto iBuffer = bgfx::createIndexBuffer(bgfx::makeRef(sIndexData, sizeof(sIndexData)));

        bgfx::setVertexBuffer(vBuffer);
        bgfx::setIndexBuffer(iBuffer);
        bgfx::setProgram(Assets::instance().findShader("sprite")->getHandle());
        bgfx::setTexture(0, u_texColor, texture->getHandle());
        bgfx::setState(0
                | BGFX_STATE_RGB_WRITE
                | BGFX_STATE_ALPHA_WRITE
                | BGFX_STATE_DEPTH_WRITE
                | BGFX_STATE_DEPTH_TEST_ALWAYS
                | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
                | BGFX_STATE_BLEND_EQUATION_ADD
        );

        bgfx::submit(0, depth);

        bgfx::destroyVertexBuffer(vBuffer);
        bgfx::destroyIndexBuffer(iBuffer);
        bgfx::destroyUniform(u_texColor);
    }

    void renderRect(const glm::vec2 &size, const U32 color, const U32 depth) {

    }
}
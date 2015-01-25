#include <glm/detail/type_vec.hpp>
#include <bgfx.h>

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

    void renderSprite(const glm::vec2 &position, const Sprite &sprite) {
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
        setVertex(sVertexData, 0, position + vec2{0, 0}, {oy / th, ox / tw}); // uv: 0, 0
        setVertex(sVertexData, 1, position + vec2{0, h}, {oy / th, (ox + w) / tw}); // uv: 0, 1
        setVertex(sVertexData, 2, position + vec2{w, h}, {(oy + h) / th, (ox + w) / tw}); // uv: 1, 1
        setVertex(sVertexData, 3, position + vec2{w, 0}, {(oy + h) / th, ox / tw}); // uv: 1, 0

        auto vBuffer = bgfx::createVertexBuffer(bgfx::makeRef(sVertexData, 16 * sizeof(F32)), getVertexDecl());
        auto iBuffer = bgfx::createIndexBuffer(bgfx::makeRef(sIndexData, sizeof(sIndexData)));

        bgfx::setVertexBuffer(vBuffer);
        bgfx::setIndexBuffer(iBuffer);
        bgfx::setProgram(Assets::instance().findShader("sprite")->getHandle());
        bgfx::setTexture(0, u_texColor, texture->getHandle());
        bgfx::setState(BGFX_STATE_RGB_WRITE
                | BGFX_STATE_ALPHA_WRITE
                | BGFX_STATE_DEPTH_WRITE
                | BGFX_STATE_DEPTH_TEST_LESS
                | BGFX_STATE_MSAA);

        bgfx::submit(0);

        bgfx::destroyVertexBuffer(vBuffer);
        bgfx::destroyIndexBuffer(iBuffer);
//        bgfx::destroyUniform(u_texColor);
    }
}
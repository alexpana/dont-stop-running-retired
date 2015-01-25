#pragma once

#include <bgfx.h>

#include "types.h"

namespace bgfx {

    bgfx::ShaderHandle loadShader(const char *name);

    bgfx::ProgramHandle loadProgram(const char *name);

    bgfx::TextureHandle loadTexture(const char *name, U32 flags = BGFX_TEXTURE_NONE, U8 skip = 0, bgfx::TextureInfo *info = nullptr);

    void calcTangents(void *vertices, U16 numVertices, bgfx::VertexDecl decl, const U16 *indices, U32 numIndices);

    struct TimeUnit {
        /**
        * Time unit value, measured in CPU cycles.
        */
        I64 value;

        TimeUnit(I64 value) : value(value) {
        }

        F32 toSeconds() {
            return value / static_cast<F32>(bx::getHPFrequency());
        }

        F32 toMilliseconds() {
            return toSeconds() * 1000;
        }

        operator I64() {
            return value;
        }

        static I64 fromSeconds(F32 seconds) {
            return static_cast<I64>(seconds * bx::getHPFrequency());
        };

        static I64 fromMilliseconds(F32 milliseconds) {
            return static_cast<I64>((milliseconds / 1000) * bx::getHPFrequency());
        };

        static TimeUnit counter() {
            return TimeUnit{bx::getHPCounter()};
        }
    };

    static I64 msec(F32 milliseconds) {
        return TimeUnit::fromMilliseconds(milliseconds);
    }

    static I64 sec(F32 seconds) {
        return TimeUnit::fromSeconds(seconds);
    }

    static U8 redChannel(const U32 color) {
        return (U8) (color >> 24 & 0xff);
    }

    static U8 greenChannel(const U32 color) {
        return (U8) (color >> 16 & 0xff);
    }

    static U8 blueChannel(const U32 color) {
        return (U8) (color >> 8 & 0xff);
    }

    static U8 alphaChannel(const U32 color) {
        return (U8) (color >> 0 & 0xff);
    }
}

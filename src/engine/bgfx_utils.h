#pragma once

#include <string>

#include <bgfx.h>
#include <bx/timer.h>

#include "types.h"

namespace dsr {
        void initBgfx(uint16_t width, uint16_t height, const std::string &windowName);

        void calcTangents(void *vertices, U16 numVertices, bgfx::VertexDecl decl, const U16 *indices, U32 numIndices);

        struct TimeUnit {
            /**
            * Time unit value, measured in CPU cycles.
            */
            S64
                    value;

            TimeUnit(S64 value) : value(value) {
            }

            F32 toSeconds() {
                return value / static_cast<F32>(bx::getHPFrequency());
            }

            F32 toMilliseconds() {
                return toSeconds() * 1000;
            }

            operator S64() {
                return value;
            }

            static S64 fromSeconds(F32 seconds) {
                return static_cast<S64>(seconds * bx::getHPFrequency());
            };

            static S64 fromMilliseconds(F32 milliseconds) {
                return static_cast<S64>((milliseconds / 1000) * bx::getHPFrequency());
            };

            static TimeUnit counter() {
                return TimeUnit{bx::getHPCounter()};
            }
        };

        static S64 msec(F32 milliseconds) {
            return TimeUnit::fromMilliseconds(milliseconds);
        }

        static S64 sec(F32 seconds) {
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

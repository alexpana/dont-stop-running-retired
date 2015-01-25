#pragma once

#include <bgfx.h>
#include <GL/gl.h>

namespace dsr {

    /**
    * Resource holder for a bgfx TextureHandle.
    * Does not allow copying, only moving.
    */
    class Texture {
    public:
        Texture(bgfx::TextureHandle handle, bgfx::TextureInfo info) : handle(handle), info(info) {
        }

        Texture() {
            handle.idx = bgfx::invalidHandle;
        }

        Texture(const Texture &other) = delete;

        Texture &operator=(Texture &other) = delete;

        Texture &operator=(Texture &&other) {
            handle = std::move(other.handle);
            info = std::move(other.info);

            other.handle.idx = bgfx::invalidHandle;

            return *this;
        }

        Texture(Texture &&other) {
            handle = std::move(other.handle);
            info = std::move(other.info);

            other.handle.idx = bgfx::invalidHandle;
        };

        ~Texture() {
            if (isValid()) {
                bgfx::destroyTexture(handle);
            }
        }

        bgfx::TextureHandle &getHandle() {
            return handle;
        }

        bgfx::TextureInfo getInfo() {
            return info;
        }

        bool isValid() {
            return bgfx::isValid(handle);
        }

    private:
        bgfx::TextureHandle handle;
        bgfx::TextureInfo info;
    };
}
#pragma once

#include <bgfx.h>

namespace dsr {

    /**
    * Resource holder for a bgfx ProgramHandle.
    * Does not allow copying, only moving.
    */
    class Shader {
    public:
        Shader() {
            programHandle.idx = bgfx::invalidHandle;
        }

        Shader(bgfx::ProgramHandle handle) : programHandle(handle) {
        }

        Shader(const Shader &other) = delete;

        Shader &operator=(const Shader &other) = delete;

        Shader &operator=(Shader &&other) {
            this->programHandle = other.programHandle;
            other.programHandle.idx = bgfx::invalidHandle;
            return *this;
        }

        bgfx::ProgramHandle &getHandle() {
            return programHandle;
        }

        ~Shader() {
            if (isValid()) {
                bgfx::destroyProgram(programHandle);
            }
        }


        bool isValid() {
            return programHandle.idx != bgfx::invalidHandle;
        }

    private:
        bgfx::ProgramHandle programHandle;
    };
}
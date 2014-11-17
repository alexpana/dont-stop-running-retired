#pragma once

#include <string>
#include <iostream>

#include <SDL_surface.h>

namespace engine {

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Texture
{
public:
    Texture(SDL_Texture *texture) : m_texture(texture) {
        SDL_QueryTexture(texture, nullptr, nullptr, &m_width, &m_height);
    }

    ~Texture() {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    SDL_Texture* getNative() {
        return m_texture;
    }

    int getWidth() {
        return m_width;
    }

    int getHeight() {
        return m_height;
    }

    static TexturePtr create(SDL_Texture *native) {
        return std::make_shared<Texture>(native);
    }

private:
    SDL_Texture *m_texture;

    int m_width;
    int m_height;
};

}
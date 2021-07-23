#ifndef RENDER_MANAGER_HPP_INCLUDED
#define RENDER_MANAGER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <vec/vec.hpp>
#include <gl/gl.h>

struct vertex
{
    vec2f position; ///[opengl]
    vec4f colour; ///[0 -> 1], linear
    vec2f uv; ///normalised
};

struct texture
{
    vec2i dim;
    GLuint tex_handle = 0;

    void load_from_file(const std::string& file);
    void load_from_image(const sf::Image& image);
};

struct render_manager
{
    void draw(const std::vector<vertex>& vertices, sf::PrimitiveType type, texture* tex = nullptr);
    void draw(const sf::Shape& shape);
};

#endif // RENDER_MANAGER_HPP_INCLUDED

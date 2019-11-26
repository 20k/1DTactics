#include <gl/glew.h>
#include "render_window.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void render_window::draw(const std::vector<vertex>& vertices, sf::PrimitiveType type, texture* tex)
{
    ImDrawList* idl = ImGui::GetBackgroundDrawList(ImGui::GetMainViewport());

    if(tex)
    {
        assert(tex->dim.x() > 0);
        assert(tex->dim.y() > 0);

        idl->PushTextureID((void*)tex->tex_handle);
    }
    else
    {
        idl->PushTextureID(ImGui::GetIO().Fonts->TexID);
    }

    idl->PrimReserve(vertices.size(), vertices.size());
    ImDrawVert* vtx_write = idl->_VtxWritePtr;
    ImDrawIdx* idx_write = idl->_IdxWritePtr;
    unsigned int vtx_current_idx = idl->_VtxCurrentIdx;

    for(int i=0; i < (int)vertices.size(); i++)
    {
        vtx_write[i].pos.x = vertices[i].position.x();
        vtx_write[i].pos.y = vertices[i].position.y();

        vtx_write[i].uv.x = vertices[i].uv.x();
        vtx_write[i].uv.y = vertices[i].uv.y();

        vec3f srgb_col = lin_to_srgb(vertices[i].colour.xyz()) * 255.f;

        vtx_write[i].col = IM_COL32((int)srgb_col.x(), (int)srgb_col.y(), (int)srgb_col.z(), 255);

        idx_write[i] = vtx_current_idx + i;
    }

    idl->_VtxWritePtr += vertices.size();
    idl->_IdxWritePtr += vertices.size();
    idl->_VtxCurrentIdx += vertices.size();

    idl->PopTextureID();
}

void render_window::draw(const sf::Shape& shape)
{
    int vcount = shape.getPointCount();

    assert(vcount >= 3);

    vec2f centre_pos = {0,0};

    for(int i=0; i < vcount; i++)
    {
        auto pos = shape.getTransform() * shape.getPoint(i);

        centre_pos += (vec2f){pos.x, pos.y} / (float)vcount;
    }

    auto scol = shape.getFillColor();
    auto lin_col = srgb_to_lin_approx((vec3f){scol.r, scol.g, scol.b}/255.f);

    std::vector<vertex> vertices;

    for(int i=0; i < vcount; i++)
    {
        int next = (i + 1) % vcount;

        auto real_pos_1 = shape.getTransform() * shape.getPoint(i);
        auto real_pos_2 = shape.getTransform() * shape.getPoint(next);

        vertex vert;
        vert.position = {real_pos_1.x, real_pos_1.y};
        vert.colour = {lin_col.x(), lin_col.y(), lin_col.z(), scol.a};
        vert.uv = {ImGui::GetDrawListSharedData()->TexUvWhitePixel.x, ImGui::GetDrawListSharedData()->TexUvWhitePixel.y};

        vertex vert2 = vert;
        vert2.position = {real_pos_2.x, real_pos_2.y};

        vertex centre = vert;
        centre.position = centre_pos;

        vertices.push_back(vert);
        vertices.push_back(vert2);
        vertices.push_back(centre);
    }

    draw(vertices, sf::PrimitiveType::Triangles, nullptr);
}

void texture::load_from_file(const std::string& file)
{
    int width, height, nrchannels;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrchannels, 4);

    if(!data)
        throw std::runtime_error("Could not load from file " + file);

    dim = {width, height};

    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmapEXT(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void texture::load_from_image(const sf::Image& image)
{
    dim = {image.getSize().x, image.getSize().y};

    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x(), dim.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmapEXT(GL_TEXTURE_2D);
}

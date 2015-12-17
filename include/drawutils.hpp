#ifndef DRAWUTILS_HPP
#define DRAWUTILS_HPP 1

#include "gfxapi.glcommon.hpp"

#include <memory>

namespace GfxApi{
    class Mesh;
    class ShaderProgram;
    class Texture;
}



std::shared_ptr<GfxApi::Mesh> axes(float width = .1, float length = 5.0);
std::shared_ptr<GfxApi::Mesh> simpletri();


std::shared_ptr<GfxApi::Mesh> reguizdemo();

std::shared_ptr<GfxApi::Mesh> basictexture();


std::shared_ptr<GfxApi::Texture> gentesttexture(int width, int height);


///Screen-Space Textured Quad
std::shared_ptr<GfxApi::Mesh> SSTQRGBD(bool indexed);
std::shared_ptr<GfxApi::ShaderProgram> SSTQRGBDShader();


std::shared_ptr<GfxApi::Mesh> SSTQRGBD2(bool indexed);

#endif

#ifndef SGFXAPI_DRAWUTILS_HPP
#define SGFXAPI_DRAWUTILS_HPP 1

#include "sgfxapi/sgfxapi.glcommon.hpp"

#include <memory>

namespace SGFXAPI{
    class Mesh;
    class ShaderProgram;
    class Texture;
}



std::shared_ptr<SGFXAPI::Mesh> axes(float width = .1, float length = 5.0);
std::shared_ptr<SGFXAPI::Mesh> simpletri();


std::shared_ptr<SGFXAPI::Mesh> reguizdemo();

std::shared_ptr<SGFXAPI::Mesh> basictexture();


std::shared_ptr<SGFXAPI::Texture> gentesttexture(int width, int height);


///Screen-Space Textured Quad
std::shared_ptr<SGFXAPI::Mesh> SSTQRGBD(bool indexed);
std::shared_ptr<SGFXAPI::ShaderProgram> SSTQRGBDShader();


std::shared_ptr<SGFXAPI::Mesh> SSTQRGBD2(bool indexed);

#endif

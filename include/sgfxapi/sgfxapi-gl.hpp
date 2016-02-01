#ifndef SGFXAPI_SGFXAPI_GL_H
#define SGFXAPI_SGFXAPI_GL_H 1


#include "sgfxapi/sgfxapi.hpp"
#include "sgfxapi/sgfxapi.glcommon.hpp"

#include <stdint.h>

//must incldue assert first, else mathgeolib defines it
#include <cassert>
#include "MathGeoLib.h"


namespace SGFXAPI {

GLenum toGL(VertexDataType type);
GLenum toGL(Access access);
GLenum toGL(Usage usage);
GLenum toGL(TextureInternalFormat internalformat);
GLenum toGL(TexturePixelFormat pixelformat);
GLenum toGL(TextureElementType elementtype);
GLenum toGL(TextureType texturetype);
GLenum toGL(PrimitiveIndexType indextype);
GLenum toGL(TextureFilterMode filtermode);
GLenum toGL(ShaderType type);
GLint toGL(TextureAddressMode addressmode);

GLenum toGLBinding(TextureType texturetype);



GLuint getHandle(const ShaderProgram& sp);

/// Retrieve the opengl handle for the sampler.
GLuint getHandle(const TextureSampler& sampler);
GLuint getHandle(const Texture& texture);
GLuint getHandle(const Shader& shader);
GLuint getHandle(const IndexBuffer& ib);


} // namespace SGFXAPI



#endif

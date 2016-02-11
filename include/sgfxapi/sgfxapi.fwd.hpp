#ifndef SGFXAPI_SGFXAPI_FWD_H
#define SGFXAPI_SGFXAPI_FWD_H 1



#include <stdint.h>
#include <iosfwd>


namespace SGFXAPI {

class VertexBuffer;
class IndexBuffer;
class ShaderProgram;
class Mesh;
class Texture;
class TextureUnit;
class TextureSampler;


enum class Access;
enum class Usage;
enum class VertexDataSemantic;
enum class VertexDataType;
enum class GPUVertexDataType;
enum class ShaderType;
enum class PrimitiveType;
enum class PrimitiveIndexType;
enum class ResourceUsage;
enum class TextureType;
enum class TextureAddressMode;
enum class TextureFilterMode;
enum class TextureElementType;
enum class TexturePixelFormat;
enum class TextureInternalFormat;


} // namespace SGFXAPI



#endif

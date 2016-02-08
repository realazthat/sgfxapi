#include "sgfxapi/sgfxapi.hpp"
#include "sgfxapi/sgfxapi-gl.hpp"

#include <GLFW/glfw3.h>

#include <set>
#include <map>
#include <sstream>
#include <cstring>

#include <assert.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>


namespace SGFXAPI {


////////////////////////////////////////////////////////////////////////////////
// Pimpl declarations
////////////////////////////////////////////////////////////////////////////////
struct ShaderProgramPimpl{
    GLuint m_programHandle;
};

struct TexturePimpl{
    GLuint m_tex;
    TextureType m_texture_type;
    TextureInternalFormat m_internal_format;
    ResourceUsage m_usage;
    int m_width, m_height, m_depth, m_rowalignment, m_mipmaps;
};

struct TextureSamplerPimpl{
    GLuint m_smplr;
};
struct ShaderPimpl{
    GLuint m_shaderHandle;
    ShaderType m_type;
};

struct MeshPimpl{

    GLuint m_vao;
    int m_numVertices;
    ///If GenerateVAO() is called with a startVertexOffset > 0, the number
    /// of renderable vertices will be less.
    int m_numRenderableVertices;
    VertexDeclaration m_declaration;

    /**
    * The primitive type that is used for rendering.
    */
    PrimitiveType m_primType;
};

struct VertexBufferPimpl{
    std::weak_ptr<Mesh> m_mesh;
    
    int m_numVertices;
    VertexDeclaration m_declaration;
    Usage m_usage;
    GLuint m_vbo;
    int m_gpuSize;
    std::shared_ptr< VertexBuffer::cpu_data_t > m_cpuData;
};
struct IndexBufferPimpl{
    std::weak_ptr<Mesh> m_mesh;
    int m_numIndices;
    PrimitiveIndexType m_indexType;
    Usage m_usage;

    std::shared_ptr< IndexBuffer::cpu_data_t > m_indexData;
    
    GLuint m_indexBuffer;
    int m_gpuSize;
};
struct PixelBufferPimpl{
    GLuint m_pbo;
    Usage m_usage;
    std::size_t m_logicalBytes;
    int m_gpuSize;


    std::shared_ptr< PixelBuffer::cpu_data_t > m_cpuData;
};

struct FencePimpl{
    GLsync m_fence;
    bool m_signaled;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeGlew()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::string msg = std::string("Error initializing glew: ") + reinterpret_cast<const char*>(glewGetErrorString(err));
        throw std::runtime_error(msg);
    }
}

int checkOglError(const char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;
    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        std::ostringstream msg;
        msg << "glError in file " << file << " @ line " << line << ": " << gluErrorString(glErr) << "\n";
        
        //std::string msg = (f::format("glError in file %s @ line %d: %s\n")  % file % line % gluErrorString(glErr)).str();
        throw std::runtime_error(msg.str());
        retCode = 1;
    }
    return retCode;
}
#define checkOpenGLError() checkOglError(__FILE__, __LINE__)


GLenum toGL(VertexDataType type)
{
    switch (type)
    {
        case (VertexDataType::BYTE): return GL_BYTE;
        case (VertexDataType::UNSIGNED_BYTE): return GL_UNSIGNED_BYTE;
        case (VertexDataType::UNSIGNED_SHORT): return GL_UNSIGNED_SHORT;
        case (VertexDataType::SHORT): return GL_SHORT;
        case (VertexDataType::INT): return GL_INT;
        case (VertexDataType::UNSIGNED_INT): return GL_UNSIGNED_INT;
        case (VertexDataType::FLOAT): return GL_FLOAT;
        case (VertexDataType::DOUBLE): return GL_DOUBLE;
    }
    assert(false);
    return GL_NONE;
}

GLenum toGL(Access access)
{
    switch (access)
    {
        case (Access::READ_ONLY): return GL_READ_ONLY;
        case (Access::WRITE_ONLY): return GL_WRITE_ONLY;
        case (Access::READ_WRITE): return GL_READ_WRITE;
    }
    assert(false);
    return GL_NONE;
}

GLenum toGL(Usage usage)
{
    switch (usage)
    {
        case (Usage::STREAM_DRAW): return GL_STREAM_DRAW;
        case (Usage::STREAM_READ): return GL_STREAM_READ;
        case (Usage::STREAM_COPY): return GL_STREAM_COPY;
        
        case (Usage::STATIC_DRAW): return GL_STATIC_DRAW;
        case (Usage::STATIC_READ): return GL_STATIC_READ;
        case (Usage::STATIC_COPY): return GL_STATIC_COPY;
        
        case (Usage::DYNAMIC_DRAW): return GL_DYNAMIC_DRAW;
        case (Usage::DYNAMIC_READ): return GL_DYNAMIC_READ;
        case (Usage::DYNAMIC_COPY): return GL_DYNAMIC_COPY;

    }
    assert(false);
    return GL_NONE;
}


GLenum toGL(TextureElementType elementtype)
{
    switch (elementtype)
    {
        case (TextureElementType::UNSIGNED_BYTE): return GL_UNSIGNED_BYTE;
        case (TextureElementType::BYTE): return GL_BYTE;
        case (TextureElementType::UNSIGNED_SHORT): return GL_UNSIGNED_SHORT;
        case (TextureElementType::SHORT): return GL_SHORT;
        case (TextureElementType::UNSIGNED_INT): return GL_UNSIGNED_INT;
        case (TextureElementType::INT): return GL_INT;
        case (TextureElementType::FLOAT): return GL_FLOAT;
        case (TextureElementType::UNSIGNED_BYTE_3_3_2): return GL_UNSIGNED_BYTE_3_3_2;
        case (TextureElementType::UNSIGNED_BYTE_2_3_3_REV): return GL_UNSIGNED_BYTE_2_3_3_REV;
        case (TextureElementType::UNSIGNED_SHORT_5_6_5): return GL_UNSIGNED_SHORT_5_6_5;
        case (TextureElementType::UNSIGNED_SHORT_5_6_5_REV): return GL_UNSIGNED_SHORT_5_6_5_REV;
        case (TextureElementType::UNSIGNED_SHORT_4_4_4_4): return GL_UNSIGNED_SHORT_4_4_4_4;
        case (TextureElementType::UNSIGNED_SHORT_4_4_4_4_REV): return GL_UNSIGNED_SHORT_4_4_4_4_REV;
        case (TextureElementType::UNSIGNED_SHORT_5_5_5_1): return GL_UNSIGNED_SHORT_5_5_5_1;
        case (TextureElementType::UNSIGNED_SHORT_1_5_5_5_REV): return GL_UNSIGNED_SHORT_1_5_5_5_REV;
        case (TextureElementType::UNSIGNED_INT_8_8_8_8): return GL_UNSIGNED_INT_8_8_8_8;
        case (TextureElementType::UNSIGNED_INT_8_8_8_8_REV): return GL_UNSIGNED_INT_8_8_8_8_REV;
        case (TextureElementType::UNSIGNED_INT_10_10_10_2): return GL_UNSIGNED_INT_10_10_10_2;
        case (TextureElementType::UNSIGNED_INT_2_10_10_10_REV): return GL_UNSIGNED_INT_2_10_10_10_REV;
        
    }
    assert(false);
    return GL_NONE;
}

int toGLBytes(TextureElementType elementtype)
{

    switch (elementtype)
    {
        case (TextureElementType::UNSIGNED_BYTE): return 1;
        case (TextureElementType::BYTE): return 1;
        case (TextureElementType::UNSIGNED_SHORT): return 2;
        case (TextureElementType::SHORT): return 2;
        case (TextureElementType::UNSIGNED_INT): return 4;
        case (TextureElementType::INT): return 4;
        case (TextureElementType::FLOAT): return 4;
        case (TextureElementType::UNSIGNED_BYTE_3_3_2): return 1;
        case (TextureElementType::UNSIGNED_BYTE_2_3_3_REV): return 1;
        case (TextureElementType::UNSIGNED_SHORT_5_6_5): return 2;
        case (TextureElementType::UNSIGNED_SHORT_5_6_5_REV): return 2;
        case (TextureElementType::UNSIGNED_SHORT_4_4_4_4): return 2;
        case (TextureElementType::UNSIGNED_SHORT_4_4_4_4_REV): return 2;
        case (TextureElementType::UNSIGNED_SHORT_5_5_5_1): return 2;
        case (TextureElementType::UNSIGNED_SHORT_1_5_5_5_REV): return 2;
        case (TextureElementType::UNSIGNED_INT_8_8_8_8): return 4;
        case (TextureElementType::UNSIGNED_INT_8_8_8_8_REV): return 4;
        case (TextureElementType::UNSIGNED_INT_10_10_10_2): return 4;
        case (TextureElementType::UNSIGNED_INT_2_10_10_10_REV): return 4;
        
    }
    assert(false);
    return 0;
}

GLenum toGL(TexturePixelFormat pixelformat)
{
    switch (pixelformat)
    {
        case (TexturePixelFormat::RED): return GL_RED;
        case (TexturePixelFormat::RG): return GL_RG;
        case (TexturePixelFormat::RGB): return GL_RGB;
        case (TexturePixelFormat::BGR): return GL_BGR;
        case (TexturePixelFormat::RGBA): return GL_RGBA;
        case (TexturePixelFormat::BGRA): return GL_BGRA;
        case (TexturePixelFormat::RED_INTEGER): return GL_RED_INTEGER;
        case (TexturePixelFormat::RG_INTEGER): return GL_RG_INTEGER;
        case (TexturePixelFormat::RGB_INTEGER): return GL_RGB_INTEGER;
        case (TexturePixelFormat::BGR_INTEGER): return GL_BGR_INTEGER;
        case (TexturePixelFormat::RGBA_INTEGER): return GL_RGBA_INTEGER;
        case (TexturePixelFormat::BGRA_INTEGER): return GL_BGRA_INTEGER;
        case (TexturePixelFormat::STENCIL_INDEX): return GL_STENCIL_INDEX;
        case (TexturePixelFormat::DEPTH_COMPONENT): return GL_DEPTH_COMPONENT;
        case (TexturePixelFormat::DEPTH_STENCIL): return GL_DEPTH_STENCIL;
        
        
    }
    assert(false);
    return GL_NONE;
}

int toGLDim(TexturePixelFormat pixelformat)
{
    switch (pixelformat)
    {
        case (TexturePixelFormat::RED): return 1;
        case (TexturePixelFormat::RG): return 2;
        case (TexturePixelFormat::RGB): return 3;
        case (TexturePixelFormat::BGR): return 3;
        case (TexturePixelFormat::RGBA): return 4;
        case (TexturePixelFormat::BGRA): return 4;
        case (TexturePixelFormat::RED_INTEGER): return 1;
        case (TexturePixelFormat::RG_INTEGER): return 2;
        case (TexturePixelFormat::RGB_INTEGER): return 3;
        case (TexturePixelFormat::BGR_INTEGER): return 3;
        case (TexturePixelFormat::RGBA_INTEGER): return 4;
        case (TexturePixelFormat::BGRA_INTEGER): return 4;
        ///TODO: FIXME: what is the count for STENCIL_INDEX?
        case (TexturePixelFormat::STENCIL_INDEX): assert(false); return 1;
        case (TexturePixelFormat::DEPTH_COMPONENT): return 1;
        case (TexturePixelFormat::DEPTH_STENCIL): return 2;
        
    }
    assert(false);
    return 0;
}


GLenum toGL(TextureType texturetype)
{
    switch (texturetype)
    {
        case (TextureType::Texture1D): return GL_TEXTURE_1D;
        case (TextureType::Texture1DArray): return GL_TEXTURE_1D_ARRAY;
        case (TextureType::Texture2D): return GL_TEXTURE_2D;
        case (TextureType::Texture2DArray): return GL_TEXTURE_2D_ARRAY;
        case (TextureType::TextureRectangle): return GL_TEXTURE_RECTANGLE;
        case (TextureType::Texture3D): return GL_TEXTURE_3D;

        case (TextureType::TextureCubeMap): return GL_TEXTURE_CUBE_MAP;
        case (TextureType::TextureCubeMapArray): return GL_TEXTURE_CUBE_MAP_ARRAY;

    }
        
    assert(false);
    return GL_NONE;
}

GLenum toGLBinding(TextureType texturetype)
{

    switch (texturetype)
    {
        case (TextureType::Texture1D): return GL_TEXTURE_BINDING_1D;
        case (TextureType::Texture1DArray): return GL_TEXTURE_BINDING_1D_ARRAY;
        case (TextureType::Texture2D): return GL_TEXTURE_BINDING_2D;
        case (TextureType::Texture2DArray): return GL_TEXTURE_BINDING_2D_ARRAY;
        case (TextureType::TextureRectangle): return GL_TEXTURE_BINDING_RECTANGLE;
        case (TextureType::Texture3D): return GL_TEXTURE_BINDING_3D;
        case (TextureType::TextureCubeMap): return GL_TEXTURE_BINDING_CUBE_MAP;
        case (TextureType::TextureCubeMapArray): return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
    }
    assert(false);
    return GL_NONE;
}


GLint toGL(TextureAddressMode addressmode)
{
    switch (addressmode)
    {
        case (TextureAddressMode::TextureAddressWrap): return GL_REPEAT;
        case (TextureAddressMode::TextureAddressMirrorWrap): return GL_MIRRORED_REPEAT;
        case (TextureAddressMode::TextureAddressEdgeClamp): return GL_CLAMP_TO_EDGE;
        case (TextureAddressMode::TextureAddressMirrorEdgeClamp): return GL_MIRROR_CLAMP_TO_EDGE;
        case (TextureAddressMode::TextureAddressBorderClamp): return GL_CLAMP_TO_BORDER;

    }
        
    assert(false);
    return GL_NONE;
}

GLenum toGL(TextureFilterMode filtermode)
{
    switch (filtermode)
    {
        case (TextureFilterMode::Nearest): return GL_NEAREST;
        case (TextureFilterMode::Linear): return GL_LINEAR;
        case (TextureFilterMode::NearestMipmapNearest): return GL_NEAREST_MIPMAP_NEAREST;
        case (TextureFilterMode::LinearMipmapNearest): return GL_LINEAR_MIPMAP_NEAREST;
        case (TextureFilterMode::NearestMipmapLinear): return GL_NEAREST_MIPMAP_LINEAR;
        case (TextureFilterMode::LinearMipmapLinear): return GL_LINEAR_MIPMAP_LINEAR;

    }
    assert(false);
    return GL_NONE;
}


GLenum toGL(TextureInternalFormat internalformat)
{
    switch (internalformat)
    {

        case (TextureInternalFormat::DEPTH_COMPONENT): return GL_DEPTH_COMPONENT;
        case (TextureInternalFormat::DEPTH_STENCIL): return GL_DEPTH_STENCIL;
        case (TextureInternalFormat::RED): return GL_RED;
        case (TextureInternalFormat::RG): return GL_RG;
        case (TextureInternalFormat::RGB): return GL_RGB;
        case (TextureInternalFormat::RGBA): return GL_RGBA;


        case (TextureInternalFormat::R8): return GL_R8; 
        case (TextureInternalFormat::R8_SNORM): return GL_R8_SNORM; 
        case (TextureInternalFormat::R16): return GL_R16; 
        case (TextureInternalFormat::R16_SNORM): return GL_R16_SNORM; 
        case (TextureInternalFormat::RG8): return GL_RG8; 
        case (TextureInternalFormat::RG8_SNORM): return GL_RG8_SNORM; 
        case (TextureInternalFormat::RG16): return GL_RG16; 
        case (TextureInternalFormat::RG16_SNORM): return GL_RG16_SNORM; 
        case (TextureInternalFormat::R3_G3_B2): return GL_R3_G3_B2; 
        case (TextureInternalFormat::RGB4): return GL_RGB4; 
        case (TextureInternalFormat::RGB5): return GL_RGB5; 
        case (TextureInternalFormat::RGB8): return GL_RGB8; 
        case (TextureInternalFormat::RGB8_SNORM): return GL_RGB8_SNORM; 
        case (TextureInternalFormat::RGB10): return GL_RGB10; 
        case (TextureInternalFormat::RGB12): return GL_RGB12; 
        case (TextureInternalFormat::RGB16_SNORM): return GL_RGB16_SNORM; 
        case (TextureInternalFormat::RGBA2): return GL_RGBA2; 
        case (TextureInternalFormat::RGBA4): return GL_RGBA4; 
        case (TextureInternalFormat::RGB5_A1): return GL_RGB5_A1; 
        case (TextureInternalFormat::RGBA8): return GL_RGBA8; 
        case (TextureInternalFormat::RGBA8_SNORM): return GL_RGBA8_SNORM; 
        case (TextureInternalFormat::RGB10_A2): return GL_RGB10_A2; 
        case (TextureInternalFormat::RGB10_A2UI): return GL_RGB10_A2UI; 
        case (TextureInternalFormat::RGBA12): return GL_RGBA12; 
        case (TextureInternalFormat::RGBA16): return GL_RGBA16; 
        case (TextureInternalFormat::SRGB8): return GL_SRGB8; 
        case (TextureInternalFormat::SRGB8_ALPHA8): return GL_SRGB8_ALPHA8; 
        case (TextureInternalFormat::R16F): return GL_R16F; 
        case (TextureInternalFormat::RG16F): return GL_RG16F; 
        case (TextureInternalFormat::RGB16F): return GL_RGB16F; 
        case (TextureInternalFormat::RGBA16F): return GL_RGBA16F; 
        case (TextureInternalFormat::R32F): return GL_R32F; 
        case (TextureInternalFormat::RG32F): return GL_RG32F; 
        case (TextureInternalFormat::RGB32F): return GL_RGB32F; 
        case (TextureInternalFormat::RGBA32F): return GL_RGBA32F; 
        case (TextureInternalFormat::R11F_G11F_B10F): return GL_R11F_G11F_B10F; 
        case (TextureInternalFormat::RGB9_E5): return GL_RGB9_E5; 
        case (TextureInternalFormat::R8I): return GL_R8I; 
        case (TextureInternalFormat::R8UI): return GL_R8UI; 
        case (TextureInternalFormat::R16I): return GL_R16I; 
        case (TextureInternalFormat::R16UI): return GL_R16UI; 
        case (TextureInternalFormat::R32I): return GL_R32I; 
        case (TextureInternalFormat::R32UI): return GL_R32UI; 
        case (TextureInternalFormat::RG8I): return GL_RG8I; 
        case (TextureInternalFormat::RG8UI): return GL_RG8UI;
        case (TextureInternalFormat::RG16I): return GL_RG16I;
        case (TextureInternalFormat::RG16UI): return GL_RG16UI;
        case (TextureInternalFormat::RG32I): return GL_RG32I;
        case (TextureInternalFormat::RG32UI): return GL_RG32UI;
        case (TextureInternalFormat::RGB8I): return GL_RGB8I;
        case (TextureInternalFormat::RGB8UI): return GL_RGB8UI;
        case (TextureInternalFormat::RGB16I): return GL_RGB16I;
        case (TextureInternalFormat::RGB16UI): return GL_RGB16UI;
        case (TextureInternalFormat::RGB32I): return GL_RGB32I;
        case (TextureInternalFormat::RGB32UI): return GL_RGB32UI;
        case (TextureInternalFormat::RGBA8I): return GL_RGBA8I;
        case (TextureInternalFormat::RGBA8UI): return GL_RGBA8UI;
        case (TextureInternalFormat::RGBA16I): return GL_RGBA16I;
        case (TextureInternalFormat::RGBA16UI): return GL_RGBA16UI;
        case (TextureInternalFormat::RGBA32I): return GL_RGBA32I;
        case (TextureInternalFormat::RGBA32UI): return GL_RGBA32UI;

        case (TextureInternalFormat::COMPRESSED_RED): return GL_COMPRESSED_RED;
        case (TextureInternalFormat::COMPRESSED_RG): return GL_COMPRESSED_RG;
        case (TextureInternalFormat::COMPRESSED_RGB): return GL_COMPRESSED_RGB;
        case (TextureInternalFormat::COMPRESSED_RGBA): return GL_COMPRESSED_RGBA;
        case (TextureInternalFormat::COMPRESSED_SRGB): return GL_COMPRESSED_SRGB;
        case (TextureInternalFormat::COMPRESSED_SRGB_ALPHA): return GL_COMPRESSED_SRGB_ALPHA;
        case (TextureInternalFormat::COMPRESSED_RED_RGTC1): return GL_COMPRESSED_RED_RGTC1;
        case (TextureInternalFormat::COMPRESSED_SIGNED_RED_RGTC1): return GL_COMPRESSED_SIGNED_RED_RGTC1;
        case (TextureInternalFormat::COMPRESSED_RG_RGTC2): return GL_COMPRESSED_RG_RGTC2;
        case (TextureInternalFormat::COMPRESSED_SIGNED_RG_RGTC2): return GL_COMPRESSED_SIGNED_RG_RGTC2;
        case (TextureInternalFormat::COMPRESSED_RGBA_BPTC_UNORM): return GL_COMPRESSED_RGBA_BPTC_UNORM;
        case (TextureInternalFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM): return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
        case (TextureInternalFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT): return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
        case (TextureInternalFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT): return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        
    }
    assert(false);
    return GL_NONE;
}




GLenum toGL(PrimitiveIndexType primitiveindextype)
{
    switch (primitiveindextype)
    {
        case (PrimitiveIndexType::Indices8Bit): return GL_UNSIGNED_BYTE;
        case (PrimitiveIndexType::Indices16Bit): return GL_UNSIGNED_SHORT;
        case (PrimitiveIndexType::Indices32Bit): return GL_UNSIGNED_INT;
        case (PrimitiveIndexType::IndicesNone): return GL_NONE;
    }
    assert(false);
    return GL_NONE;
}

GLenum toGL(ShaderType type)
{
    switch (type)
    {
        case (ShaderType::GeometryShader): return GL_GEOMETRY_SHADER;
        case (ShaderType::VertexShader): return GL_VERTEX_SHADER;
        case (ShaderType::PixelShader): return GL_FRAGMENT_SHADER;
        case (ShaderType::ComputeShader): return GL_COMPUTE_SHADER;
        case (ShaderType::TessControlShader): return GL_TESS_CONTROL_SHADER;
        case (ShaderType::TessEvalShader): return GL_TESS_EVALUATION_SHADER;
        case (ShaderType::NullShader): return GL_NONE;
    }
    assert(false);
    return GL_NONE;
}



GLuint getHandle(const ShaderProgram& sp)
{
    return sp.pimpl->m_programHandle;
}

GLuint getHandle(const Texture& texture)
{
    return texture.pimpl->m_tex;
}

GLuint getHandle(const TextureSampler& sampler)
{
    return sampler.pimpl->m_smplr;
}

GLuint getHandle(const Shader& shader)
{
    return shader.pimpl->m_shaderHandle;
}

std::map<PrimitiveIndexType, const char*> genGLSTRMAP()
{
    std::map<PrimitiveIndexType, const char*> result;

    result.insert( {PrimitiveIndexType::Indices8Bit, "GL_UNSIGNED_BYTE"} );
    result.insert( {PrimitiveIndexType::Indices16Bit, "GL_UNSIGNED_SHORT"} );
    result.insert( {PrimitiveIndexType::Indices32Bit, "GL_UNSIGNED_INT"} );
    result.insert( {PrimitiveIndexType::IndicesNone, "GL_NONE"} );

    return result;
}

const char* toGLSTR(PrimitiveIndexType primitiveindextype)
{
    static auto strmap = genGLSTRMAP();

    auto w = strmap.find(primitiveindextype);

    if (w != strmap.end())
    {
        return w->second;
    }
    assert (false);
    return "GL_NONE";
}


////////////////////////////////////////////////////////////////////////////////

const char* toSTR(VertexDataSemantic semantic)
{
    switch (semantic)
    {
        case (VertexDataSemantic::COLOR):
            return "COLOR";
        case (VertexDataSemantic::COLOR2):
            return "COLOR2";
        case (VertexDataSemantic::NORMAL):
            return "NORMAL";
        case (VertexDataSemantic::TCOORD):
            return "TCOORD";
        case (VertexDataSemantic::VCOORD):
            return "VCOORD";
    }
    
    assert(false);
    return "UNKNOWN";
}


const char* toSTR(VertexDataType type)
{
    switch (type)
    {
        case (VertexDataType::BYTE):
            return "BYTE";
        case (VertexDataType::UNSIGNED_BYTE):
            return "UNSIGNED_BYTE";
        case (VertexDataType::SHORT):
            return "SHORT";
        case (VertexDataType::UNSIGNED_SHORT):
            return "UNSIGNED_SHORT";
        case (VertexDataType::INT):
            return "INT";
        case (VertexDataType::UNSIGNED_INT):
            return "UNSIGNED_INT";
        case (VertexDataType::FLOAT):
            return "FLOAT";
        case (VertexDataType::DOUBLE):
            return "DOUBLE";
    }
    
    assert(false);
    return "UNKNOWN";
}

const char* toSTR(GPUVertexDataType type)
{
    switch (type)
    {
        case (GPUVertexDataType::DEFAULT_TO_SRC):
            return "DEFAULT_TO_SRC";
        case (GPUVertexDataType::INT):
            return "INT";
        case (GPUVertexDataType::FLOAT):
            return "FLOAT";
        case (GPUVertexDataType::DOUBLE):
            return "DOUBLE";
    }
    
    assert(false);
    return "UNKNOWN";
}



////////////////////////////////////////////////////////////////////////////////






Graphics::Graphics() 
{
  

}


Graphics::~Graphics(void)
{
}


void Graphics::Clear(bool clearDepth, bool clearStencil, bool clearColor,
            float red, float green, float blue, float alpha,
            float depthValue, int stencilValue)
{
    //glClearStencil(stencilValue);
        
    {
        glClearColor(red, green, blue, alpha);
    }
    {
        GLbitfield mask = 0;
            
        mask |= ((clearDepth) ? GL_DEPTH_BUFFER_BIT : 0)
                | ((clearStencil) ? GL_STENCIL_BUFFER_BIT : 0)
                | ((clearColor) ? GL_COLOR_BUFFER_BIT : 0);
            
        glClear(mask);
    }
        
}

////////////////////////////////////////////////////////////////////////////////


VertexElement::VertexElement( VertexDataSemantic semantic
                            , VertexDataType src_type
                            , int dimension
                            , const std::string& name
                            , bool normalized
                            , GPUVertexDataType dst_type)
    : m_semantic(semantic)
    , m_src_type(src_type)
    , m_dst_type(dst_type)
    , m_normalized(normalized)
    , m_dimension(dimension)
    , m_name(name)
{
    assert(dimension >= 1);
    assert(dimension <= 4);
    
    if (m_dst_type == GPUVertexDataType::DEFAULT_TO_SRC)
        m_dst_type = toDefaultGPUDataType(m_src_type);

    assert(Valid());
}


VertexDataType VertexElement::SrcType() const
{
    return m_src_type;
}



GPUVertexDataType VertexElement::DstType() const
{
    return m_dst_type;
}


bool VertexElement::Normalized() const
{
    return m_normalized;
}



bool VertexElement::Valid() const
{
    VertexDataType src_type = m_src_type;
    GPUVertexDataType dst_type = m_dst_type;

    static const std::set<VertexDataType> int_src_types{
          VertexDataType::BYTE
        , VertexDataType::UNSIGNED_BYTE
        , VertexDataType::SHORT
        , VertexDataType::UNSIGNED_SHORT
        , VertexDataType::INT
        , VertexDataType::UNSIGNED_INT};

    if (!int_src_types.count(src_type) && Normalized())
        return false;

    switch (dst_type)
    {
        case(GPUVertexDataType::INT): {
            if (int_src_types.count(src_type))
                return true;

            return false;
        } case (GPUVertexDataType::FLOAT): {
            return true;
        } case (GPUVertexDataType::DOUBLE): {
            if (src_type == VertexDataType::DOUBLE)
                return true;
            return false;
        } case (GPUVertexDataType::DEFAULT_TO_SRC): {
            return false;
        }
    }

    assert(false);
    return false;
}

GPUVertexDataType toDefaultGPUDataType(VertexDataType src_type)
{
    switch (src_type)
    {
        case(VertexDataType::BYTE):
        case(VertexDataType::UNSIGNED_BYTE):
        case(VertexDataType::UNSIGNED_SHORT):
        case(VertexDataType::SHORT):
        case(VertexDataType::INT):
        case(VertexDataType::UNSIGNED_INT):
        {
            return GPUVertexDataType::INT;
        } case (VertexDataType::FLOAT): {
            return GPUVertexDataType::FLOAT;
        } case (VertexDataType::DOUBLE): {
            return GPUVertexDataType::DOUBLE;
        }
    }

    assert(false);
    return GPUVertexDataType::FLOAT;
}

VertexElement::~VertexElement()
{
}


const std::string& VertexElement::Name() const
{
    return m_name;
}

int VertexElement::SizeBytes() const
{
    int size = 0;
    switch(m_src_type)
    {
    case VertexDataType::BYTE:
    case VertexDataType::UNSIGNED_BYTE:
        size = 1;
        break;
    case VertexDataType::DOUBLE:
        size = 8;
        break;
    case VertexDataType::FLOAT:
    case VertexDataType::INT:
    case VertexDataType::UNSIGNED_INT:
        size = 4;
        break;
    case VertexDataType::SHORT:
    case VertexDataType::UNSIGNED_SHORT:
        size = 2;
        break;
    }

    return size * m_dimension;
}


int VertexElement::SizeDimension() const
{
    return m_dimension;
}

VertexDataSemantic VertexElement::Semantic() const
{
    return m_semantic;
}


std::string VertexElement::ToString() const
{
    std::ostringstream ostr;
    ostr << *this;
    return ostr.str();
}

::std::ostream& operator<<(::std::ostream&out, const VertexElement& element)
{
    // "semantic: (semantic), name: (name), type: (type), count: (num)"
    out << toSTR(element.Semantic()) << ", name: " << element.Name()
        << ", type: " << toSTR(element.SrcType());
    switch (element.DstType())
    {
        case (GPUVertexDataType::DEFAULT_TO_SRC):
            out << " => " << toSTR(element.SrcType());
            break;
        
        case (GPUVertexDataType::INT):
            assert(element.SrcType() != VertexDataType::FLOAT);
            assert(element.SrcType() != VertexDataType::DOUBLE);
            out << " => " << toSTR(element.SrcType());
            break;
        case (GPUVertexDataType::FLOAT):
        case (GPUVertexDataType::DOUBLE):
            out << " => " << toSTR(element.DstType());
            break;
        
    }
    
    out << ", count: " << element.SizeDimension();
    
    return out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


VertexDeclaration::VertexDeclaration()
{
}


VertexDeclaration::~VertexDeclaration(void)
{
}


void VertexDeclaration::Add(VertexElement elem)
{
    m_elements.push_back(elem);
}


int VertexDeclaration::Stride() const
{

    int size = 0;

    for(auto& elem : m_elements)
    {
        size += elem.SizeBytes();
    }

    return size;
}


const std::vector<VertexElement>& VertexDeclaration::Elements() const
{
    return m_elements;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


VertexBuffer::VertexBuffer(std::weak_ptr<Mesh> mesh, int numVertices, const VertexDeclaration& dec, Usage usage, bool allocateCpu)
    : pimpl(new VertexBufferPimpl())
      
{
    pimpl->m_mesh = (mesh);
    pimpl->m_numVertices = (numVertices);
    pimpl->m_declaration = (dec);
    pimpl->m_usage = (usage);
    pimpl->m_vbo = (0);
    pimpl->m_gpuSize = (0);
    
    glGenBuffers(1, &pimpl->m_vbo); 
    checkOpenGLError();

    if (allocateCpu)
        AllocateCpuMemory();
}


VertexBuffer::~VertexBuffer()
{
    if(pimpl->m_vbo)
    {
        glDeleteBuffers(1, &pimpl->m_vbo);
        pimpl->m_vbo = 0;
    }
}


int VertexBuffer::NumVertices() const
{
    return pimpl->m_numVertices;
}
   

int VertexBuffer::LogicalBufferSizeBytes() const
{
    return pimpl->m_numVertices * pimpl->m_declaration.Stride();
}


const unsigned char* VertexBuffer::CpuPtr() const
{
    if (!pimpl->m_cpuData)
    {
        throw std::runtime_error( "no cpu data" );
    }
    return pimpl->m_cpuData->data();
}


unsigned char* VertexBuffer::CpuPtr()
{
    if (!pimpl->m_cpuData)
    {
        throw std::runtime_error( "no cpu data" );
    }

    return pimpl->m_cpuData->data();
}


int VertexBuffer::GpuSizeInBytes() const
{
    return pimpl->m_gpuSize;
}


const VertexDeclaration& VertexBuffer::Declaration() const
{
    return pimpl->m_declaration;
}



void VertexBuffer::SetNumVertices(std::size_t size, bool preserve_old_cpu_data)
{

    ///set the new number of vertices
    pimpl->m_numVertices = size;

    std::size_t new_bytes = LogicalBufferSizeBytes();

    if (HasCpuMemory())
    {
        if (preserve_old_cpu_data && !!pimpl->m_cpuData && new_bytes > pimpl->m_cpuData->size())
        {
            ///keep the old data around a bit
            std::shared_ptr<cpu_data_t> oldCpuData = pimpl->m_cpuData;
             
                
            ///allocate the buffer
            AllocateCpuMemory();

            ///copy the old buffer to the new one
            memcpy(pimpl->m_cpuData->data(), oldCpuData->data(), oldCpuData->size());
        } 
        else 
        {
            AllocateCpuMemory();
        }
    }
}


void VertexBuffer::AllocateCpuMemory()
{
    std::size_t bytes = LogicalBufferSizeBytes();
   
    if (!pimpl->m_cpuData)
    {
        pimpl->m_cpuData = std::make_shared< cpu_data_t >(bytes);
    }
    else
    {
        if (pimpl->m_cpuData->size() != bytes)
            pimpl->m_cpuData->resize(bytes);
    }
}


void VertexBuffer::AllocateGpuMemory()
{

    assert(IsBound());
    checkOpenGLError();

    int bytes = LogicalBufferSizeBytes();

    glBufferData(GL_ARRAY_BUFFER, bytes, NULL, toGL(pimpl->m_usage));
    checkOpenGLError();
         
    pimpl->m_gpuSize = bytes;

}

bool VertexBuffer::HasCpuMemory() const
{
    return !!(pimpl->m_cpuData); 
}

void VertexBuffer::Bind(){
    ///https://www.opengl.org/wiki/Buffer_Object#General_use
    ///"Note that this binding target is part of a Vertex Array Objects state,
    /// so a VAO must be bound before binding a buffer here".
    assert(VAOIsBound());
    checkOpenGLError();
    glBindBuffer(GL_ARRAY_BUFFER, pimpl->m_vbo); 
    checkOpenGLError();
    assert(IsBound());
}
void VertexBuffer::UnBind(){
    checkOpenGLError();
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    checkOpenGLError();
}

void VertexBuffer::UnBindAll(){
    checkOpenGLError();
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    checkOpenGLError();
}

bool VertexBuffer::IsBound() const
{
    checkOpenGLError();
    GLint boundvbo = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundvbo);
    checkOpenGLError();
    return boundvbo != 0 && boundvbo == pimpl->m_vbo;
}

bool VertexBuffer::VAOIsBound() const
{
    if (auto mesh = pimpl->m_mesh.lock())
    {
        return mesh->IsBound();
    }

    return false;
}

void VertexBuffer::UpdateToGpu()
{
    if (!pimpl->m_cpuData)
    {
        throw std::runtime_error( "no cpu data to send to gpu" );
    }
    
    
    const uint8_t* data = pimpl->m_cpuData->data();
    std::size_t size = pimpl->m_cpuData->size();
    this->UpdateToGpu(data, size);
}

void VertexBuffer::UpdateToGpu(const uint8_t* data, int size)
{
    assert(IsBound());
    checkOpenGLError();
    if (!data && !pimpl->m_cpuData)
    {
        throw std::runtime_error( "no cpu data to send to gpu" );
    }


    if (!data)
    {
        data = pimpl->m_cpuData->data();
        size = pimpl->m_cpuData->size();
    }


    assert(size == LogicalBufferSizeBytes());
    glBufferData(GL_ARRAY_BUFFER, size, data, toGL(pimpl->m_usage));
    checkOpenGLError();
      
    pimpl->m_gpuSize = size;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IndexBuffer::IndexBuffer(std::weak_ptr<Mesh> mesh, int numIndices, PrimitiveIndexType type, Usage usage, bool allocateCpu)
    : pimpl(new IndexBufferPimpl())
     
{
    pimpl->m_mesh = (mesh);
    pimpl->m_numIndices = numIndices;
    pimpl->m_indexBuffer = 0;
    pimpl->m_indexType = (type);
    pimpl->m_usage = (usage);
    pimpl->m_gpuSize = 0;
    
    
    glGenBuffers(1, &pimpl->m_indexBuffer);
    checkOpenGLError();

    if (allocateCpu)
        AllocateCpuMemory();
}


IndexBuffer::~IndexBuffer(void)
{
    if(pimpl->m_indexBuffer)
    {
        glDeleteBuffers(1, &pimpl->m_indexBuffer);
        pimpl->m_indexBuffer = 0;
    }
}


int IndexBuffer::NumIndices() const
{
    return pimpl->m_numIndices;
}


PrimitiveIndexType IndexBuffer::IndexType() const
{
    return pimpl->m_indexType;
}


int IndexBuffer::IndexSizeBytes() const
{
    int32_t size = 0;

    switch(pimpl->m_indexType)
    {
    case PrimitiveIndexType::IndicesNone:
        size = 0;
        break;
    case PrimitiveIndexType::Indices8Bit:
        size = 1;
        break;
    case PrimitiveIndexType::Indices16Bit:
        size = 2;
        break;
    case PrimitiveIndexType::Indices32Bit:
        size = 4;
        break;
    }

    return size;
}



bool IndexBuffer::HasCpuMemory() const
{
    return !!pimpl->m_indexData;
}
int IndexBuffer::LogicalBufferSizeBytes() const
{
    return NumIndices() * IndexSizeBytes();
}

void IndexBuffer::AllocateCpuMemory()
{
    int bytes = LogicalBufferSizeBytes();
   
    if (!pimpl->m_indexData)
    {
        pimpl->m_indexData = std::make_shared< cpu_data_t >(bytes);
    }
    else 
    {
        pimpl->m_indexData->resize(bytes);
    }
}


void IndexBuffer::SetNumIndices(std::size_t size, bool perseve_old_cpu_data)
{

    ///set the new number of vertices
    pimpl->m_numIndices = size;

    std::size_t newBytes = LogicalBufferSizeBytes();

    if (HasCpuMemory())
    {
        if (perseve_old_cpu_data && !!pimpl->m_indexData && newBytes > pimpl->m_indexData->size())
        {
            ///keep the old data around a bit
            std::shared_ptr<cpu_data_t> oldCpuData = pimpl->m_indexData;

            ///allocate the buffer
            AllocateCpuMemory();

            ///copy the old buffer to the new one
            memcpy(pimpl->m_indexData->data(), oldCpuData->data(), oldCpuData->size());
        } 
        else 
        {
            AllocateCpuMemory();
        }
    }
}


unsigned char * IndexBuffer::CpuPtr()
{
    if ( !pimpl->m_indexData )
    {
        throw std::runtime_error( "no cpu data" );
    }
    return pimpl->m_indexData->data();
}


const unsigned char * IndexBuffer::CpuPtr() const
{
    if ( !pimpl->m_indexData )
    {
        throw std::runtime_error( "no cpu data" );
    }
    return pimpl->m_indexData->data();
}


bool IndexBuffer::VAOIsBound() const
{
    if (auto mesh = pimpl->m_mesh.lock())
    {
        return mesh->IsBound();
    }

    return false;
}

void IndexBuffer::Bind()
{
    ///https://www.opengl.org/wiki/Buffer_Object#General_use
    ///"Note that this binding target is part of a Vertex Array Objects state,
    /// so a VAO must be bound before binding a buffer here".
    assert(VAOIsBound());
    checkOpenGLError();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pimpl->m_indexBuffer);
    checkOpenGLError();
}

void IndexBuffer::UnBind()
{
    checkOpenGLError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    checkOpenGLError();
}

void IndexBuffer::UnBindAll()
{
    checkOpenGLError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    checkOpenGLError();
}

bool IndexBuffer::IsBound() const
{
    checkOpenGLError();
    GLint boundibo = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundibo);
    checkOpenGLError();
    return boundibo != 0 && boundibo == pimpl->m_indexBuffer;
}

void IndexBuffer::AllocateGpuMemory()
{
    assert(IsBound());
    checkOpenGLError();

    int bytes = LogicalBufferSizeBytes();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes, NULL, toGL(pimpl->m_usage)); 
    checkOpenGLError();
         
    pimpl->m_gpuSize = bytes;
}


void IndexBuffer::UpdateToGpu(const uint8_t* data, int bytes)
{
    assert(IsBound());
    checkOpenGLError();
    if (!data && !pimpl->m_indexData)
    {
        throw std::runtime_error( "no cpu data to send to gpu" );
    }

    if (!data)
    {
        data = pimpl->m_indexData->data();
        bytes = pimpl->m_indexData->size();
    }
    

    assert(bytes == LogicalBufferSizeBytes());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes, data, toGL(pimpl->m_usage));
    checkOpenGLError();
      
    pimpl->m_gpuSize = bytes;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Shader::Shader(ShaderType type)
    : pimpl(new ShaderPimpl{ /*.m_shaderHandle=*/ 0, /*.m_type=*/ type })
{

    if (type != ShaderType::NullShader)
    {
        pimpl->m_shaderHandle = glCreateShader (toGL(type));
        checkOpenGLError();
    } 
    else
    {
        throw std::runtime_error("Cannot create Shader: Invalid shader type");
    }
    assert(pimpl->m_shaderHandle);
}

    
void Shader::LoadFromString(ShaderType type,
                            const char* shaderData,
                            const char* entryPoint,
                            const char* profile)
{
    assert(entryPoint == 0);
    assert(profile == 0);        
    if (type != pimpl->m_type)
    {
        throw std::runtime_error("Cannot load Shader: Invalid shader type; does not match type indicated in constructor");
    }
         
    assert(pimpl->m_shaderHandle);
         
    glShaderSource(pimpl->m_shaderHandle, 1, &shaderData, NULL); 
    checkOpenGLError();
    glCompileShader(pimpl->m_shaderHandle); checkOpenGLError();
        
    GLint params = 0;
    glGetShaderiv(pimpl->m_shaderHandle,GL_COMPILE_STATUS, &params); 
    checkOpenGLError();
        
    if (params == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(pimpl->m_shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(pimpl->m_shaderHandle, maxLength, &maxLength, &errorLog[0]);

        glDeleteShader(pimpl->m_shaderHandle);
        pimpl->m_shaderHandle = 0;
        throw std::runtime_error(std::string("Shader: Failed to compile. ") + errorLog.data());
    }
}


void Shader::LoadFromFile(ShaderType type,
                        const char* fileName,
                        const char* entryPoint,
                        const char* profile)
{
   

    //open file
    std::ifstream f;
    f.open(fileName, std::ios::in | std::ios::binary);
    if(!f.is_open()){
        throw std::runtime_error(std::string("Failed to open file: ") + fileName);
    }

    //read whole file into stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();

    LoadFromString(type,buffer.str().c_str(), entryPoint, profile);

}


Shader::~Shader(void)
{
    glDeleteShader(pimpl->m_shaderHandle); 
    checkOpenGLError();
    pimpl->m_shaderHandle = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ShaderProgram::ShaderProgram(void)
    : pimpl( new ShaderProgramPimpl{0} )
{
    pimpl->m_programHandle = glCreateProgram();
    checkOpenGLError();
    assert(pimpl->m_programHandle);
    
}


ShaderProgram::~ShaderProgram(void)
{
    glDeleteProgram(pimpl->m_programHandle);
    checkOpenGLError();
}



void ShaderProgram::Use()
{
    assert(pimpl->m_programHandle);
    checkOpenGLError();
    glUseProgram(pimpl->m_programHandle);
    checkOpenGLError();
}

bool ShaderProgram::InUse() const
{
    checkOpenGLError();
    GLint boundsp = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &boundsp);
    checkOpenGLError();
    return boundsp != 0 && boundsp == pimpl->m_programHandle;
}


void ShaderProgram::Deselect()
{
    checkOpenGLError();
    if (InUse())
        glUseProgram(0);
    checkOpenGLError();
}

void ShaderProgram::DeselectAll()
{
    checkOpenGLError();
    glUseProgram(0);
    checkOpenGLError();
}


int ShaderProgram::GetUniformLocation(const char* name)
{
    return glGetUniformLocation(pimpl->m_programHandle, name);
}

void ShaderProgram::SetUniform(const float4x4& matrix, const std::string& name)
{
    int location = GetUniformLocation(name.c_str());
    assert(location != -1);

    ShaderProgram::SetFloat4x4(location, matrix);
}

void ShaderProgram::SetUniform(const float value, const std::string& name)
{
    int location = GetUniformLocation(name.c_str());
    assert(location != -1);

    ShaderProgram::SetFloat(location, value);
}

void ShaderProgram::SetUniform(const float v1, const float v2, const float v3, const std::string& name)
{
    int location = GetUniformLocation(name.c_str());
    assert(location != -1);

    ShaderProgram::SetFloat3(location, v1, v2, v3);
}


void ShaderProgram::SetUniform(const int value, const std::string& name)
{
    int location = GetUniformLocation(name.c_str());
    assert(location != -1);

    ShaderProgram::SetInt(location, value);
}


void ShaderProgram::SetFloat4x4(int parameterIndex, const float4x4& matrix)
{
    float4x4 colMajor = matrix.Transposed();
    glUniformMatrix4fv(parameterIndex, 1 /*count*/, GL_FALSE /*transpose*/, colMajor.ptr() /*value*/);
    
}

void ShaderProgram::SetFloat(int parameterIndex, const float value)
{
    glUniform1f(parameterIndex, value);    
}

void ShaderProgram::SetFloat3(int parameterIndex, const float v1, const float v2, const float v3)
{
    glUniform3f(parameterIndex, v1, v2, v3);    
}

void ShaderProgram::SetInt(int parameterIndex, const int value)
{
    glUniform1i(parameterIndex, value);    
}

void ShaderProgram::BindTexture(int index, TextureUnit& texture_unit, Texture& texture, const std::string& samplerName)
{
    assert(!(index < 0));
    assert(InUse());
    assert(texture_unit.IsActive());
    assert(texture.IsBound());
    assert(index == texture_unit.Index());
    checkOpenGLError();


    GLint location = glGetUniformLocation(pimpl->m_programHandle, samplerName.c_str());
    assert(location != -1);
    glUniform1i(location, index);
    checkOpenGLError();
}

void ShaderProgram::Attach(Shader& shader)
{
    assert(pimpl->m_programHandle);
    checkOpenGLError();


    glAttachShader (pimpl->m_programHandle, getHandle(shader));
    checkOpenGLError();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Mesh::Mesh(PrimitiveType primType)
    : pimpl(new MeshPimpl{ /*.m_vao=*/ 0, /*.m_numVertices=*/ 0, /*.m_numRenderableVertices=*/ 0})
{
    pimpl->m_primType = primType;
    glGenVertexArrays (1, &pimpl->m_vao); 
    checkOpenGLError();
}


Mesh::~Mesh(void)
{
    glDeleteVertexArrays (1, &pimpl->m_vao); 
    checkOpenGLError();
}


PrimitiveType Mesh::PrimType() const
{
    return pimpl->m_primType;
}


void Mesh::GenerateVAO(int startVertexOffset)
{
    assert(IsBound());

    ///determine numVertices
    {
        pimpl->m_numVertices = std::numeric_limits<int>::max();
 
        for(const auto& vb : vbs)
        {
            pimpl->m_numVertices = std::min(vb->NumVertices(), pimpl->m_numVertices);
        }
    }

    pimpl->m_numRenderableVertices = pimpl->m_numVertices - startVertexOffset;
    CheckValid();
    CheckValidVBO(startVertexOffset);




    ///Fill in declaration.elements
    {
        for(const auto& vb : vbs)
        {
            for(const auto& element : vb->Declaration().Elements())
                pimpl->m_declaration.Add(element);
        }
    }

       
    int attr_index = 0;
    ///vbo
    for(auto& vb_ptr : vbs)
    {
        /**
        * From http://antongerdelan.net/opengl/vertexbuffers.html
        * 
        * Also see http://www.arcsynthesis.org/gltut/Positioning/Tutorial%2005.html
        */
 
        auto& vb = *vb_ptr;
        const VertexDeclaration& dec = vb.Declaration();
      
        vb.Bind();
        assert(vb.IsBound());
        //glBindBuffer(GL_ARRAY_BUFFER, vb.m_vbo); 
        //checkOpenGLError();
       
        int offset = 0;
        int stride = dec.Stride();
       
        ///FIXME: Make this a NDEBUG runtime check too
        assert( startVertexOffset < vb.LogicalBufferSizeBytes() );
        for(const VertexElement& element : dec.Elements())
        {
               
            //element.check_valid();
            
            glEnableVertexAttribArray(attr_index);


            assert(element.Valid());
            ///See http://www.informit.com/articles/article.aspx?p=2033340&seqNum=3
            if (element.DstType() == GPUVertexDataType::INT)
            {
                assert(!element.Normalized());
                std::set<GLenum> valid_src_types = {GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT};

                assert( valid_src_types.count(toGL(element.SrcType())) );
                glVertexAttribIPointer (attr_index,
                                       element.SizeDimension(),
                                       toGL(element.SrcType()),
                                       stride,
                                       (void*)(startVertexOffset + offset)); 
            } else if (element.DstType() == GPUVertexDataType::DOUBLE) {
                assert(!element.Normalized());
                std::set<GLenum> valid_src_types = {GL_DOUBLE};

                assert( valid_src_types.count(toGL(element.SrcType())) );
                glVertexAttribLPointer  (attr_index,
                                       element.SizeDimension(),
                                       toGL(element.SrcType()),
                                       stride,
                                       (void*)(startVertexOffset + offset)); 
            } else if (element.DstType() == GPUVertexDataType::FLOAT) {
                glVertexAttribPointer (attr_index,
                                       element.SizeDimension(),
                                       toGL(element.SrcType()),
                                       element.Normalized() ? GL_TRUE : GL_FALSE,
                                       stride,
                                       (void*)(startVertexOffset + offset));
            } else {
                assert(false);
            }
            checkOpenGLError();
             
           
            offset += element.SizeBytes();
            ++attr_index;
        }
          
          
    }

    for (int i = 0; i < attr_index; ++i)
    {
   //     glEnableVertexAttribArray (i); 
    //    checkOpenGLError();
    }
   
    ///indices
    if (ib)
    {
        /**
        * From http://www.opengl.org/wiki/Vertex_Specification#Index_buffers
        * 
        * 
        * Indexed rendering, as defined above, requires an array of indices;
        * all vertex attributes will use the same index from this index array.
        * The index array is provided by a Buffer Object bound to the
        * GL_ELEMENT_ARRAY_BUFFER​ binding point. When a buffer is bound to
        * GL_ELEMENT_ARRAY_BUFFER​, all rendering commands of the form
        * gl*Draw*Elements*​ will use indexes from that buffer. Indices can be
        * unsigned bytes, unsigned shorts, or unsigned ints.
        */
        //ib->Bind();
        assert(ib->IsBound());
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_indexBuffer);
        //checkOpenGLError();
    }
    
    /*
    UnBind();
    VertexBuffer::UnBindAll();

    if (ib)
        ib->UnBind();

    Bind();
    */
}


void Mesh::LinkShaders()
{
    if (!sp)
    {
        throw std::runtime_error("Mesh.LinkShaders(): No shader program; no shaders to link");
    }

    assert(IsBound());

    GLint programHandle = getHandle(*sp);

    int attr_index = 0;

    for(const auto& vb : vbs)
    {
        const VertexDeclaration& dec = vb->Declaration();
  
        for(const VertexElement& element : dec.Elements())
        {
            std::string attribute_name = element.Name();
            glBindAttribLocation (programHandle, attr_index, attribute_name.c_str()); 
            checkOpenGLError();
            ++attr_index;
        }
    }
      
    glLinkProgram (programHandle); 
    checkOpenGLError();
}


void Mesh::Bind()
{
    checkOpenGLError();
    if (pimpl->m_vao == 0)
        throw std::runtime_error("Can't bind the mesh: no VAO");
    
    glBindVertexArray(pimpl->m_vao);
    checkOpenGLError();
    assert(IsBound());
}

void Mesh::UnBind()
{
    checkOpenGLError();
    glBindVertexArray(0); 
    checkOpenGLError();
}

void Mesh::UnBindAll()
{
    checkOpenGLError();
    glBindVertexArray(0); 
    checkOpenGLError();
}

bool Mesh::IsBound() const
{
    checkOpenGLError();
    GLint boundvao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundvao);
    checkOpenGLError();
    return boundvao != 0 && boundvao == pimpl->m_vao;
}

void Mesh::Draw(int numIndices, int startIndexOffset)
{
    assert(IsBound());
    checkOpenGLError();
    CheckValid();
    
    if (pimpl->m_vao == 0)
        throw std::runtime_error("Can't render the mesh: no VAO");
            
        
        
    if (ib)
    {
        assert(ib->IsBound());

        int index_count = ib->NumIndices();
        
        if (numIndices == -1)
            numIndices = index_count - startIndexOffset;
        

        if (startIndexOffset + numIndices > index_count)
            throw std::runtime_error("Can't render the mesh: startIndexOffset/numIndices lie outside the range of possible indices");
            
            
        ///FIXME: need to get a primitve count based on the primitive type
        assert(PrimType() == PrimitiveType::TriangleList);

        /*
        GLenum data_type = 0;
        switch (ib->getIndexType())
        {
            case(PrimitiveIndexType::IndicesNone):
                throw std::runtime_error("IndexBuffer has no index type specified");
            case(PrimitiveIndexType::Indices8Bit):
                data_type = GL_UNSIGNED_BYTE; 
                break;
            case(PrimitiveIndexType::Indices16Bit):
                data_type = GL_UNSIGNED_SHORT; 
                break;
            case(PrimitiveIndexType::Indices32Bit):
                data_type = GL_UNSIGNED_INT; 
                break;
            default:
                assert(false);
                
        }
        */

        //GLint bound_buff; 
        
        //glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound_buff);
        
        //printf("%i\n", bound_buff);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_indexBuffer);
        //printf("%i\n", index_count - startIndexOffset);

        //std::cout << "numIndices" << numIndices << ", startIndexOffset: " << startIndexOffset
        //    << ", ib->IndexType(): " << toGLSTR(ib->IndexType()) << std::endl;
        glDrawElements(GL_TRIANGLES, numIndices, toGL(ib->IndexType()), (void*)startIndexOffset);
        checkOpenGLError();
        
    } 
    else 
    {
        switch(PrimType())
        {
            case PrimitiveType::TriangleList:
                glDrawArrays (GL_TRIANGLES, startIndexOffset, pimpl->m_numVertices); 
                checkOpenGLError();
                break;
            case PrimitiveType::LineList:
                glDrawArrays (GL_LINES, 0, pimpl->m_numVertices); 
                checkOpenGLError();
                break;
            case PrimitiveType::Point:
                glDrawArrays (GL_POINTS, 0, pimpl->m_numVertices); 
                checkOpenGLError();
                break;
            default: {
                assert(false);
            }
        } 
    }
        
}



template<typename index_t>
void Mesh::_CheckIndexBounds(int numIndices, int startIndexOffset) const
{
    
    if (!ib)
        return;

    if (numIndices == -1)
        numIndices = ib->NumIndices();

    if (startIndexOffset + numIndices > ib->NumIndices())
        throw std::runtime_error("requesting check for indices that do not exist in the buffer");

    const index_t* begin = reinterpret_cast<const index_t*>(ib->CpuPtr());

    const index_t* it = begin;
    const index_t* end = begin + numIndices;

    it += startIndexOffset;


    for (; it != end; ++it)
    {
        const index_t& index = *it;

        //std::cout << "index: " << index
        //    << ", numIndices: " << numIndices
        //    << ", m_numRenderableVertices: " << m_numRenderableVertices
        //    << std::endl;
        if (!(index < pimpl->m_numRenderableVertices))
            throw std::runtime_error("Index out of range");
    }

}


void Mesh::CheckIndexBounds(int numIndices, int startIndexOffset) const
{

    if (!ib)
        return;

    if (ib->IndexType() == PrimitiveIndexType::Indices8Bit)
        _CheckIndexBounds<uint8_t>(numIndices, startIndexOffset);
    else if (ib->IndexType() == PrimitiveIndexType::Indices16Bit)
        _CheckIndexBounds<uint16_t>(numIndices, startIndexOffset);
    else if (ib->IndexType() == PrimitiveIndexType::Indices32Bit)
        _CheckIndexBounds<uint32_t>(numIndices, startIndexOffset);
    else
        assert(false);

}


void Mesh::CheckValidVBO(int startVertexOffset) const
{

    if (!(startVertexOffset < pimpl->m_numVertices)) {
        throw std::runtime_error("cannot start with a vertex offset greater than the number of vertices");
    }

    if (!ib)
    {
        if (pimpl->m_numRenderableVertices % 3 != 0)
            throw std::runtime_error("empty index buffer, but vbo has a non-divisible-by-3 length");
    }
}

void Mesh::CheckValid() const
{
    if (ib)
    {
        if (ib->IndexType() == PrimitiveIndexType::IndicesNone)
            throw std::runtime_error("Invalid Mesh: Index Buffer does not have a valid index type");        
    }
          
    if (vbs.size() == 0)
        throw std::runtime_error("Invalid Mesh: no Vertex Buffer");
        
    
    /*
    BOOST_FOREACH(const VertexBuffer& vb, vbs | indirected)
    {
        if (vb.GpuSizeInBytes() < vb.LogicalBufferSizeBytes())
            throw std::runtime_error("Invalid Mesh: GPU side of VertexBuffer does not have enough memory");
    }
    */
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Input::Input(GLFWwindow* window)
    : m_pWindow(window)
{
}


Input::~Input(void)
{
}


int Input::keyPressed(int key)
{
    return glfwGetKey(m_pWindow, key);
}


void Input::poll()
{
    glfwPollEvents();
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PixelBuffer::PixelBuffer(Usage usage, std::size_t bytes, bool allocateCpu)
    : pimpl(new PixelBufferPimpl())
{
    
    pimpl->m_pbo = 0;
    pimpl->m_usage = usage;
    pimpl->m_logicalBytes = bytes;
    pimpl->m_gpuSize = 0;
    pimpl->m_cpuData = std::shared_ptr<PixelBuffer::cpu_data_t>();
    
    /// READ THIS:
    ///https://developer.apple.com/library/mac/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_texturedata/opengl_texturedata.html

    glGenBuffers(1, &pimpl->m_pbo); 
    checkOpenGLError();

    if (allocateCpu)
        AllocateCpuMemory();
}


PixelBuffer::~PixelBuffer()
{
    if (pimpl->m_pbo)
        glDeleteBuffers(1, &pimpl->m_pbo);
    pimpl->m_pbo = 0;
}

int PixelBuffer::LogicalBufferSizeBytes() const
{
    return pimpl->m_logicalBytes;
}

int PixelBuffer::CpuSizeInBytes() const
{
    if (!pimpl->m_cpuData)
        return 0;
    return pimpl->m_cpuData->size();
}

int PixelBuffer::GpuSizeInBytes() const
{
    return pimpl->m_gpuSize;
}

bool PixelBuffer::HasGpuMemory() const
{
    return pimpl->m_gpuSize;
}
bool PixelBuffer::HasCpuMemory() const
{
    return !!pimpl->m_cpuData;
}

void PixelBuffer::Bind()
{
    checkOpenGLError();
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pimpl->m_pbo);
    checkOpenGLError();
    assert(IsBound());
}

void PixelBuffer::UnBind()
{
    PixelBuffer::UnBindAll();
}

void PixelBuffer::UnBindAll()
{
    checkOpenGLError();
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    checkOpenGLError();
}

bool PixelBuffer::IsBound() const
{
    checkOpenGLError();
    GLint boundpbo = 0;
    glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &boundpbo);
    checkOpenGLError();
    return boundpbo != 0 && boundpbo == pimpl->m_pbo;
}

void PixelBuffer::AllocateCpuMemory()
{
    int bytes = LogicalBufferSizeBytes();
   
    if (!pimpl->m_cpuData)
    {
        pimpl->m_cpuData = std::make_shared< cpu_data_t >(bytes);
    }
    else 
    {
        pimpl->m_cpuData->resize(bytes);
    }
}

void PixelBuffer::AllocateGpuMemory()
{
    assert(pimpl->m_pbo);
    assert(IsBound());
    checkOpenGLError();


    int bytes = LogicalBufferSizeBytes();

    ///Create and initialize the data store of the buffer object by calling the function glBufferData
    glBufferData(GL_PIXEL_UNPACK_BUFFER, bytes, 0, toGL(pimpl->m_usage));
    checkOpenGLError();
    pimpl->m_gpuSize = bytes;
}

void PixelBuffer::Resize(std::size_t bytes)
{
    pimpl->m_logicalBytes = bytes;
}

void PixelBuffer::UpdateToGpu() {

    if (!pimpl->m_cpuData)
    {
        throw std::runtime_error( "no cpu data to send to gpu" );
    }


    const uint8_t* data = pimpl->m_cpuData->data();
    std::size_t bytes = pimpl->m_cpuData->size();
    
    this->UpdateToGpu(data,bytes,0);

}
void PixelBuffer::UpdateToGpu(const uint8_t* data, int bytes, int gpuoffset) {
    assert(pimpl->m_pbo);
    assert(data);
    assert(bytes > 0);
    assert(IsBound());

    assert(HasGpuMemory());
    assert(!HasCpuMemory() || CpuSizeInBytes() == LogicalBufferSizeBytes());
    assert(!HasGpuMemory() || GpuSizeInBytes() == LogicalBufferSizeBytes());
    
    assert(gpuoffset + bytes <= LogicalBufferSizeBytes());
    checkOpenGLError();

    GLbitfield invalidate_bit = GL_MAP_INVALIDATE_RANGE_BIT;
    if (gpuoffset == 0 && bytes == LogicalBufferSizeBytes())
        invalidate_bit = GL_MAP_INVALIDATE_BUFFER_BIT;
    
    void* gpuptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, gpuoffset, bytes, GL_MAP_WRITE_BIT | invalidate_bit );
    if (!gpuptr)
        throw std::runtime_error("unable to map buffer");
    
    std::memcpy(gpuptr, data,bytes);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    checkOpenGLError();
}






Texture::Texture( TextureType texture_type, TextureInternalFormat internal_format
           , int width, int height, int depth, size_t mipmaps, int rowalignment, ResourceUsage usage)
    : pimpl(new TexturePimpl())
{
    if (!(rowalignment == 1 || rowalignment == 2 || rowalignment == 4 || rowalignment == 8))
        throw std::runtime_error("rowalignment: The GL_PACK/UNPACK_ALIGNMENTs can only be 1, 2, 4, or 8.");

    pimpl->m_tex = 0;
    pimpl->m_texture_type = texture_type;
    pimpl->m_internal_format = internal_format;
    pimpl->m_usage = usage;
    pimpl->m_width = width;
    pimpl->m_height = height;
    pimpl->m_depth = depth;
    pimpl->m_rowalignment = rowalignment;
    pimpl->m_mipmaps = mipmaps;
    
    pimpl->m_mipmaps = std::min(pimpl->m_mipmaps, MaxMimpapLevels());
        

    assert(width > 0);
    assert(height > 0);
    assert(depth > 0);
    assert(pimpl->m_mipmaps > 0);
    assert(pimpl->m_mipmaps <= MaxMimpapLevels());
    checkOpenGLError();
    glGenTextures(1, &pimpl->m_tex);

    assert(pimpl->m_mipmaps > 0);
    Bind();


    if (_GetRowAlignment() != rowalignment)
    {
        _SetRowAlignment(rowalignment);
    }

    if (pimpl->m_usage == ResourceUsage::UsageImmutable)
    {
        _InitializeImmutableStorage();
    } else {
        ///set the mimpap levels
        glTexParameteri(toGL(pimpl->m_texture_type), GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(toGL(pimpl->m_texture_type), GL_TEXTURE_MAX_LEVEL, pimpl->m_mipmaps-1);
    }

    checkOpenGLError();
}

namespace detail{
    template<typename T>
    static inline T log2i( T x )
    {
        T result = 0;
        T x1 = x;
        while (x1 >>= 1) ++result;
        return result;
    }
} // namespace detail

int Texture::Width() const
{
    return pimpl->m_width;
}
int Texture::Height() const
{
    return pimpl->m_height;
}
int Texture::Depth() const
{
    return pimpl->m_depth;
}


int Texture::MaxMimpapLevels() const
{
    if (pimpl->m_texture_type == TextureType::Texture1D || pimpl->m_texture_type == TextureType::Texture1DArray) {
        return detail::log2i(pimpl->m_width) + 1;
    } else if (pimpl->m_texture_type == TextureType::Texture2D
            || pimpl->m_texture_type == TextureType::TextureRectangle
            || pimpl->m_texture_type == TextureType::TextureCubeMap
            || pimpl->m_texture_type == TextureType::Texture2DArray
            || pimpl->m_texture_type == TextureType::TextureCubeMapArray) {

        return detail::log2i(std::max(pimpl->m_width, pimpl->m_height)) + 1;
    } else if (pimpl->m_texture_type == TextureType::Texture3D) {

        return detail::log2i(std::max(pimpl->m_width, std::max(pimpl->m_height, pimpl->m_depth))) + 1;
    } else {
        assert (false);
        return 1;
    }
}

void Texture::_InitializeImmutableStorage()
{
    assert(IsBound());
    assert(pimpl->m_width > 0);
    assert(pimpl->m_height > 0);
    assert(pimpl->m_depth > 0);
    assert(pimpl->m_mipmaps <= MaxMimpapLevels());
    checkOpenGLError();


    
    ///see decription of https://www.opengl.org/sdk/docs/man/html/glGetTexImage.xhtml
    if (pimpl->m_texture_type == TextureType::Texture1D) {


        glTexStorage1D(toGL(pimpl->m_texture_type),
                        pimpl->m_mipmaps,
                        toGL(pimpl->m_internal_format),
                        pimpl->m_width);
        assert(pimpl->m_height == 1 && pimpl->m_depth == 1);
    }
    else if (pimpl->m_texture_type == TextureType::Texture2D || pimpl->m_texture_type == TextureType::Texture1DArray
            || pimpl->m_texture_type == TextureType::TextureRectangle || pimpl->m_texture_type == TextureType::TextureCubeMap)
    {

        glTexStorage2D(toGL(pimpl->m_texture_type),
                        pimpl->m_mipmaps,
                        toGL(pimpl->m_internal_format),
                        pimpl->m_width,
                        pimpl->m_height);

        assert(pimpl->m_depth == 1);
    } else if (pimpl->m_texture_type == TextureType::Texture3D || pimpl->m_texture_type == TextureType::Texture2DArray
                || pimpl->m_texture_type == TextureType::TextureCubeMapArray) {

        glTexStorage3D(toGL(pimpl->m_texture_type),
                        pimpl->m_mipmaps,
                        toGL(pimpl->m_internal_format),
                        pimpl->m_width,
                        pimpl->m_height,
                        pimpl->m_depth);
    } else {
        assert(false);
    }

    checkOpenGLError();
}
Texture::~Texture()
{
    if (pimpl->m_tex)
        glDeleteTextures(1, &pimpl->m_tex);
    pimpl->m_tex = 0;
}

void Texture::Bind()
{
    checkOpenGLError();
    glBindTexture(toGL(pimpl->m_texture_type), pimpl->m_tex);
    checkOpenGLError();
}


void Texture::UnBind()
{
    checkOpenGLError();
    if (IsBound())
        glBindTexture(toGL(pimpl->m_texture_type), 0);
    checkOpenGLError();
}

bool Texture::IsBound() const
{
    checkOpenGLError();
    GLint boundtex = 0;
    glGetIntegerv(toGLBinding(pimpl->m_texture_type), &boundtex);
    checkOpenGLError();
    return boundtex != 0 && boundtex == pimpl->m_tex;
}


int Texture::_GetRowAlignment()
{
    assert(IsBound());

    GLint rowalignment = 0;
    checkOpenGLError();
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &rowalignment);
    checkOpenGLError();
    return rowalignment;
}

void Texture::_SetRowAlignment(int alignment)
{
    assert(IsBound());
    ///https://www.opengl.org/wiki/Common_Mistakes#Texture_upload_and_pixel_reads
    ///The GL_PACK/UNPACK_ALIGNMENTs can only be 1, 2, 4, or 8. So an alignment of 3 is not allowed.
    assert((alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8) && "The GL_PACK/UNPACK_ALIGNMENTs can only be 1, 2, 4, or 8.");
    checkOpenGLError();

    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    checkOpenGLError();
}
void Texture::UpdateToGpu( int width, int height, int depth, TextureFormat textureFormat
                         , const uint8_t* data, size_t dataBytesSize, int level, const std::string& debugName)
{

    if (level == 0)
        assert(width == pimpl->m_width && height == pimpl->m_height);
    assert(width > 0);
    assert(height > 0);
    assert(depth > 0);
    assert(IsBound());
    assert(pimpl->m_rowalignment == _GetRowAlignment());
    assert (LogicalSizeBytes(width, height, depth, textureFormat, pimpl->m_rowalignment) == dataBytesSize);

    if (pimpl->m_usage == ResourceUsage::UsageImmutable)
    {
        this->_ImmutableUpdateToGpu(width, height, depth, textureFormat, data, dataBytesSize, level, debugName);
    } else {

        this->_MutableUpdateToGpu(width, height, depth, textureFormat, data, dataBytesSize, level, debugName);
    }
}

void Texture::_ImmutableUpdateToGpu( int width, int height, int depth, TextureFormat textureFormat
                         , const uint8_t* data, size_t dataBytesSize, int level, const std::string& debugName)
{
    ///https://www.opengl.org/wiki/Common_Mistakes#Creating_a_complete_texture


    checkOpenGLError();

    int border = 0;

    ///see decription of https://www.opengl.org/sdk/docs/man/html/glGetTexImage.xhtml
    if (pimpl->m_texture_type == TextureType::Texture1D) {

        glTexSubImage1D(toGL(pimpl->m_texture_type),
                        level,
                        0, width,
                        toGL(textureFormat.pixelformat),
                        toGL(textureFormat.elementtype),
                        data);

        assert(height == 1 && depth == 1);
    }
    else if (pimpl->m_texture_type == TextureType::Texture2D || pimpl->m_texture_type == TextureType::Texture1DArray
            || pimpl->m_texture_type == TextureType::TextureRectangle || pimpl->m_texture_type == TextureType::TextureCubeMap)
    {

        glTexSubImage2D(toGL(pimpl->m_texture_type),
                        level,
                        0, 0, width, height,
                        toGL(textureFormat.pixelformat),
                        toGL(textureFormat.elementtype),
                        data);

        assert(depth == 1);
    } else if (pimpl->m_texture_type == TextureType::Texture3D || pimpl->m_texture_type == TextureType::Texture2DArray
                || pimpl->m_texture_type == TextureType::TextureCubeMapArray) {

        glTexSubImage3D(toGL(pimpl->m_texture_type),
                        level,
                        0, 0, 0, width, height, depth,
                        toGL(textureFormat.pixelformat),
                        toGL(textureFormat.elementtype),
                        data);
    } else {
        assert(false);
    }

    checkOpenGLError();

}
void Texture::_MutableUpdateToGpu( int width, int height, int depth, TextureFormat textureFormat
                         , const uint8_t* data, size_t dataBytesSize, int level, const std::string& debugName)
{
    ///https://www.opengl.org/wiki/Common_Mistakes#Creating_a_complete_texture

    checkOpenGLError();


    

    int border = 0;

    ///see decription of https://www.opengl.org/sdk/docs/man/html/glGetTexImage.xhtml
    if (pimpl->m_texture_type == TextureType::Texture1D) {

        glTexImage1D(GL_TEXTURE_1D, level, toGL(pimpl->m_internal_format), width, border
                        , toGL(textureFormat.pixelformat), toGL(textureFormat.elementtype), data);

        assert(height == 1 && depth == 1);
    }
    else if (pimpl->m_texture_type == TextureType::Texture2D || pimpl->m_texture_type == TextureType::Texture1DArray
            || pimpl->m_texture_type == TextureType::TextureRectangle || pimpl->m_texture_type == TextureType::TextureCubeMap)
    {
        glTexImage2D(toGL(pimpl->m_texture_type), level, toGL(pimpl->m_internal_format), width, height, border
                        , toGL(textureFormat.pixelformat), toGL(textureFormat.elementtype), data);


        assert(depth == 1);
    } else if (pimpl->m_texture_type == TextureType::Texture3D || pimpl->m_texture_type == TextureType::Texture2DArray
                || pimpl->m_texture_type == TextureType::TextureCubeMapArray) {

        glTexImage3D(toGL(pimpl->m_texture_type), level, toGL(pimpl->m_internal_format), width, height, depth, border
                        , toGL(textureFormat.pixelformat), toGL(textureFormat.elementtype), data);
    } else {
        assert(false);
    }

    checkOpenGLError();

}

void Texture::GenMipmaps()
{
    assert(IsBound());
    assert(pimpl->m_mipmaps > 0);
    checkOpenGLError();


    if (pimpl->m_mipmaps > 1)
    {
        glGenerateMipmap(toGL(pimpl->m_texture_type));
        checkOpenGLError();
    }
}




int Texture::LogicalSizeBytes(int width, int height, int depth, TextureFormat format, int rowalignment)
{
    assert(width > 0);
    assert(height > 0);
    assert(depth > 0);

    ///https://www.opengl.org/wiki/Common_Mistakes#Texture_upload_and_pixel_reads
    ///The GL_PACK/UNPACK_ALIGNMENTs can only be 1, 2, 4, or 8. So an alignment of 3 is not allowed.
    assert((rowalignment == 1 || rowalignment == 2 || rowalignment == 4 || rowalignment == 8) && "The GL_PACK/UNPACK_ALIGNMENTs can only be 1, 2, 4, or 8.");

    int baserowsize = width*toGLDim(format.pixelformat)*toGLBytes(format.elementtype);
    int unalignedbytes = baserowsize % rowalignment;
    int alignmentadjustment = (rowalignment - unalignedbytes) % rowalignment;

    int alignedrowsize = baserowsize + alignmentadjustment;

    return alignedrowsize * height * depth;
}



TextureSampler::TextureSampler()
    : addressU(TextureAddressMode::TextureAddressWrap)
    , addressV(TextureAddressMode::TextureAddressWrap)
    , addressW(TextureAddressMode::TextureAddressWrap)
    , minFilter(TextureFilterMode::Linear)
    , mipFilter(TextureFilterMode::Linear)
    , magFilter(TextureFilterMode::Linear)
    , pimpl(new TextureSamplerPimpl{/*.m_smplr=*/0})
{
    checkOpenGLError();
    glGenSamplers(1, &pimpl->m_smplr);
    checkOpenGLError();
}

TextureSampler::~TextureSampler()
{
    if (pimpl->m_smplr)
        glDeleteSamplers(1, &pimpl->m_smplr);
    pimpl->m_smplr = 0;
}



void TextureSampler::GenerateParams()
{
    std::set<TextureFilterMode> minfilters = {
        TextureFilterMode::Nearest,
        TextureFilterMode::Linear,
        TextureFilterMode::NearestMipmapNearest,
        TextureFilterMode::LinearMipmapNearest,
        TextureFilterMode::NearestMipmapLinear,
        TextureFilterMode::LinearMipmapLinear};
    std::set<TextureFilterMode> magfilters{
        TextureFilterMode::Nearest,
        TextureFilterMode::Linear};

    assert(minfilters.count(minFilter) && "minFilter is not valid");
    assert(magfilters.count(magFilter) && "magFilter is not valid");

    checkOpenGLError();
    glSamplerParameteri(pimpl->m_smplr, GL_TEXTURE_WRAP_S, toGL(addressU));
    glSamplerParameteri(pimpl->m_smplr, GL_TEXTURE_WRAP_T, toGL(addressV));
    glSamplerParameteri(pimpl->m_smplr, GL_TEXTURE_WRAP_R, toGL(addressW));
    glSamplerParameteri(pimpl->m_smplr, GL_TEXTURE_MAG_FILTER, toGL(magFilter));
    glSamplerParameteri(pimpl->m_smplr, GL_TEXTURE_MIN_FILTER, toGL(minFilter));
    checkOpenGLError();

}














TextureUnit::TextureUnit(int index)
    : m_index(index)
{
    static const int max_texture_units = MaxTextureUnits();
    assert(m_index < max_texture_units);
}


TextureUnit::~TextureUnit()
{
    glBindSampler(Index(), 0);
}

void TextureUnit::BindSampler(const TextureSampler& sampler)
{
    
    checkOpenGLError();
    glBindSampler(Index(), getHandle(sampler));
    checkOpenGLError();
}
void TextureUnit::UnBindSampler()
{    
    checkOpenGLError();
    glBindSampler(Index(), 0);
    checkOpenGLError();
}

int TextureUnit::Index() const
{
    return m_index;
}


void TextureUnit::Activate()
{
    checkOpenGLError();
    glActiveTexture(GL_TEXTURE0 + Index());
    checkOpenGLError();

}

bool TextureUnit::IsActive() const
{


    GLint active = 0;
    checkOpenGLError();
    glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
    checkOpenGLError();

    return active == GL_TEXTURE0 + Index();
}

int TextureUnit::MaxTextureUnits()
{
    GLint result = 0;
    checkOpenGLError();
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &result);
    checkOpenGLError();
    return result;
}

Fence::Fence(bool initialize)
    : pimpl(new FencePimpl{ /*.m_fence=*/ 0, /*.m_signaled=*/ false })
{
    if (initialize)
        this->Reset(true);
}


Fence::~Fence()
{
    if (pimpl->m_fence)
        glDeleteSync(pimpl->m_fence);
    pimpl->m_fence = 0;
}

void Fence::Reset(bool initialize)
{
    if (Initialized())
    {
        if (pimpl->m_fence)
            glDeleteSync(pimpl->m_fence);
        pimpl->m_fence = 0;
        pimpl->m_signaled = false;
    }

    if (initialize) {
        pimpl->m_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        pimpl->m_signaled = false;
    }
}

bool Fence::CheckWaiting()
{
    if (!Initialized())
        return false;
    if (pimpl->m_signaled)
        return false;

    GLint fence_signal = 0;
    glGetSynciv(pimpl->m_fence, GL_SYNC_STATUS, 1, NULL, &fence_signal);
    pimpl->m_signaled = fence_signal == GL_SIGNALED;

    return !pimpl->m_signaled;
}



bool Fence::Waiting() const
{
    return (pimpl->m_fence != 0 && !pimpl->m_signaled);
}


bool Fence::Initialized() const
{
    return pimpl->m_fence != 0;
}



void ClearBindings()
{

    Mesh::UnBindAll();
    //Texture::UnBindAll();
    VertexBuffer::UnBindAll();
    IndexBuffer::UnBindAll();
    PixelBuffer::UnBindAll();
    ShaderProgram::DeselectAll();
}

std::shared_ptr<ShaderProgram> DefaultShader()
{
    static std::shared_ptr<ShaderProgram> sp;
    if (!sp)
    {
        std::shared_ptr<Shader> vs = std::make_shared<Shader>(ShaderType::VertexShader);
        const char* vertex_shader =
            "#version 400\n"
            "uniform mat4 world;"
            "uniform mat4 worldViewProj;"
            "layout(location = 0) in vec3 vertex_position;\n"
            "layout(location = 1) in vec3 vertex_colour;\n"
            "\n"
            "out vec3 colour;\n"

            "void main () {\n"
            "  colour = vertex_colour;\n"
            "  gl_Position =  worldViewProj * world * vec4 (vertex_position, 1.0);\n"
            "}\n";

        vs->LoadFromString(ShaderType::VertexShader, vertex_shader);
        
        std::shared_ptr<Shader> ps = std::make_shared<Shader>(ShaderType::PixelShader);

        const char* fragment_shader =
            "#version 400\n"
            "in vec3 colour;\n"
            "out vec4 frag_colour;\n"
            "\n"
            "void main () {\n"
            "  frag_colour = vec4 (colour, 1.0);\n"
            "}\n";

        ps->LoadFromString(ShaderType::PixelShader, fragment_shader );
        
        sp = std::make_shared<ShaderProgram>();
        sp->Attach(*vs);
        sp->Attach(*ps);
        
    }
    return sp;
}



std::shared_ptr<ShaderProgram> DefaultSSPCWhiteShader()
{
    static std::shared_ptr<ShaderProgram> sp;
    if (!sp)
    {
        std::shared_ptr<Shader> vs = std::make_shared<Shader>(ShaderType::VertexShader);

        const char* vertex_shader =
            "#version 400\n"
            //"uniform mat4 world;"
            //"uniform mat4 worldViewProj;"
            "layout(location = 0) in vec2 vertex_position;\n"
            "\n"
            "void main () {\n"
            "  gl_Position =  vec4(vertex_position.xy,0,1);\n"
            "}\n";

        vs->LoadFromString(ShaderType::VertexShader, vertex_shader);
        
        std::shared_ptr<Shader> ps = std::make_shared<Shader>(ShaderType::PixelShader);
        const char* fragment_shader =
            "#version 400\n"
            "out vec4 frag_colour;\n"
            "\n"
            "void main () {\n"
            "  frag_colour = vec4 (1,1,1, 1.0);\n"
            "}\n";

        ps->LoadFromString(ShaderType::PixelShader, fragment_shader );
        
        sp = std::make_shared<ShaderProgram>();
        sp->Attach(*vs);
        sp->Attach(*ps);
        
    }
    return sp;
}

std::shared_ptr<ShaderProgram> DefaultWhiteShader()
{
    static std::shared_ptr<ShaderProgram> sp;
    if (!sp)
    {
        std::shared_ptr<Shader> vs = std::make_shared<Shader>(ShaderType::VertexShader);

        const char* vertex_shader =
            "#version 400\n"
            "uniform mat4 world;"
            "uniform mat4 worldViewProj;"
            "layout(location = 0) in vec3 vertex_position;\n"
            "\n"
            "out vec3 colour;\n"

            "void main () {\n"
            "  colour = vec3(1,1,1);\n"
            "  gl_Position =  worldViewProj * world * vec4 (vertex_position, 1.0);\n"
            "}\n";

        vs->LoadFromString(ShaderType::VertexShader, vertex_shader);
        
        std::shared_ptr<Shader> ps = std::make_shared<Shader>(ShaderType::PixelShader);
        const char* fragment_shader =
            "#version 400\n"
            "in vec3 colour;\n"
            "out vec4 frag_colour;\n"
            "\n"
            "void main () {\n"
            "  frag_colour = vec4 (1,1,1, 1.0);\n"
            "}\n";

        ps->LoadFromString(ShaderType::PixelShader, fragment_shader );
        
        sp = std::make_shared<ShaderProgram>();
        sp->Attach(*vs);
        sp->Attach(*ps);
        
    }
    return sp;
}

std::shared_ptr<ShaderProgram> DefaultTextureShader()
{
    static std::shared_ptr<ShaderProgram> sp;
    if (!sp)
    {
        std::shared_ptr<Shader> vs = std::make_shared<Shader>(ShaderType::VertexShader);
        const char* vertex_shader =
            "#version 400\n"
            "uniform mat4 world;"
            "uniform mat4 worldViewProj;"
            "layout(location = 0) in vec3 vertex_position;\n"
            "layout(location = 1) in vec2 texture_coord;\n"
            "\n"
            "out vec2 frag_texture_coord;\n"

            "void main () {\n"
            "  frag_texture_coord = texture_coord;\n"
            "  gl_Position =  worldViewProj * world * vec4 (vertex_position, 1.0);\n"
            "}\n";

        vs->LoadFromString(ShaderType::VertexShader, vertex_shader);
        
        std::shared_ptr<Shader> ps = std::make_shared<Shader>(ShaderType::PixelShader);


        const char* fragment_shader =
            "#version 400\n"
            "\n"
            "uniform sampler2D texture1;\n"
            "\n"
            "in vec2 frag_texture_coord;\n"
            "out vec4 result_colour;\n"
            "\n"
            "void main () {\n"
            "  result_colour = texture2D(texture1, frag_texture_coord);\n"
            "}\n";

        ps->LoadFromString(ShaderType::PixelShader, fragment_shader );
        
        sp = std::make_shared<ShaderProgram>();
        sp->Attach(*vs);
        sp->Attach(*ps);
        
    }
    return sp;
}


} //namespace sgfxapi

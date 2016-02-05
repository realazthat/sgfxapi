#ifndef SGFXAPI_SGFXAPI_H
#define SGFXAPI_SGFXAPI_H 1


#include "sgfxapi/sgfxapi.fwd.hpp"

#include <stdint.h>
#include <iosfwd>
#include <memory>

//#include <boost/format.hpp>
//#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/weak_ptr.hpp>

//must incldue assert first, else mathgeolib defines it
#include <cassert>
#include "MathGeoLib.h"


namespace SGFXAPI {


void initializeGlew();

int checkOglError(const char *file, int line);


enum class Access{
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE
};


enum class Usage{
    STREAM_DRAW,
    STREAM_READ,
    STREAM_COPY,
    STATIC_DRAW,
    STATIC_READ,
    STATIC_COPY,
    DYNAMIC_DRAW,
    DYNAMIC_READ,
    DYNAMIC_COPY
};

enum class VertexDataSemantic
{
    COLOR,
    COLOR2,
    NORMAL,
    TCOORD,
    VCOORD
};


enum class VertexDataType
{
    BYTE,
    UNSIGNED_BYTE,
    UNSIGNED_SHORT,
    SHORT,
    INT,
    UNSIGNED_INT,
    FLOAT,
    DOUBLE
};

enum class GPUVertexDataType
{
    DEFAULT_TO_SRC,
    INT,
    FLOAT,
    DOUBLE
};


enum class ShaderType
{
    NullShader,
    GeometryShader,
    VertexShader,
    PixelShader,
    ComputeShader,
    TessControlShader,
    TessEvalShader
};


enum class PrimitiveType
{
    Point,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip
};


enum class PrimitiveIndexType
{
    IndicesNone,
    Indices8Bit,
    Indices16Bit,
    Indices32Bit
};

enum class ResourceUsage
{
    UsageImmutable,
    UsageMutable
};

enum class TextureType
{
    Texture1D,
    Texture1DArray,
    Texture2D,
    Texture2DArray,
    TextureRectangle,
    Texture3D,

    TextureCubeMap,
    TextureCubeMapArray
};

enum class TextureAddressMode{
    TextureAddressWrap,
    TextureAddressMirrorWrap,
    TextureAddressEdgeClamp,
    TextureAddressMirrorEdgeClamp,
    TextureAddressBorderClamp
};

enum class TextureFilterMode{
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

/**
 * See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
 *
 * Section describing the @c type parameter.
 */
enum class TextureElementType{
    UNSIGNED_BYTE,
    BYTE,
    UNSIGNED_SHORT,
    SHORT,
    UNSIGNED_INT,
    INT,
    FLOAT,
    UNSIGNED_BYTE_3_3_2,
    UNSIGNED_BYTE_2_3_3_REV,
    UNSIGNED_SHORT_5_6_5,
    UNSIGNED_SHORT_5_6_5_REV,
    UNSIGNED_SHORT_4_4_4_4,
    UNSIGNED_SHORT_4_4_4_4_REV,
    UNSIGNED_SHORT_5_5_5_1,
    UNSIGNED_SHORT_1_5_5_5_REV,
    UNSIGNED_INT_8_8_8_8,
    UNSIGNED_INT_8_8_8_8_REV,
    UNSIGNED_INT_10_10_10_2,
    UNSIGNED_INT_2_10_10_10_REV
};

///https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml see the section on the @c format parameter
enum class TexturePixelFormat{
    RED,
    RG,
    RGB,
    BGR,
    RGBA,
    BGRA,
    RED_INTEGER,
    RG_INTEGER,
    RGB_INTEGER,
    BGR_INTEGER,
    RGBA_INTEGER,
    BGRA_INTEGER,
    STENCIL_INDEX,
    DEPTH_COMPONENT,
    DEPTH_STENCIL
};

///https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml see the section on the @c internalFormat parameter
enum class TextureInternalFormat{
    DEPTH_COMPONENT,
    DEPTH_STENCIL,
    RED,
    RG,
    RGB,
    RGBA,


    R8,
    R8_SNORM,
    R16,
    R16_SNORM,
    RG8,
    RG8_SNORM,
    RG16,
    RG16_SNORM,
    R3_G3_B2,
    RGB4,
    RGB5,
    RGB8,
    RGB8_SNORM,
    RGB10,
    RGB12,
    RGB16_SNORM,
    RGBA2,
    RGBA4,
    RGB5_A1,
    RGBA8,
    RGBA8_SNORM,
    RGB10_A2,
    RGB10_A2UI,
    RGBA12,
    RGBA16,
    SRGB8,
    SRGB8_ALPHA8,
    R16F,
    RG16F,
    RGB16F,
    RGBA16F,
    R32F,
    RG32F,
    RGB32F,
    RGBA32F,
    R11F_G11F_B10F,
    RGB9_E5,
    R8I,
    R8UI,
    R16I,
    R16UI,
    R32I,
    R32UI,
    RG8I,
    RG8UI,
    RG16I,
    RG16UI,
    RG32I,
    RG32UI,
    RGB8I,
    RGB8UI,
    RGB16I,
    RGB16UI,
    RGB32I,
    RGB32UI,
    RGBA8I,
    RGBA8UI,
    RGBA16I,
    RGBA16UI,
    RGBA32I,
    RGBA32UI,

    COMPRESSED_RED,
    COMPRESSED_RG,
    COMPRESSED_RGB,
    COMPRESSED_RGBA,
    COMPRESSED_SRGB,
    COMPRESSED_SRGB_ALPHA,
    COMPRESSED_RED_RGTC1,
    COMPRESSED_SIGNED_RED_RGTC1,
    COMPRESSED_RG_RGTC2,
    COMPRESSED_SIGNED_RG_RGTC2,
    COMPRESSED_RGBA_BPTC_UNORM,
    COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
    COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
    COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
};




const char* toSTR(VertexDataSemantic semantic);
const char* toSTR(VertexDataType type);
const char* toSTR(GPUVertexDataType type);

GPUVertexDataType toDefaultGPUDataType(VertexDataType src_type);

class Graphics
{
public:
    Graphics();
    ~Graphics(void);

    void Clear(bool clearDepth = true, bool clearStencil = true, bool clearColor = true,
               float red = 0.f, float green = 0.f, float blue = 0.f, float alpha = 0.f,
               float depthValue = 1.f, int stencilValue = 0);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** This class describes an element type of each entry in a VertexBuffer.
 * 
 * VertexBuffers consist of a series of entries, each of which may have several elements.
 * As an example would be a vertex buffer might contain an entry for each vertex containing
 * (normal, color, texture coordinates). A VetexDeclaration describes the data layout
 * of these entries, and a VertexElement describes each element in the VertexDeclaration.
 * 
 */
class VertexElement
{
public:

    /** Constructs a VertexElement.
     * @param semantic the "point" of the element. I believe this is mostly legacy, before using named elements;
                I'm not exactly sure what opengl does with this information,
     *          and I sometimes abuse them to mean the wrong thing to pass in data that I want.
     * @param src_type the base type of the source array; that is the array that will be copied from the CPU, if any.
     * @param dimension each element is a vector of size [1,4]; so a "color" might be of @p src_type or @p dst_type `UNSIGNED_SHORT` and of @p dimension 3.
     *          A @p dimension of 1 would mean a scalar. @p dimension must be at least 1 and at most 4.
     * @param name the name of the vertex element; it should be descriptive, like "color", "normal", or "extra-data1" etc.
     *          and no two names should conflict for the all the vertex-data (possibly throughout multiple VertexBuffers and
     *          VertexDeclarations in the same mesh) when rendering.
     * @param normalized if the destination data is a floating point, and the source data is of integer type, setting this to true
     *          will automatically convert the data to be in the range of [0,1] for unsigned types, or [-1,1] for signed
     *          types. Specifically, the number will be mapped from the full range of the source type to the normalized floating
     *          point range.
     * @param dst_type the type to convert to/store on the GPU; defaults to the same type on the CPU; opengl itself will sometimes
     *          unexpectedly convert it to something else first, even if your shader is expecting the same type as the CPU,
     *          and lose information as a result - unless you are careful. This library defaults to keeping them the same (no conversion)
     *          unless specified by changing this argument.
     *          See http://www.informit.com/articles/article.aspx?p=2033340&seqNum=3
     */
    VertexElement( VertexDataSemantic semantic
                 , VertexDataType src_type, int dimension
                 , const std::string& name
                 , bool normalized=false
                 , GPUVertexDataType dst_type = GPUVertexDataType::DEFAULT_TO_SRC);
    ~VertexElement(void);

    /**
     * @brief Calculates the size, in bytes, of this element, taking the @c type and @c dimension into account.
     * @return The size in bytes.
     */
    int SizeBytes() const;
    /**
     * @brief The dimension.
     * @return The dimension.
     */
    int SizeDimension() const;
    /**
     * @brief The semantic of this VertexElement.
     * @return The VertexDataSemantic.
     */
    VertexDataSemantic Semantic() const;
    /**
     * @brief The type of the source (CPU-side) data.
     * @return The VertexDataType.
     */
    VertexDataType SrcType() const;
    /**
     * @brief The type of the gpu data.
     * @return The VertexDataType.
     */
    GPUVertexDataType DstType() const;
    
    /**
     * @brief If the source type is an integer-like type, and the target type is a flaot-like
     *      type, then the conversion will optionally "normalize" the data; where "normalize"
     *      here means to make each float be in the range [0,1] (for unsigned source)
     *      or [-1,1] (for signed source), and the mapping is SOURCE_TYPE_MIN will be the lower
     *      part of the range, and SOURCE_TYPE_MAX will be the upper part of the range. See docs on
     *      [glVertexAttribPointer](https://www.opengl.org/sdk/docs/man/html/glVertexAttribPointer.xhtml)
     *      in the section on `normalized`.
     * @return true if the conversion should normalize the data, false otherwise. 
     */
    bool Normalized() const;

    /**
     * @brief The name of the VertexElement.
     * @return The name.
     */
    const std::string& Name() const;
    
    //void save_to_file(std::ostream& out);
    //void load_from_file(std::istream& in);

    //void dump_to_console();
    
    /**
    * Returns a string of format "(semantic: (semantic), name: (name), type: (type), count: (num))".
    */
    std::string ToString() const;
    bool Valid() const;
    
private:
    VertexDataSemantic m_semantic;
    VertexDataType m_src_type;
    GPUVertexDataType m_dst_type;
    bool m_normalized;
    int m_dimension;
    std::string m_name;
    
};


/**
* Prints a string of format "(semantic: (semantic), name: (name), type: (type), dimension: (num))".
*/
::std::ostream& operator<<(::std::ostream&out, const VertexElement& element);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class VertexDeclaration
 * @brief Holds a list of VertexElement descriptions; the list
 *          describes the contents of each vertex in a single VertexBuffer.
 *          
 */
class VertexDeclaration
{
public:
    VertexDeclaration();
    ~VertexDeclaration();
 
    /**
    * Adds a new data element to the end of the element list.
    */
    void Add(VertexElement elem);
    
    /**
    * The size of a single vertex in the stream in bytes, including any padding.
    */
    int Stride() const;

    //int SizeBytes() const;
    
    /**
     * Get at the elements.
     */
    const std::vector<VertexElement>& Elements() const;
    
    //void save_to_file(std::ostream& out);
    //void load_from_file(std::istream& in);
    //void dump_to_console();
private:

    
    std::vector<VertexElement> m_elements;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VertexBufferPimpl;

/**
 * The VertexBuffer (VBO) actually holds the vertex data array sent to the GPU.
 * The format of the data is as follows (n is the number of vertices in the mesh/chunk/draw call):
 *
 * ```
 * A VBO with n vertices:
 * | vertex 0 data || vertex 1 data || vertex 2 data || vertex 3 data |  ...  | vertex n data |
 * 
 * A single vertex's data, with k elements:
 * | vertex element 0 | vertex element 1 | vertex element 2 | ... | vertex element k |
 *
 * Example VBO:
 *
 * | pos.x | pos.y | pos.z | uv.u | uv.v | | pos.x | pos.y | pos.z | uv.u | uv.v | ...
 * In the above example the VertexDeclaration looks like:
 *
 * | "pos", float X 3 | "uv", float X 2 |
 * ```
 * 
 *
 */
class VertexBuffer 
{
public:
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
public:
    /**
     * 
     * @param mesh the mesh this VBO will be rendered with; this is for safety, to check that the mesh's
     *          VAO (vertex attributes object) is bound when doing VBO operations.
     * @param numVertices the length of the buffer, in number of vertices.
     * @param dec the layout of each vertex within the buffer's vertex-data-array.
     * @param usage indicates if this VBO will be subsequently read from, or written to during rendering.
     * @param allocateCpu allocates a CPU-side buffer for filling. The CPU side buffer is not strictly necessary
     *          if you upload the data already properly formatted. @see UpdateToGpu(), UpdateToCpu().
     *
     */
    explicit VertexBuffer(std::weak_ptr<Mesh> mesh, int numVertices, const VertexDeclaration& dec, Usage usage, bool allocateCpu=true);
    ~VertexBuffer();

    
    std::string name;
    
    int NumVertices() const;
    
    //int getSizeInBytes() const;

    int VertexSizeBytes() const;

    void SetNumVertices(std::size_t size, bool perseve_old_cpu_data = true);
    

    void UpdateToCpu(uint8_t* data=0, int bytes=0);
    void UpdateToGpu(const uint8_t* data, int size);
    void UpdateToGpu();
    void AllocateCpuMemory();
    void AllocateGpuMemory();

    bool HasCpuMemory() const;
    bool HasGpuMemory() const;
    
    const VertexDeclaration& Declaration() const;
    VertexDeclaration& Declaration();
    
    
    const unsigned char* CpuPtr() const;
    unsigned char* CpuPtr();
    
    int LogicalBufferSizeBytes() const;
    int GpuSizeInBytes() const;
    int CpuSizeInBytes() const;
    
    void Bind();
    void UnBind();
    bool IsBound() const;
    bool VAOIsBound() const;
    static void UnBindAll();

    typedef std::vector<unsigned char> cpu_data_t;
    std::unique_ptr<VertexBufferPimpl> pimpl;
private:
    
    friend class Graphics;
    friend class Mesh;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IndexBufferPimpl;

/**
 * @see getHandle(const IndexBuffer&)
 */
class IndexBuffer
{
public:
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
public:
    typedef std::vector<unsigned char> cpu_data_t;

    explicit IndexBuffer(std::weak_ptr<Mesh> mesh, int numIndices
                        , PrimitiveIndexType type
                        , Usage usage
                        , bool allocateCpu=true);
    ~IndexBuffer();
        
    std::string name;
    
    void UpdateToGpu(const uint8_t* data=0, int bytes=0);
    void UpdateToCpu(uint8_t* data=0, int bytes=0);
    void AllocateCpuMemory();
    void AllocateGpuMemory();
        
    int NumIndices() const;

    ///Returns the size of a single index in bytes (1,2 or 4, depending on indexType).
    int IndexSizeBytes() const;
    int LogicalBufferSizeBytes() const;

    bool HasCpuMemory() const;
    bool HasGpuMemory() const;

    void SetNumIndices(std::size_t size, bool perseve_old_cpu_data);
        
    const std::string& Name() const;

    unsigned char * CpuPtr();
    const unsigned char * CpuPtr() const;

    PrimitiveIndexType IndexType() const;

    void Bind();
    void UnBind();
    bool IsBound() const;
    bool VAOIsBound() const;
    static void UnBindAll();
    
    std::unique_ptr<IndexBufferPimpl> pimpl;
private:
    
    friend class Graphics;
    friend class Mesh;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct bind_guard_t{
    bind_guard_t(const bind_guard_t&) = delete;
    bind_guard_t& operator=(const bind_guard_t&) = delete;
    
    T* b;

    bind_guard_t(bind_guard_t&& o) : b(std::move(o.b)) {}
    bind_guard_t(T& b)
        : b(&b)
    {
        this->b->Bind();
    }

    
    ~bind_guard_t()
    {
        b->UnBind();
    }
};

template<typename T>
bind_guard_t<T> make_bind_guard(T& b)
{
    return bind_guard_t<T>(b);
}

/*
template<typename T>
struct map_guard_t : boost::noncopyable {
    void* mptr;

    bind_guard_t(T& b, Access access)
    {
        mptr = b.Map(access);
    }

    
    ~bind_guard_t(T& b)
    {
        b.UnBind();
    }

    void* ptr(){
        return mptr;
    }
};


template<typename T>
map_guard_t<T> make_map_guard(T& b, Access access)
{
    return map_guard_t<T>(b);
}
*/

struct PixelBufferPimpl;

class PixelBuffer
{
    PixelBuffer(const PixelBuffer&) = delete;
    PixelBuffer& operator=(const PixelBuffer&) = delete;
public:
    typedef std::vector<unsigned char> cpu_data_t;
    
    PixelBuffer(Usage usage, std::size_t bytes, bool allocateCpu=true);
    ~PixelBuffer();


    int LogicalBufferSizeBytes() const;
    int GpuSizeInBytes() const;
    int CpuSizeInBytes() const;


    void Bind();
    void UnBind();
    static void UnBindAll();
    bool IsBound() const;


    unsigned char* CpuPtr();
    const unsigned char* CpuPtr() const;
    void AllocateCpuMemory();
    void AllocateGpuMemory();
    bool HasCpuMemory() const;
    bool HasGpuMemory() const;


    /**
     * Uploads data to the GPU, assuming there is a CPU buffer.
     */
    void UpdateToGpu();
    void UpdateToGpu(const uint8_t* data, int bytes, int gpuoffset=0);


    void UpdateToCpu(uint8_t* data=0, int bytes=-1);

    void Resize(std::size_t bytes);


    /**
     * Locks the GPU buffer, and returns a pointer to the memory on the CPU.
     *
     * Must call UnMap() when reading/writing is complete.
     *
     * Allows calls to glReadPixels(), while bound, and it will not block until the
     * PBO is mapped.
     */
    //void* Map(Access access);
    //void UnMap();
    
    std::unique_ptr<PixelBufferPimpl> pimpl;
};


struct TextureUnit
{
    TextureUnit(const TextureUnit&) = delete;
    TextureUnit& operator=(const TextureUnit&) = delete;
    
    TextureUnit(int index);
    ~TextureUnit();

    void Activate();
    //void Deactivate();

    bool IsActive() const;

    int Index() const;

    void BindSampler(const TextureSampler& sampler);
    void UnBindSampler();

    static int MaxTextureUnits();
private:
    int m_index;
};




struct TextureSamplerPimpl;


/**
 * @brief Describes how the GPU should sample the texture.
 *
 * See [OpenGL 3.3 Sampler Objects: Control your Texture Units](http://www.geeks3d.com/20110908/opengl-3-3-sampler-objects-control-your-texture-units/)
 *
 * Usage:
 * \code{.cpp}
 * auto sampler = std::make_shared<TextureSampler>();
 * 
 * // the texture should wrap around in the u-direction
 * sampler->addressU = TextureAddressMode::TextureAddressWrap;
 * // the texture should take the border-value if it goes over the texture border in the v-direction.
 * sampler->addressV = TextureAddressMode::TextureAddressBorderClamp;
 *
 * sampler->GenerateParams();
 *
 * auto mesh_texture = std::make_shared<MeshTexture>(texture, texture_unit, sampler, sampler_name);
 * \endcode
 *
 * @see getHandle(const TextureSampler&)
 */
struct TextureSampler
{
    TextureSampler(const TextureSampler&) = delete;
    TextureSampler& operator=(const TextureSampler&) = delete;
    
    
    TextureSampler();
    ~TextureSampler();


    //void Bind();
    //void UnBind();
    //bool IsBound() const;
    //static void UnBindAll();

    TextureAddressMode addressU;
    TextureAddressMode addressV;
    TextureAddressMode addressW;
    TextureFilterMode minFilter;
    TextureFilterMode mipFilter;
    TextureFilterMode magFilter;

    /**
     * This should be called after construction, and/or after changing any member values
     * and before using this object for rendering. GenerateParams() "saves" the values
     * to the GPU-side.
     *
     */
    void GenerateParams();

    std::unique_ptr<TextureSamplerPimpl> pimpl;
};


/**
 *
 *
 *
 */
struct TextureFormat
{
    TextureFormat(TextureElementType elementtype, TexturePixelFormat pixelformat)
        : elementtype(elementtype), pixelformat(pixelformat)
    {}

    TextureElementType elementtype;
    TexturePixelFormat pixelformat;
};


struct TexturePimpl;

/**
 *
 * @see getHandle(const Texture&)
 */
class Texture
{
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
public:
    /**
     * @param texture_type
     *      The type of texture, 1D,2D,3D/arrays etc.
     * @param internal_format
     *      A suggestion to the GPU of the type of format to store the image in, internally, on the GPU.
     * @param usage
     *      If this texture parameters will change (they won't, because the parameters are set in the ctor).
     * @param width
     *      The width of the image; for 1D, the length of the image.
     * @param height
     *      The height of the image, for 1D, this will be 1.
     * @param depth
     *      The depth of the image, for 1D,2D, this will be 1.
     * @param mipmaps
     *      The number of mipmaps this texture will have.
     * @param rowalignment
     *      The uploaded data will have this rowalignment. See @c UpdateToGpu(). Defaults to 4.
     */
    Texture(TextureType texture_type, TextureInternalFormat internal_format, ResourceUsage usage
           , int width, int height, int depth, int mipmaps=1000, int rowalignment = 4);
    ~Texture();

    ///Given the dimensions and type of texture, this function returns the maximum number of mipmap
    /// levels.
    int MaxMimpapLevels() const;
    
    /**
     * Set a mipmap level of the texture.
     *
     * @param textureFormat
     *      The format of the data being uploaded. The data will be stored according to the @c internal_format
     *      parameter in the ctor.
     * @param data pointer to a buffer with the data for the texture.
     *        If a PBO is bound, data is the offset into the PBO.
     * @param dataBytesSize the size of the entire buffer. This is basically rowsize*width*height,
     *        where rowsize is (sizeperpixel*width) adjusted for @c rowalignment, specified in the ctor.
     * @param level this specifies the mipmap level to upload the data for. Level 0 *must* be uploaded.
     *        Other levels are optional, depending on the @c mipmaps parameter specified in the ctor.
     *        Call @c GenMipmaps() to automatically generate the mipmaps from level 0.
     */
    void UpdateToGpu( int width, int height, int depth, TextureFormat textureFormat
                         , const uint8_t* data, size_t dataBytesSize, int level=0, const std::string& debugName=std::string());

    /*
    void CreateNew( int width, int height, int depth, int numMipmaps
                  , TextureType type, TextureFormat format, ResourceUsage usage
                  , bool cpuReadAccess, bool cpuWriteAccess
                  , const uint8_t **initialSurfaceDataArray, size_t *initialSurfaceDataByteSizes, int numInitialSurfaces
                  , const char *debugName=0);
    void CreateNew( int width, int height, int depth, int numMipmaps
                  , TextureType type, TextureFormat format, ResourceUsage usage
                  , bool cpuReadAccess, bool cpuWriteAccess
                  , const Image *initialImageArray, int numInitialImages, const char *debugName=0);
    */
    
    /**
     * Automatically generate mipmaps.
     */
    void GenMipmaps();

    ///Activate this texture, to be used in the following calls; some methods of this class
    /// may require that this texture is in the "bound" state before using them
    ///@see UnBind(), UnBindAll(), IsBound()
    void Bind();
    ///Deactivate this texture; this function checks with the driver that this texture is bound;
    /// and if it is not already bound, it is a noop; use UnBindAll() to clear the bound state of
    /// all/any texture.
    ///@see Bind(), UnBindAll(), IsBound()
    void UnBind();
    ///Check if this texture is "bound"
    ///@see Bind(), UnBind(), UnBindAll()
    bool IsBound() const;
    ///Deactivate *any* texture that is bound
    ///@see Bind(), UnBind(), IsBound()
    static void UnBindAll();


    const std::string& Name() const;
    int Height() const;
    int Width() const;
    int Depth() const;
    TextureFormat Format() const;
    int NumMipmaps() const;
    //void SetNumMipmaps(int numMipLevels, bool generateNewMipLevels=true);
    int CpuSizeInBytes() const;
    int GpuSizeInBytes() const;
    void DisableMipmaps();
    static int LogicalSizeBytes(int width, int height, int depth, TextureFormat format, int rowalignment);

    std::unique_ptr<TexturePimpl> pimpl;
private:
    void _InitializeImmutableStorage();

    int _GetRowAlignment();
    void _SetRowAlignment(int alignment);

    void _MutableUpdateToGpu( int width, int height, int depth, TextureFormat textureFormat
                         , const uint8_t* data, size_t dataBytesSize, int level, const std::string& debugName=std::string());
    void _ImmutableUpdateToGpu( int width, int height, int depth, TextureFormat textureFormat
                         , const uint8_t* data, size_t dataBytesSize, int level, const std::string& debugName=std::string());
                         
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct ShaderPimpl;

/**
 * This is a single shader, of a particular type; either pixel shader, .
 *
 * @see getHandle(const Shader& shader)
 */
class Shader
{
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
public:
    explicit Shader(ShaderType type);
    ~Shader();
    
    /**
     * Load the shader from source.
     *
     * @param shaderData should be a null-terminated c-string, containing the source code.
     * @param entryPoint must be 0
     * @param profile must be 0
     * @see LoadFromFile()
     */
    void LoadFromString(ShaderType type,
                        const char* shaderData,
                        const char* entryPoint=0,
                        const char* profile=0);

    /**
     * Load the shader from a source file.
     *
     * @copydoc LoadFromString()
     * @see LoadFromString()
     */
    void LoadFromFile(ShaderType type,
                        const char* fileName,
                        const char* entryPoint=0,
                        const char* profile=0);

    std::unique_ptr<ShaderPimpl> pimpl;
    
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ShaderProgramPimpl;

/**
 * Bundles up shaders and links them into a single "program" for the GPU pipeline to use.
 *
 * @see getHandle(const ShaderProgram& sp)
 */
class ShaderProgram
{
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
public:
    explicit ShaderProgram();
    ~ShaderProgram();
    
    /**
     * Attaches a shader to this ShaderProgram.
     * 
     * Note on lifetimes: it is safe to delete the shader right after attaching it to the ShaderProgram.
     *
     */
    void Attach(Shader& shader);
    
    ///Get the index of a uniform variable in the shader. Each uniform variable has an index.
    int GetUniformLocation(const char* name);
    
    ///Call this before drawing to activate the shader program.
    void Use();
    ///Check if this shader program is in use.
    ///@see Use()
    bool InUse() const;
    ///If this shader is in use, deactivate it. Note that this method will check with the driver if this
    /// shader is in use before deactivating it; use DeselectAll() if you want to
    /// deactivate *all* shaders from being in use.
    void Deselect();
    
    ///Deselects all shaders
    static void DeselectAll();

    void BindTexture(int index, TextureUnit& texture_unit, Texture& texture, const std::string& samplerName);

    void SetUniform(const float4x4& matrix, const std::string& name);
    void SetUniform(const float value, const std::string& name);
    void SetUniform(const int value, const std::string& name);
    void SetUniform(const float v1, const float v2, const float v3, const std::string& name);

    static void SetFloat4x4(int parameterIndex, const float4x4& matrix);
    static void SetFloat(int parameterIndex, const float value);
    static void SetInt(int parameterIndex, const int value);
    static void SetFloat3(int parameterIndex, const float v1, const float v2, const float v3);

    std::unique_ptr<ShaderProgramPimpl> pimpl;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Just a container that bundles together the texture, texture unit, sampler, and texture name (for retrieval in the shader).
 */
struct MeshTexture{
    MeshTexture(  std::shared_ptr<Texture> texture
                , std::shared_ptr<TextureUnit> texture_unit
                , std::shared_ptr<TextureSampler> sampler
                , std::string sampler_name)
        : texture(texture)
        , texture_unit(texture_unit)
        , sampler(sampler)
        , sampler_name(sampler_name)
    {}

    ///store the texture here
    std::shared_ptr<Texture> texture;
    ///store the texture unit here
    std::shared_ptr<TextureUnit> texture_unit;
    ///store the sampler here
    std::shared_ptr<TextureSampler> sampler;
    ///give the sampler a name, this is the name that will be passed into the shader
    std::string sampler_name;
};



struct MeshPimpl;

/**
 * Bundles together one or more VertexBuffer objects, an optional IndexBuffer, a ShaderProgram,
 * and one or more Texture objects, and bounds them together into a "VAO" for rendering.
 *
 */
class Mesh
{
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
public:
    explicit Mesh(PrimitiveType primType);
    ~Mesh();
    
    /// Add VertexBuffer objects to the Mesh via this public member.
    std::vector<std::shared_ptr<VertexBuffer> > vbs;
    /// Set the IndexBuffer to the mesh - if using indexed geometry - via this public member.
    std::shared_ptr<IndexBuffer> ib;
    /// Set the ShaderProgram to the mesh via this public member.
    std::shared_ptr<ShaderProgram> sp;
    /// Add MeshTexture objects to the mesh via this public member.
    std::vector<std::shared_ptr<MeshTexture> > textures;
    
    
    /**
     * Call this once, after the mesh's properties are set, before calling LinkShaders().
     * 
     * @param startVertexOffset tells the mesh which vertex is the 0th vertex of the mesh.
     *          The IndexBuffer will count off this starting vertex if the mesh is indexed.
     *          Otherwise, the mesh will start at the vertex indicated, in groups of 3 vertices,
     *          with one group of 3 vertices per triangle. Default is 0th vertex.
     */
    void GenerateVAO(int startVertexOffset=0);
    
    /**
     * Call this once, after the mesh has generated a VAO, and before ever calling Bind().
     *
     */
    void LinkShaders();
    
    /**
     * Bind the VAO. Call this to bind the Mesh; before calling Draw(), the mesh must be bound.
     */
    void Bind();
    ///Unbind the VAO
    void UnBind();
    
    /**
     * Check if the Mesh is bound.
     *
     *
     * @returns true if the Mesh is bound, else returns false.
     *
     * @see Mesh::Bind()
     */
    bool IsBound() const;
    
    /// Static method to unbind all VAOs.
    /// @see Mesh::Bind(), Mesh::IsBound()
    static void UnBindAll();
    
    /**
     * Draw the Mesh.
     *
     * Mesh must be bound before calling this. The ShaderProgram of this mesh (Mesh::sp)
     * should be "in use" (call ShaderProgram::Use()) before drawing to render properly.
     *
     * @param numIndices the number of indices to use for this mesh; default is -1 which means
     *          all of the indices in the IndexBuffer if using indexed geometry,
     *          or all of the implicit indices/triangles in the VertexBuffer objects if not using indexed
     *          geometry.
     * @param startIndexOffset the start index to use for this mesh; default is 0 which means
     *          the first index in the IndexBuffer if using indexed geometry,
     *          or all of the implicit indices/triangles in the VertexBuffer.
     *
     * @see Mesh::Bind(), Mesh::sp, ShaderProgram::Use()
     */
    void Draw(int numIndices=-1, int startIndexOffset=0);

    void CheckValid() const;
    void CheckValidVBO(int startVertexOffset=0) const;

    ///This loops through all the indices and checks that they are valid.
    void CheckIndexBounds(int numIndices=-1, int startIndexOffset=0) const;

    ///Return the effective vertex-declaration across all VBOs in the mesh.
    const VertexDeclaration& Declaration() const;

    /**
    * Returns the type of primitives used by this mesh (PrimPoint, PrimLineList, PrimLineStrip, PrimTriangleList or PrimTriangleStrip).
    */
    PrimitiveType PrimType() const;

    std::unique_ptr<MeshPimpl> pimpl;
private:
    
    
    template<typename index_t>
    void _CheckIndexBounds(int numIndices=-1, int startIndexOffset=0) const;

    friend class Graphics;
    
};


struct FencePimpl;

/**
 * Calls to the GPU may not happen immediately. Sometimes it is useful to know
 * when a certain point in the call-stream is reached; the Fence class gives this ability.
 * Construct a fence, and poll it later to see if that point in the stream was yet processed.
 *
 */
class Fence
{
    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;
public:

    /**
     * Constructor; when constructed with @param initialize set to @c true, this will put
     * a placeholder in the GPU processing stream.
     *
     * @param initialize if set to true, this will put the placeholder in the GPU processing stream;
     *      otherwise, this will create an empty fence that must later be initlaized.
     */
    Fence(bool initialize=true);
    ~Fence();

    /**
     * Uninitializes/destroys the current fence, if any, and optionally places another fence into the
     * GPU processing stream.
     *
     * @param initialize if set to true, this will put the placeholder in the GPU processing stream;
     *      otherwise, this will create an empty fence that must later be initlaized.
     */
    void Reset(bool initialize);

    /**
     * This checks with the GPU to see if the execution has passed this Fence,
     * and then sets the state of this Fence object. To only check the
     * state of the fence, use Waiting().
     *
     * @see Waiting()
     */
    bool CheckWaiting();
    
    /**
     * This checks the state of this fence object; note that this call does not poll
     * the GPU; to do that, use CheckWaiting().
     *
     * @see CheckWaiting()
     */
    bool Waiting() const;
    
    /// Check if this fence is initialized.
    bool Initialized() const;


    std::unique_ptr<FencePimpl> pimpl;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * Convenience structure to bundle together a Mesh and a position/scale/rotation matrix.
 */
class RenderNode
{
public:

    std::shared_ptr<Mesh> mesh;
    float4x4 xform;
};


void ClearBindings();


std::shared_ptr<ShaderProgram> DefaultShader();
std::shared_ptr<ShaderProgram> DefaultWhiteShader();
std::shared_ptr<ShaderProgram> DefaultSSPCWhiteShader();
std::shared_ptr<ShaderProgram> DefaultTextureShader();


} // namespace SGFXAPI



#endif

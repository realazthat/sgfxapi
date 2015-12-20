
#include "sgfxapi/sgfxapi-drawutils.hpp"
#include "sgfxapi/sgfxapi.hpp"
#include "cubelib.h"

#include <cmath>
#include <vector>
#include <memory>

std::shared_ptr<SGFXAPI::Mesh> axes(float width, float length)
{
    

  using namespace SGFXAPI;
  Mesh::UnBindAll();


  int numvertices = 6 //vertices
                  * 3 //axes
                  ;
  int numindices = 2 //triangles per face
                 * 2 //faces per axes
                 * 3 //indices per triangle
                 * 3 //axes
                 ;

  VertexDeclaration dec;
  dec.Add(VertexElement(VertexDataSemantic::VCOORD, VertexDataType::FLOAT, 3, "vertex_position"));
  dec.Add(VertexElement(VertexDataSemantic::COLOR, VertexDataType::FLOAT, 3, "vertex_color"));

  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(PrimitiveType::TriangleList);
  auto vb = std::make_shared<VertexBuffer>(mesh,numvertices, dec, Usage::STATIC_DRAW);
  auto ib = std::make_shared<IndexBuffer>(mesh, numindices,PrimitiveIndexType::Indices32Bit, Usage::STATIC_DRAW);

  mesh->vbs.push_back(vb);
  mesh->ib = ib;
  mesh->sp = DefaultShader();


  float3 colors[3] = {float3(1,0,0), float3(0,1,0), float3(0,0,1)};

  corner_t base_corner = get_corner_by_int3(0,0,0);

  float* vertex_data0 = static_cast<float*>(static_cast<void*>(vb->CpuPtr()));
  float* vertex_data = vertex_data0;
  std::size_t vertex_size = dec.Stride();

  typedef uint32_t index_t;

  index_t* index_data0 = static_cast<index_t*>(static_cast<void*>(ib->CpuPtr()));
  index_t* index_data = index_data0;

  for (int axis : {0,1,2})
  {

    corner_t tipcorner = calc_cnr_adj_cnr(base_corner, axis);
    float3 tip0 = length * float3(get_corner_unitx(tipcorner), get_corner_unity(tipcorner), get_corner_unitz(tipcorner));
    float3 base0(0,0,0);

    //std::cout << "tip0: " << tip0 << std::endl;

    std::size_t base0_index = ((vertex_data - vertex_data0)*sizeof(float)) / vertex_size;
    for (int i = 0; i < 3; ++i) *vertex_data++ = base0[i];
    for (int i = 0; i < 3; ++i) *vertex_data++ = colors[axis][i];


    std::size_t tip0_index = ((vertex_data - vertex_data0)*sizeof(float)) / vertex_size;
    for (int i = 0; i < 3; ++i) *vertex_data++ = tip0[i];
    for (int i = 0; i < 3; ++i) *vertex_data++ = colors[axis][i];

    ///Each arrow has a base extending out slightly to the other axes.
    for (int baseaxis : {0,1,2})
    {
      if (baseaxis == axis)
        continue;

      corner_t corneri = calc_cnr_adj_cnr(base_corner, baseaxis);

      float3 basei = width * float3(get_corner_unitx(corneri), get_corner_unity(corneri), get_corner_unitz(corneri));
      float3 tipi = basei + tip0;

      //std::cout << "base(i): " << basei << "tip(i): " << tipi << std::endl;

      std::size_t basei_index = ((vertex_data - vertex_data0)*sizeof(float)) / vertex_size;
      for (int i = 0; i < 3; ++i) *vertex_data++ = basei[i];
      for (int i = 0; i < 3; ++i) *vertex_data++ = colors[axis][i];

      std::size_t tipi_index = ((vertex_data - vertex_data0)*sizeof(float)) / vertex_size;
      for (int i = 0; i < 3; ++i) *vertex_data++ = tipi[i];
      for (int i = 0; i < 3; ++i) *vertex_data++ = colors[axis][i];

      *index_data++ = base0_index;
      *index_data++ = basei_index;
      *index_data++ = tip0_index;

      *index_data++ = tipi_index;
      *index_data++ = tip0_index;
      *index_data++ = basei_index;
    }
  }

  assert(((vertex_data - vertex_data0) * sizeof(float)) / vertex_size  == numvertices);
  assert((index_data - index_data0) == numindices);

  {
    auto mesh_bind = make_bind_guard(*mesh);
    vb->Bind();
    ib->Bind();

    vb->AllocateGpuMemory();
    ib->AllocateGpuMemory();

    vb->UpdateToGpu();
    ib->UpdateToGpu();

    mesh->GenerateVAO();
    mesh->LinkShaders();
  }
  return mesh;
}


std::shared_ptr<SGFXAPI::Mesh> simpletri()
{

  using namespace SGFXAPI;
  Mesh::UnBindAll();


  int numvertices = 3;
  int numindices = 3;

  VertexDeclaration dec;
  dec.Add(VertexElement(VertexDataSemantic::VCOORD, VertexDataType::FLOAT, 3, "vertex_position"));
  dec.Add(VertexElement(VertexDataSemantic::COLOR, VertexDataType::FLOAT, 3, "vertex_color"));

  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(PrimitiveType::TriangleList);
  auto vb = std::make_shared<VertexBuffer>(mesh,numvertices, dec, Usage::STATIC_DRAW);
  //auto ib = std::make_shared<IndexBuffer>(mesh, numindices,PrimitiveIndexType::Indices32Bit, Usage::STATIC_DRAW);


  mesh->vbs.push_back(vb);
  //mesh->ib = ib;
  mesh->sp = DefaultShader();



  float* vertex_data0 = static_cast<float*>(static_cast<void*>(vb->CpuPtr()));
  float* vertex_data = vertex_data0;
  std::size_t vertex_size = dec.Stride();


  typedef uint32_t index_t;

  //index_t* index_data0 = static_cast<index_t*>(static_cast<void*>(ib->CpuPtr()));
  //index_t* index_data = index_data0;



  float points[] = {
     0.0f,  0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
  };

  float colours[] = {
    1.0f, 0.0f,  0.0f,
    0.0f, 1.0f,  0.0f,
    0.0f, 0.0f,  1.0f
  };

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      *vertex_data++ = points[i*3 + j];
      *vertex_data++ = colours[i*3 + j];
    }


  //*index_data++ = 0;
  //*index_data++ = 2;
  //*index_data++ = 1;


  assert(((vertex_data - vertex_data0)*sizeof(float)) / vertex_size == numvertices);
  //assert((index_data - index_data0) == numindices);
  //assert(index_data == ib->CpuPtr() + ib->LogicalBufferSize());


  {

    //auto mesh_bind = make_bind_guard(*mesh);
    mesh->Bind();
    vb->Bind();
    //ib->Bind();
    //vb->AllocateGpuMemory();
    //ib->AllocateGpuMemory();

    vb->UpdateToGpu();
    //ib->UpdateToGpu();

    //std::cout << "vb->LogicalBufferSizeBytes(): " << vb->LogicalBufferSizeBytes()
    //  << ", vb->GpuSizeInBytes(): " << vb->GpuSizeInBytes()
    //  << std::endl;

    mesh->GenerateVAO();
    mesh->LinkShaders();
  }

  mesh->CheckValidVBO();
  mesh->CheckIndexBounds();

  Mesh::UnBindAll();
  return mesh;
}

#if 0
std::shared_ptr<SGFXAPI::Mesh> cube(float length)
{

  using namespace SGFXAPI;
  Mesh::UnbindVAO();


  int numvertices = 4 //vertices
                  * 6 //faces
                  ;
  int numindices = 2 //triangles per face
                 * 6 //faces
                 * 3 //indices per triangle
                 ;

  VertexDeclaration dec;
  dec.add(VertexElement(VertexDataSemantic::VCOORD, VertexDataType::FLOAT, 3, "vertex_position"));
  dec.add(VertexElement(VertexDataSemantic::COLOR, VertexDataType::FLOAT, 3, "vertex_color"));

  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(PrimitiveType::TriangleList);
  mesh->vbs.push_back(std::make_shared<VertexBuffer>(numvertices, dec));
  mesh->ib.reset(new IndexBuffer(numindices, dec,PrimitiveIndexType::Indices32Bit));
  mesh->sp = DefaultShader();

  auto vb = mesh->vbs.back();
  auto ib = mesh->ib;
  //mesh->sp = sp;

  //float3 colors[3] = {float3(1,0,0), float3(0,1,0), float3(0,0,1)};
  float3 color(1,1,1);

  corner_t base_corner = get_corner_by_int3(0,0,0);

  float* vertex_data0 = static_cast<float*>(static_cast<void*>(vb->getCpuPtr()));
  float* vertex_data = vertex_data0;
  std::size_t vertex_size = dec.getSize();
  assert(vertex_data);

  typedef uint32_t index_t;

  index_t* index_data0 = static_cast<index_t*>(static_cast<void*>(ib->getCpuPtr()));
  index_t* index_data = index_data0;
  assert(index_data);

  for (face_t face : all_faces)
  {
    for (corner_t corner : dir_adj_cnrs(face))
    {
      float3 corner_vertex(get_corner_unitx(corner), get_corner_unity(corner), get_corner_unitz(corner));

      for (int i = 0; i < 3; ++i) *vertex_data++ = corner_vertex[i];
      for (int i = 0; i < 3; ++i) *vertex_data++ = color[i];


    }
  }

  assert((vertex_data - vertex_data0) / vertex_size == numvertices);
  assert((index_data - index_data0) / sizeof(index_t) == numindices);

  vb->updateToGpu();
  ib->updateToGpu();

  mesh->GenerateVAO();
  mesh->LinkShaders();
  return mesh;
}

#endif












std::shared_ptr<SGFXAPI::Mesh> reguizdemo()
{
  using namespace SGFXAPI;
  Mesh::UnBindAll();
  //////////////////////////////////
  //////////////////////////////////
  // SET UP SHADER
  auto mesh = std::make_shared<Mesh>(PrimitiveType::TriangleList);

  std::shared_ptr<Shader> vs = std::make_shared<Shader>(ShaderType::VertexShader);
  vs->LoadFromFile(ShaderType::VertexShader, "main.vs");

  std::shared_ptr<Shader> ps = std::make_shared<Shader>(ShaderType::PixelShader);
  ps->LoadFromFile(ShaderType::PixelShader, "main.ps");

  auto sp = std::make_shared<ShaderProgram>();
  sp->Attach(*vs);
  sp->Attach(*ps);

  /////////////////////////////////
  /////////////////////////////////
  // SET UP MESH

  VertexDeclaration decl;
  
  decl.Add(VertexElement(VertexDataSemantic::VCOORD, 
                                 VertexDataType::FLOAT, 
                                 3,
                                 "vertex_position"));


  auto pVertexBuffer = std::make_shared<VertexBuffer>(mesh, 3, decl, Usage::STATIC_DRAW);

  float * vbPtr = reinterpret_cast<float*>(pVertexBuffer->CpuPtr());
  uint32_t offset = 0;

  // FILL VERTEX DATA

  vbPtr[0] = -1.0f;
  vbPtr[1] = -1.0f;
  vbPtr[2] =  0.0f;

  vbPtr[3] = 1.0f;
  vbPtr[4] = -1.0f;
  vbPtr[5] = 0.0f;

  vbPtr[6] = 0.0f;
  vbPtr[7] = 1.0f;
  vbPtr[8] = 0.0f;

  {
    auto mesh_bind = make_bind_guard(*mesh);
    auto vb_bind = make_bind_guard(*pVertexBuffer);
    pVertexBuffer->AllocateGpuMemory();
    pVertexBuffer->UpdateToGpu();

    mesh->vbs.push_back(pVertexBuffer);

    mesh->sp = sp;


    mesh->GenerateVAO();
    mesh->LinkShaders();
  }

  return mesh;
}



std::shared_ptr<SGFXAPI::Mesh> basictexture()
{

  using namespace SGFXAPI;
  Mesh::UnBindAll();

  auto mesh = std::make_shared<Mesh>(PrimitiveType::TriangleList);


  auto sp = DefaultTextureShader();

  /////////////////////////////////
  /////////////////////////////////
  // SET UP MESH

  VertexDeclaration decl;
  
  decl.Add(VertexElement(VertexDataSemantic::VCOORD, 
                                 VertexDataType::FLOAT, 
                                 3,
                                 "vertex_position"));
  decl.Add(VertexElement(VertexDataSemantic::TCOORD, 
                                 VertexDataType::FLOAT, 
                                 2,
                                 "texture_coord"));


  auto pVertexBuffer = std::make_shared<VertexBuffer>(mesh,3, decl, Usage::STATIC_DRAW);

  float* vbPtr0 = reinterpret_cast<float*>(pVertexBuffer->CpuPtr());
  float* vbPtr = vbPtr0;
  uint32_t offset = 0;

  // FILL VERTEX DATA

  ///position
  *vbPtr++ = -1.0f;
  *vbPtr++ = -1.0f;
  *vbPtr++ =  0.0f;

  //uv
  *vbPtr++ = 0;
  *vbPtr++ = 0;


  ///position
  *vbPtr++ = 1.0f;
  *vbPtr++ = -1.0f;
  *vbPtr++ = 0.0f;

  //uv
  *vbPtr++ = 1;
  *vbPtr++ = 0;


  ///position
  *vbPtr++ = 0.0f;
  *vbPtr++ = 1.0f;
  *vbPtr++ = 0.0f;

  //uv
  *vbPtr++ = .5;
  *vbPtr++ = 1;


  mesh->vbs.push_back(pVertexBuffer);

  mesh->sp = sp;

  ///Seal the VAO
  {
    auto mesh_bind = make_bind_guard(*mesh);

    auto vb_bind = make_bind_guard(*pVertexBuffer);
    pVertexBuffer->AllocateGpuMemory();
    pVertexBuffer->UpdateToGpu();
    
    mesh->GenerateVAO();
    mesh->LinkShaders();
  }

  ///Generate texture
  {
    int width = 100;
    int height = 100;
    int depth = 1;

    auto texture = std::make_shared<Texture>(TextureType::Texture2D, TextureInternalFormat::RGBA8, ResourceUsage::UsageImmutable
                                              , width, height, depth);

    std::vector<float> bmp(width*height*depth*4, 0);

    std::size_t index = 0;
    for (int i = 0; i < width; ++i)
    {
      for (int j = 0; j < height; ++j)
      {
        for (int k = 0; k < depth; ++k)
        {
          bmp[index++] = float(i) / width;
          bmp[index++] = float(j) / height;
          bmp[index++] = float(k) / depth;
          bmp[index++] = 1;
        }
      }
    }
    assert(index == bmp.size());

    auto texture_bind = make_bind_guard(*texture);

    texture->UpdateToGpu( width, height, depth
                        , TextureFormat(TextureElementType::FLOAT, TexturePixelFormat::RGBA)
                        , reinterpret_cast<uint8_t*>(bmp.data())
                        , bmp.size()*sizeof(float) );
    texture->GenMipmaps();

    auto texture_unit = std::make_shared<TextureUnit>(0);
    auto sampler = std::make_shared<TextureSampler>();
    sampler->GenerateParams();
    std::string sampler_name = "texture1";

    auto mesh_texture = std::make_shared<MeshTexture>(texture, texture_unit, sampler, sampler_name);
    mesh->textures.push_back(mesh_texture);
  }


  return mesh;

}






std::shared_ptr<SGFXAPI::ShaderProgram> SSTQRGBDShader()
{
  using namespace SGFXAPI;

  static std::shared_ptr<ShaderProgram> sp;
  if (!sp)
  {
      std::shared_ptr<Shader> vs = std::make_shared<Shader>(ShaderType::VertexShader);

      const char* vertex_shader =
          "#version 400\n"
          "layout(location = 0) in vec2 vertex_position;\n"
          "\n"
          "out vec2 uv;\n"

          "void main () {\n"
          "  uv = (vertex_position +1)/ 2.0;\n"
          "  gl_Position =  vec4(vertex_position,0,1.0);\n"
          "}\n";

      vs->LoadFromString(ShaderType::VertexShader, vertex_shader);
      
      std::shared_ptr<Shader> ps = std::make_shared<Shader>(ShaderType::PixelShader);

      const char* fragment_shader =
          "#version 400\n"
          "\n"
          "uniform sampler2D texture1;\n"
          "\n"
          "in vec2 uv;\n"
          "out vec4 frag_color;\n"
          "\n"
          "void main () {\n"
          "  vec4 cd = texture2D(texture1, uv);\n"
          //"  frag_color = cd;//vec4(cd.rgb,1.0);\n"
          "  frag_color = vec4(cd.rgb,1.0);\n"
          //"  frag_color = vec4(1,1,1,1.0);\n"
          //"  frag_color = vec4(1,1,1,1);\n"
          "\n"
          "  gl_FragDepth = cd.a;\n"
          "}\n";

      ps->LoadFromString(ShaderType::PixelShader, fragment_shader );
      
      sp = std::make_shared<ShaderProgram>();
      sp->Attach(*vs);
      sp->Attach(*ps);
      
  }
  return sp;
}


std::shared_ptr<SGFXAPI::Texture> gentesttexture(int width = 100, int height = 100)
{
  using namespace SGFXAPI;

  int depth = 1;

  auto texture = std::make_shared<Texture>(TextureType::Texture2D, TextureInternalFormat::RGBA8, ResourceUsage::UsageImmutable
                                            , width, height, depth);

  std::vector<float> bmp(width*height*depth*4, 0);

  std::size_t index = 0;
  for (int i = 0; i < width; ++i)
  {
    for (int j = 0; j < height; ++j)
    {
      for (int k = 0; k < depth; ++k)
      {
        bmp[index++] = float(i) / width;
        bmp[index++] = float(j) / height;
        bmp[index++] = float(k) / depth;
        bmp[index++] = .5;
      }
    }
  }
  assert(index == bmp.size());

  auto texture_bind = make_bind_guard(*texture);

  texture->UpdateToGpu( width, height, depth
                      , TextureFormat(TextureElementType::FLOAT, TexturePixelFormat::RGBA)
                      , reinterpret_cast<uint8_t*>(bmp.data())
                      , bmp.size()*sizeof(float) );
  texture->GenMipmaps();

  return texture;
}


std::shared_ptr<SGFXAPI::Mesh> SSTQRGBD(bool indexed)
{

  using namespace SGFXAPI;
  Mesh::UnBindAll();

  auto mesh = std::make_shared<Mesh>(PrimitiveType::TriangleList);


  auto sp = SSTQRGBDShader();

  /////////////////////////////////
  /////////////////////////////////
  // SET UP MESH

  VertexDeclaration decl;
  
  decl.Add(VertexElement(VertexDataSemantic::VCOORD, 
                                 VertexDataType::FLOAT, 
                                 2,
                                 "vertex_position"));

  auto pVertexBuffer = std::make_shared<VertexBuffer>(mesh,6, decl, Usage::STATIC_DRAW);

  float* vbPtr0 = reinterpret_cast<float*>(pVertexBuffer->CpuPtr());
  float* vbPtr = vbPtr0;
  uint32_t offset = 0;

  // FILL VERTEX DATA

  ///position
  *vbPtr++ = -1 + .1;
  *vbPtr++ = -1 + .1;


  ///position
  *vbPtr++ = -1.0f + .1;
  *vbPtr++ = 1.0f - .1;


  ///position
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = 1.0f - .1;


  ///position
  *vbPtr++ = -1 + .1;
  *vbPtr++ = -1 + .1;

  ///position
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = 1.0f - .1;

  ///position
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = -1.0f + .1;




  mesh->Bind();

  auto vb_bind = make_bind_guard(*pVertexBuffer);
  pVertexBuffer->AllocateGpuMemory();
  pVertexBuffer->UpdateToGpu();

  mesh->vbs.push_back(pVertexBuffer);

  if (indexed)
  {
    mesh->ib = std::make_shared<IndexBuffer>(mesh, 6,PrimitiveIndexType::Indices32Bit, Usage::STATIC_DRAW);

    typedef uint32_t index_t;
    index_t* index = reinterpret_cast<index_t*>(mesh->ib->CpuPtr());

    *index++ = 0;
    *index++ = 1;
    *index++ = 2;

    *index++ = 3;
    *index++ = 4;
    *index++ = 5;

    mesh->ib->Bind();
    mesh->ib->AllocateGpuMemory();
    mesh->ib->UpdateToGpu();
  }


  mesh->sp = sp;

  ///Seal the VAO
  {
    auto mesh_bind = make_bind_guard(*mesh);

    mesh->GenerateVAO();
    mesh->LinkShaders();
  }

  ///Generate texture

  auto texture_unit = std::make_shared<TextureUnit>(0);
  auto sampler = std::make_shared<TextureSampler>();
  sampler->GenerateParams();
  std::string sampler_name = "texture1";

  auto mesh_texture = std::make_shared<MeshTexture>(gentesttexture(), texture_unit, sampler, sampler_name);
  mesh->textures.push_back(mesh_texture);
  


  Mesh::UnBindAll();
  return mesh;

}



std::shared_ptr<SGFXAPI::Mesh> SSTQRGBD2(bool indexed)
{

  using namespace SGFXAPI;
  Mesh::UnBindAll();

  auto mesh = std::make_shared<Mesh>(PrimitiveType::TriangleList);


  auto sp = DefaultWhiteShader();

  /////////////////////////////////
  /////////////////////////////////
  // SET UP MESH

  VertexDeclaration decl;
  
  decl.Add(VertexElement(VertexDataSemantic::VCOORD, 
                                 VertexDataType::FLOAT, 
                                 3,
                                 "vertex_position"));

  auto pVertexBuffer = std::make_shared<VertexBuffer>(mesh, 6, decl, Usage::STATIC_DRAW);

  float* vbPtr0 = reinterpret_cast<float*>(pVertexBuffer->CpuPtr());
  float* vbPtr = vbPtr0;
  uint32_t offset = 0;

  // FILL VERTEX DATA

  ///position
  *vbPtr++ = -1 + .1;
  *vbPtr++ = -1 + .1;
  *vbPtr++ = 0;


  ///position
  *vbPtr++ = -1.0f + .1;
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = 0;


  ///position
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = 0;


  ///position
  *vbPtr++ = -1 + .1;
  *vbPtr++ = -1 + .1;
  *vbPtr++ = 0;

  ///position
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = 0;

  ///position
  *vbPtr++ = 1.0f - .1;
  *vbPtr++ = -1.0f + .1;
  *vbPtr++ = 0;


  assert(reinterpret_cast<uint8_t*>(vbPtr) == pVertexBuffer->CpuPtr() + pVertexBuffer->LogicalBufferSizeBytes());


  mesh->Bind();

  auto vb_bind = make_bind_guard(*pVertexBuffer);
  pVertexBuffer->AllocateGpuMemory();
  pVertexBuffer->UpdateToGpu();

  mesh->vbs.push_back(pVertexBuffer);

  if (indexed)
  {
    mesh->ib = std::make_shared<IndexBuffer>(mesh, 6,PrimitiveIndexType::Indices32Bit, Usage::STATIC_DRAW);

    typedef uint32_t index_t;
    index_t* index = reinterpret_cast<index_t*>(mesh->ib->CpuPtr());

    *index++ = 0;
    *index++ = 1;
    *index++ = 2;

    *index++ = 3;
    *index++ = 4;
    *index++ = 5;

    mesh->ib->Bind();
    mesh->ib->AllocateGpuMemory();
    mesh->ib->UpdateToGpu();
  }


  mesh->sp = sp;

  ///Seal the VAO
  {
    auto mesh_bind = make_bind_guard(*mesh);

    mesh->GenerateVAO();
    mesh->LinkShaders();
  }


  Mesh::UnBindAll();
  return mesh;

}







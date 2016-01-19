

SGFXAPI Reference Documentation
===================================



+--------------------------------------------------+--------------------------------------------------+--------------------------------------------------+
|                                                  |                                                  |                                                  |
|**Texturing**                                     |**Shading**                                       |**Geometry**                                      |
|                                                  |                                                  |                                                  |
|* :cpp:class:`SGFXAPI::Texture`                   |* :cpp:class:`SGFXAPI::Shader`                    |* :cpp:class:`SGFXAPI::VertexBuffer`              |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|  Class represents a texture on the CPU and on    |  A shader, which is used to program the          |  Stores a CPU and/or GPU-side array of vertices. |
|  the GPU.                                        |  graphics pipeline.                              |                                                  |
|* :cpp:class:`SGFXAPI::TextureSampler`            |                                                  |                                                  |
|                                                  |* :cpp:class:`SGFXAPI::ShaderProgram`             |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::IndexBuffer`               |
|  Contains properties telling the GPU how to      |                                                  |                                                  |
|  sample the texture.                             |  A shader program, which bundles all of the      |  Stores a CPU and/or GPU side array of geometry  |
|                                                  |  necessary shaders to program the graphics a     |  index data.                                     |
|                                                  |  pipeline for a Mesh.                            |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|* :cpp:class:`SGFXAPI::MeshTexture`               |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::VertexDeclaration`         |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Specifies the structure of a vertex stream.     |
|  Container that bundles together the texture     |                                                  |                                                  |
|  texture unit, sampler, and texture name         |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|* :cpp:class:`SGFXAPI::PixelBuffer`               |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::VertexElement`             |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Specifies the semantic and format of a single   |
|  A stream of pixels that can be used to read and |                                                  |  element in a vertex data stream.                |
|  write pixel data between the CPU and GPU.       |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::Mesh`                      |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Bundles together VertexBuffer(s), an optional   |
|                                                  |                                                  |  IndexBuffer, a ShaderProgram, and Texture(s) for|
|                                                  |                                                  |  rendering.                                      |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
+--------------------------------------------------+--------------------------------------------------+--------------------------------------------------+

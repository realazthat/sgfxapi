

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
|  Class represents a texture on the CPU and on    |  A shader program, which is used to program the  |  Stores a CPU and/or GPU-side array of vertices. |
|  the GPU.                                        |  graphics pipeline.                              |                                                  |
|* :cpp:class:`SGFXAPI::TextureSampler`            |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::IndexBuffer`               |
|  Contains properties telling the GPU how to      |                                                  |                                                  |
|  sample the texture.                             |                                                  |  Stores a CPU and/or GPU side array of geometry  |
|                                                  |                                                  |  index data.                                     |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|* :cpp:class:`SGFXAPI::MeshTexture`               |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::VertexDeclaration`         |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Specifies the structure of a vertex stream.     |
|  Cntainer that bundles together the texture      |                                                  |                                                  |
|  texture unit, sampler, and texture name         |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::VertexElement`             |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Specifies the semantic and format of a single   |
|                                                  |                                                  |  element in a vertex data stream.                |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::Mesh`                      |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Something something                             |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
+--------------------------------------------------+--------------------------------------------------+--------------------------------------------------+

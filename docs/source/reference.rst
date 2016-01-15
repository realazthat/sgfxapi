

SGFXAPI Reference Documentation
===================================



+--------------------------------------------------+--------------------------------------------------+--------------------------------------------------+
|                                                  |                                                  |                                                  |
|**Texturing**                                     |**Shading**                                       |**Geometry**                                      |
|                                                  |                                                  |                                                  |
|* .. doxygenclass:: SGFXAPI::Texture              |* .. doxygenclass:: SGFXAPI::Shader               |* :cpp:class:`SGFXAPI::VertexBuffer`              |
|     :project: sgfxapi                            |     :project: sgfxapi                            |                                                  |
|     :outline:                                    |     :outline:                                    |                                                  |
|                                                  |                                                  |                                                  |
|  Class represents a texture on the CPU and on    |  A shader program, which is used to program the  |  Stores a CPU and/or GPU-side array of vertices. |
|  the GPU.                                        |  graphics pipeline.                              |                                                  |
|* .. doxygenstruct:: SGFXAPI::TextureSampler      |                                                  |                                                  |
|     :project: sgfxapi                            |                                                  |                                                  |
|     :outline:                                    |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::IndexBuffer`               |
|  Contains properties telling the GPU how to      |                                                  |                                                  |
|  sample the texture.                             |                                                  |  Stores a CPU and/or GPU side array of geometry  |
|                                                  |                                                  |  index data.                                     |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* :cpp:class:`SGFXAPI::VertexDeclaration`         |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Specifies the structure of a vertex stream.     |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
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

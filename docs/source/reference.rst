

SGFXAPI Reference Documentation
===================================



+--------------------------------------------------+--------------------------------------------------+--------------------------------------------------+
|                                                  |                                                  |                                                  |
|**Texturing**                                     |**Shading**                                       |**Geometry**                                      |
|                                                  |                                                  |                                                  |
|* .. doxygenclass:: SGFXAPI::Texture              |* .. doxygenclass:: SGFXAPI::Shader               |* .. doxygenclass:: SGFXAPI::VertexBuffer         |
|     :project: sgfxapi                            |     :project: sgfxapi                            |     :project: sgfxapi                            |
|     :outline:                                    |     :outline:                                    |     :outline:                                    |
|                                                  |                                                  |                                                  |
|  Class represents a texture on the CPU and on    |  A shader program, which is used to program the  |  Represents a vertex stream, the raw vertex data |
|  the GPU.                                        |  graphics pipeline.                              |  that gets sent to the GPU.                      |
|* .. doxygenstruct:: SGFXAPI::TextureSampler      |                                                  |                                                  |
|     :project: sgfxapi                            |                                                  |                                                  |
|     :outline:                                    |                                                  |                                                  |
|                                                  |                                                  |* .. doxygenclass:: SGFXAPI::IndexBuffer          |
|  Contains properties telling the GPU how to      |                                                  |     :project: sgfxapi                            |
|  sample the texture.                             |                                                  |     :outline:                                    |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  The raw buffer of triangle indices that gets    |
|                                                  |                                                  |  to the GPU.                                     |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* .. doxygenclass:: SGFXAPI::VertexDeclaration    |
|                                                  |                                                  |     :project: sgfxapi                            |
|                                                  |                                                  |     :outline:                                    |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Something something                             |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* :ref:`VertexElement`                            |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |  Something something                             |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |* .. doxygenclass:: SGFXAPI::Mesh                 |
|                                                  |                                                  |     :project: sgfxapi                            |
|                                                  |                                                  |     :outline:                                    |
|                                                  |                                                  |  Something something                             |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
|                                                  |                                                  |                                                  |
+--------------------------------------------------+--------------------------------------------------+--------------------------------------------------+

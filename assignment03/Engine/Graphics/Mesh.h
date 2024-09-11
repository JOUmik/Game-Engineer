#pragma once
#include "cVertexFormat.h"

#ifdef EAE6320_PLATFORM_D3D
#include "Engine/Graphics/Direct3D/Includes.h"
#elif EAE6320_PLATFORM_GL
#include "Engine/Graphics/OpenGL/Includes.h"
#endif // EAE6320_PLATFORM


namespace eae6320
{ 
    namespace Graphics 
    {
        class Mesh 
        {
        public:
            //varibles
            //--------
#ifdef EAE6320_PLATFORM_D3D
            cVertexFormat* s_vertexFormat = nullptr;

            // A vertex buffer holds the data for each vertex
            ID3D11Buffer* s_vertexBuffer = nullptr;
            // A index buffer holds the relationship of triangles and vertexes
            ID3D11Buffer* s_indexBuffer = nullptr;
#elif EAE6320_PLATFORM_GL
            // A vertex buffer holds the data for each vertex
            GLuint s_vertexBufferId = 0;
            // An index buffer holds the data for each index
            GLuint s_indexBufferId = 0;
            // A vertex array encapsulates the vertex data as well as the vertex input layout
            GLuint s_vertexArrayId = 0;
#endif

            //functions
            //---------
            Mesh();
            ~Mesh();
            eae6320::cResult InitializeGeometry();
            void Draw();
            cResult CleanUp();
        };
    }
}
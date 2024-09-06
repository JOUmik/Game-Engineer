#pragma once
#include "Graphics.h"
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
#elif EAE6320_PLATFORM_GL
            // A vertex buffer holds the data for each vertex
            GLuint s_vertexBufferId = 0;
            // A vertex array encapsulates the vertex data as well as the vertex input layout
            GLuint s_vertexArrayId = 0;
#endif

            //functions
            //---------
            Mesh();
            ~Mesh();
            eae6320::cResult InitializeGeometry();
#ifdef EAE6320_PLATFORM_D3D
            void Draw(ID3D11DeviceContext* const& direct3dImmediateContext);
            void CleanUp();
#elif EAE6320_PLATFORM_GL
            void Draw();
            void CleanUp(cResult& result);
#endif
        };
    }
}
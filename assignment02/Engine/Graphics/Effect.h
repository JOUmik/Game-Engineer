#pragma once
#ifdef EAE6320_PLATFORM_D3D
#include "Engine/Graphics/Direct3D/Includes.h"
#elif EAE6320_PLATFORM_GL
#include "Engine/Graphics/OpenGL/Includes.h"
#endif // EAE6320_PLATFORM

#include "cRenderState.h"

namespace eae6320
{
    namespace Graphics
    {
        class cShader;
        class Effect {
        public:
            //varibles
            //--------
            cShader* s_vertexShader = nullptr;
            cShader* s_fragmentShader = nullptr;

            cRenderState s_renderState;

#ifdef EAE6320_PLATFORM_GL
            GLuint s_programId = 0;
#endif // EAE6320_PLATFORM_GL

            //functions
            //---------
#ifdef EAE6320_PLATFORM_D3D
            void BindShadingData(ID3D11DeviceContext* const& direct3dImmediateContext);
            void CleanUp();
#elif EAE6320_PLATFORM_GL
            void BindShadingData();
            void CleanUp(cResult& result);
#endif
            eae6320::cResult InitializeShadingData();
        };
    }
}
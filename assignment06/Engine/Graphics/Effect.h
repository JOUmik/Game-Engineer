#pragma once
#ifdef EAE6320_PLATFORM_D3D
#include "Engine/Graphics/Direct3D/Includes.h"
#elif EAE6320_PLATFORM_GL
#include "Engine/Graphics/OpenGL/Includes.h"
#endif // EAE6320_PLATFORM


#include "cRenderState.h"

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <string>

namespace eae6320
{
    namespace Graphics
    {
        struct sInitializationParameters;
        class cShader;
        class Effect 
        {
        public:
            //factory mode
            static cResult Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, Effect*& o_effect);

            EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS( Effect );

            // Reference Counting
            //-------------------
            EAE6320_ASSETS_DECLAREREFERENCECOUNT();
            EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

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
            void BindShadingData();

        private:
            Effect();
            ~Effect();

            cResult CleanUp();
            cResult InitializeShadingData(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        };
    }
}
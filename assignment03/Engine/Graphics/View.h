#pragma once
#ifdef EAE6320_PLATFORM_D3D
#include "Engine/Graphics/Direct3D/Includes.h"
#elif EAE6320_PLATFORM_GL
#include "Engine/Graphics/OpenGL/Includes.h"
#endif // EAE6320_PLATFORM

namespace eae6320
{
    class cResult;
    namespace Graphics
    {
        struct sInitializationParameters;
        class View 
        {
        public:
            //varibles
            //--------
#ifdef EAE6320_PLATFORM_D3D
            // In Direct3D "views" are objects that allow a texture to be used a particular way:
            // A render target view allows a texture to have color rendered to it
            ID3D11RenderTargetView* s_renderTargetView = nullptr;
            // A depth/stencil view allows a texture to have depth rendered to it
            ID3D11DepthStencilView* s_depthStencilView = nullptr;
#endif

            //functions
            //---------
            View() = default;
            ~View() = default;

            void ClearPreviousImage(float g_elapsedSecondCount_simulationTime);
            cResult InitializeViews(const sInitializationParameters& i_initializationParameters);
            void SwapImageToFrontBuffer();
            void CleanUp();
        };
    }
}
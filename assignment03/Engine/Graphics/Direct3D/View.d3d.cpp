#include "../View.h"
#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>

namespace eae6320
{
    namespace Graphics
    {
        void View::ClearPreviousImage(float g_elapsedSecondCount_simulationTime)
        {
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);

			// Every frame an entirely new image will be created.
			// Before drawing anything, then, the previous image will be erased
			// by "clearing" the image buffer (filling it with a solid color)
			{
				EAE6320_ASSERT(s_renderTargetView);

				// Black is usually used
				float r = (std::cos(9.0f * g_elapsedSecondCount_simulationTime) * 0.1f) + 0.15f;
				float g = (std::sin(2.0f * g_elapsedSecondCount_simulationTime) * 0.1f) + 0.15f;
				float b = (-std::cos(5.0f * g_elapsedSecondCount_simulationTime) * 0.1f) + 0.15f;
				const float clearColor[4] = { r, g, b, 1.0f };
				direct3dImmediateContext->ClearRenderTargetView(s_renderTargetView, clearColor);
			}
			// In addition to the color buffer there is also a hidden image called the "depth buffer"
			// which is used to make it less important which order draw calls are made.
			// It must also be "cleared" every frame just like the visible color buffer.
			{
				EAE6320_ASSERT(s_depthStencilView);

				constexpr float clearToFarDepth = 1.0f;
				constexpr uint8_t stencilValue = 0;	// Arbitrary if stencil isn't used
				direct3dImmediateContext->ClearDepthStencilView(s_depthStencilView, D3D11_CLEAR_DEPTH, clearToFarDepth, stencilValue);
			}
        }

		void View::SwapImageToFrontBuffer()
		{
			// Everything has been drawn to the "back buffer", which is just an image in memory.
			// In order to display it the contents of the back buffer must be "presented"
			// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
			auto* const swapChain = sContext::g_context.swapChain;
			EAE6320_ASSERT(swapChain);
			constexpr unsigned int swapImmediately = 0;
			constexpr unsigned int presentNextFrame = 0;
			const auto result = swapChain->Present(swapImmediately, presentNextFrame);
			EAE6320_ASSERT(SUCCEEDED(result));
		}

		void View::CleanUp() 
		{
			if (s_renderTargetView)
			{
				s_renderTargetView->Release();
				s_renderTargetView = nullptr;
			}
			if (s_depthStencilView)
			{
				s_depthStencilView->Release();
				s_depthStencilView = nullptr;
			}
		}
    }
}
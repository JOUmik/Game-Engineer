#include "../View.h"
#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>

namespace eae6320
{
    namespace Graphics
    {
        void View::ClearPreviousImage()
        {
			// Every frame an entirely new image will be created.
			// Before drawing anything, then, the previous image will be erased
			// by "clearing" the image buffer (filling it with a solid color)
			{
				// Black is usually used
				{
					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
				{
					constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
					glClear(clearColor);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
			}
			// In addition to the color buffer there is also a hidden image called the "depth buffer"
			// which is used to make it less important which order draw calls are made.
			// It must also be "cleared" every frame just like the visible color buffer.
			{
				{
					glDepthMask(GL_TRUE);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
					constexpr GLclampd clearToFarDepth = 1.0;
					glClearDepth(clearToFarDepth);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
				{
					constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
					glClear(clearDepth);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
			}
        }

		void View::SwapImageToFrontBuffer()
		{
			// Everything has been drawn to the "back buffer", which is just an image in memory.
			// In order to display it the contents of the back buffer must be "presented"
			// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
			const auto deviceContext = sContext::g_context.deviceContext;
			EAE6320_ASSERT(deviceContext != NULL);
			const auto glResult = SwapBuffers(deviceContext);
			EAE6320_ASSERT(glResult != FALSE);
		}

		void View::CleanUp() {}
    }
}
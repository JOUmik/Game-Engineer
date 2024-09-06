#include "../Effect.h"
#include "../cShader.h"
#include "../sContext.h"

namespace eae6320
{
    namespace Graphics
    {
        void Effect::BindShadingData()
        {
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);

			{
				constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
				constexpr unsigned int interfaceCount = 0;
				// Vertex shader
				{
					EAE6320_ASSERT((s_vertexShader != nullptr) && (s_vertexShader->m_shaderObject.vertex != nullptr));
					direct3dImmediateContext->VSSetShader(s_vertexShader->m_shaderObject.vertex, noInterfaces, interfaceCount);
				}
				// Fragment shader
				{
					EAE6320_ASSERT((s_fragmentShader != nullptr) && (s_fragmentShader->m_shaderObject.vertex != nullptr));
					direct3dImmediateContext->PSSetShader(s_fragmentShader->m_shaderObject.fragment, noInterfaces, interfaceCount);
				}
			}
			// Render state
			{
				s_renderState.Bind();
			}
        }

		cResult Effect::CleanUp() 
		{
			if (s_vertexShader)
			{
				s_vertexShader->DecrementReferenceCount();
				s_vertexShader = nullptr;
			}
			if (s_fragmentShader)
			{
				s_fragmentShader->DecrementReferenceCount();
				s_fragmentShader = nullptr;
			}

			return Results::Success;
		}
    }
}
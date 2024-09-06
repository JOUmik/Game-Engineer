#include "../Effect.h"
#include "../cShader.h"

#include <Engine/Logging/Logging.h>

namespace eae6320
{
    namespace Graphics
    {
		void Effect::BindShadingData() 
		{
			{
				EAE6320_ASSERT(s_programId != 0);
				glUseProgram(s_programId);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
			// Render state
			{
				s_renderState.Bind();
			}
		}

		cResult Effect::CleanUp()
		{
			auto result = Results::Success;

			if (s_programId != 0)
			{
				glDeleteProgram(s_programId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the program: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				s_programId = 0;
			}

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

			return result;
		}
    }
}

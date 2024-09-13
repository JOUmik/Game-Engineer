#include "../Mesh.h"
#include "../VertexFormats.h"
#include "../cVertexFormat.h"
#include "../sContext.h"

#include <Engine/Logging/Logging.h>

namespace eae6320 
{
    namespace Graphics 
    {
        Mesh::Mesh() {}
        Mesh::~Mesh() {}

        eae6320::cResult Mesh::InitializeGeometry(VertexFormats::sVertex_mesh* vertexData, uint16_t* indexData, unsigned int vertexCount, unsigned int indexCount) {
			auto result = eae6320::Results::Success;

			s_vertexCount = vertexCount;
			s_indexCount = indexCount;
			
			// Create a vertex array object and make it active
			{
				constexpr GLsizei arrayCount = 1;
				glGenVertexArrays(arrayCount, &s_vertexArrayId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindVertexArray(s_vertexArrayId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Create a vertex buffer object and make it active
			{
				constexpr GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &s_vertexBufferId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ARRAY_BUFFER, s_vertexBufferId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Assign the data to the buffer
			{	
				EnsureRightHandedIndexOrder(vertexData, indexData, indexCount);
				const auto bufferSize = sizeof(VertexFormats::sVertex_mesh) * vertexCount;
				EAE6320_ASSERT(static_cast<GLsizeiptr>(bufferSize) <= std::numeric_limits<GLsizeiptr>::max());
				glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(vertexData),
					// In our class we won't ever read from the buffer
					GL_STATIC_DRAW);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Create an index buffer object and make it active
			{
				constexpr GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &s_indexBufferId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indexBufferId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to bind a new index buffer: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to get an unused index buffer ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Assign the data to the buffer
			{
				const auto bufferSize = sizeof(uint16_t) * indexCount;
				EAE6320_ASSERT(static_cast<GLsizeiptr>(bufferSize) <= std::numeric_limits<GLsizeiptr>::max());
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(indexData),
					// In our class we won't ever read from the buffer
					GL_STATIC_DRAW);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Initialize vertex format
			{
				// The "stride" defines how large a single vertex is in the stream of data
				// (or, said another way, how far apart each position element is)
				constexpr auto stride = static_cast<GLsizei>(sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh));

				// Position (0)
				// 3 floats == 12 bytes
				// Offset = 0
				{
					constexpr GLuint vertexElementLocation = 0;
					constexpr GLint elementCount = 3;
					constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
					glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
						reinterpret_cast<GLvoid*>(offsetof(Graphics::VertexFormats::sVertex_mesh, x)));
					const auto errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						glEnableVertexAttribArray(vertexElementLocation);
						const GLenum errorCode = glGetError();
						if (errorCode != GL_NO_ERROR)
						{
							result = Results::Failure;
							EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
								vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							return result;
						}
					}
					else
					{
						result = Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
							vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
			}

			return result;
        }

		void Mesh::Draw() 
		{
			// Bind a specific vertex buffer to the device as a data source
			{
				EAE6320_ASSERT(s_vertexArrayId != 0);
				glBindVertexArray(s_vertexArrayId);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
			// Render triangles from the index buffer
			{
				// The mode defines how to interpret multiple vertices as a single "primitive";
				// a triangle list is defined
				// (meaning that every primitive is a triangle and will be defined by three vertices)
				constexpr GLenum mode = GL_TRIANGLES;
				// It's possible to start rendering primitives in the middle of the stream
				const GLvoid* const offset = 0;
				glDrawElements(mode, static_cast<GLsizei>(s_indexCount), GL_UNSIGNED_SHORT, offset);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
		}

		cResult Mesh::CleanUp()
		{
			auto result = Results::Success;

			if (s_vertexArrayId != 0)
			{
				// Make sure that the vertex array isn't bound
				{
					// Unbind the vertex array
					glBindVertexArray(0);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						if (result)
						{
							result = Results::Failure;
						}
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
					}
				}
				constexpr GLsizei arrayCount = 1;
				glDeleteVertexArrays(arrayCount, &s_vertexArrayId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex array: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				s_vertexArrayId = 0;
			}
			if (s_vertexBufferId != 0)
			{
				constexpr GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &s_vertexBufferId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				s_vertexBufferId = 0;
			}
			if (s_indexBufferId != 0)
			{
				constexpr GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &s_indexBufferId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the index buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				s_indexBufferId = 0;
			}

			return result;
		}
    }
}
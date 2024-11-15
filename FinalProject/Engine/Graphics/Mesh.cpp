#include "Mesh.h"
#include "VertexFormats.h"
#include "cVertexFormat.h"
#include "sContext.h"

#include <fstream>
#include <Engine/Logging/Logging.h>
#include <Engine/Math/sVector.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Concurrency/cMutex.h>
#include <External/Lua/Includes.h>
#include <new>
#include <vector>
#include <chrono>

namespace
{
	// This exists for our class to help you catch reference counting problems.
	// In a real game there would be more sophisticated methods that would work for all asset types.
	class cMeshTracker
	{
		// Interface
		//----------

	public:

		// Access
		void AddMesh(const eae6320::Graphics::Mesh* const i_mesh);
		void RemoveMesh(const eae6320::Graphics::Mesh* const i_mesh);
		// Initializate / Clean Up
		cMeshTracker() = default;
		~cMeshTracker();

		// Data
		//-----

	private:

		std::vector<const eae6320::Graphics::Mesh*> m_meshes;
		eae6320::Concurrency::cMutex m_mutex;

		// Implementation
		//---------------

	private:

		// Initializate / Clean Up
		cMeshTracker(const cMeshTracker&) = delete;
		cMeshTracker(cMeshTracker&&) = delete;
		cMeshTracker& operator =(const cMeshTracker&) = delete;
		cMeshTracker& operator =(cMeshTracker&&) = delete;
	} s_meshTracker;
}

namespace eae6320
{
    namespace Graphics
    {
        cResult Mesh::Load(VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int vertexCount, unsigned int indexCount, Mesh*& o_mesh)
        {
			auto result = Results::Success;

			Mesh* newMesh = nullptr;
			cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
				{
					if (result)
					{
						EAE6320_ASSERT(newMesh != nullptr);
						o_mesh = newMesh;
					}
					else
					{
						if (newMesh)
						{
							newMesh->DecrementReferenceCount();
							newMesh = nullptr;
						}
						o_mesh = nullptr;
					}
				});

			// Allocate a new effect
			{
				newMesh = new (std::nothrow) Mesh();
				if (!newMesh)
				{
					result = Results::OutOfMemory;
					EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
					Logging::OutputError("Failed to allocate memory for the mesh");
					return result;
				}
			}
			// Initialize the platform-specific graphics API shader object
			if (!(result = newMesh->InitializeGeometry(vertexData, indexData, vertexCount, indexCount)))
			{
				EAE6320_ASSERTF(false, "Initialization of new mesh failed");
				return result;
			}

			return result;
        }

		cResult Mesh::LoadReadableLuaFile(const std::string& meshPath, Mesh*& o_mesh)
		{
			auto result = eae6320::Results::Success;

			// Create a new Lua state
			lua_State* luaState = nullptr;
			eae6320::cScopeGuard scopeGuard_onExit([&luaState]
				{
					if (luaState)
					{
						// If I haven't made any mistakes
						// there shouldn't be anything on the stack
						// regardless of any errors
						EAE6320_ASSERT(lua_gettop(luaState) == 0);

						lua_close(luaState);
						luaState = nullptr;
					}
				});
			{
				luaState = luaL_newstate();
				if (!luaState)
				{
					result = eae6320::Results::OutOfMemory;
					eae6320::Logging::OutputError("Create mesh failed! Failed to create a new Lua state, mesh path: %s", meshPath.c_str());
					return result;
				}
			}

			// Load the asset file as a "chunk",
			// meaning there will be a callable function at the top of the stack
			const auto stackTopBeforeLoad = lua_gettop(luaState);
			{
				const auto luaResult = luaL_loadfile(luaState, meshPath.c_str());
				if (luaResult != LUA_OK)
				{
					result = eae6320::Results::Failure;
					eae6320::Logging::OutputError("Create mesh failed! %s", lua_tostring(luaState, -1));
					// Pop the error message
					lua_pop(luaState, 1);
					return result;
				}
			}
			// Execute the "chunk", which should load the asset
			// into a table at the top of the stack
			{
				constexpr int argumentCount = 0;
				constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
				constexpr int noMessageHandler = 0;
				const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
				if (luaResult == LUA_OK)
				{
					// A well-behaved asset file will only return a single value
					const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
					if (returnedValueCount == 1)
					{
						// A correct asset file _must_ return a table
						if (!lua_istable(luaState, -1))
						{
							result = eae6320::Results::InvalidFile;
							eae6320::Logging::OutputError("Create mesh failed! Asset files must return a table (instead of a %s)", luaL_typename(luaState, -1));
							// Pop the returned non-table value
							lua_pop(luaState, 1);
							return result;
						}
					}
					else
					{
						result = eae6320::Results::InvalidFile;
						eae6320::Logging::OutputError("Create mesh failed! Asset files must return a single table (instead of a %s values)", returnedValueCount);
						// Pop every value that was returned
						lua_pop(luaState, returnedValueCount);
						return result;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					eae6320::Logging::OutputError("Create mesh failed! %s", lua_tostring(luaState, -1));
					// Pop the error message
					lua_pop(luaState, 1);
					return result;
				}
			}

			// If this code is reached the asset file was loaded successfully,
			// and its table is now at index -1
			eae6320::cScopeGuard scopeGuard_popAssetTable([luaState]
				{
					lua_pop(luaState, 1);
				});

			Graphics::VertexFormats::sVertex_mesh* vertexData = nullptr;
			uint16_t* indexData = nullptr;
			unsigned int vertexCount = 0;
			unsigned int indexCount = 0;

			if (result = LoadTableValues(*luaState, vertexData, indexData, vertexCount, indexCount))
			{
				result = Load(vertexData, indexData, vertexCount, indexCount, o_mesh);
			}
			if (vertexData != nullptr) 
			{
				delete[] vertexData;
			}
			if (indexData != nullptr) 
			{
				delete[] indexData;
			}

			return result;
		}

		cResult Mesh::LoadBinaryFile(const std::string& meshPath, Mesh*& o_mesh)
		{
			auto result = Results::Success;

			auto tick_before = std::chrono::high_resolution_clock::now();

			std::ifstream infile(meshPath, std::ifstream::binary);

			// get size of file
			infile.seekg(0, infile.end);
			long size = static_cast<long>(infile.tellg());
			infile.seekg(0);

			// allocate memory for file content
			char* buffer = new char[size];

			// read content of infile
			infile.read(buffer, size);

			auto currentOffset = reinterpret_cast<uintptr_t>(buffer);
			const auto finalOffset = currentOffset + size;

			uint16_t vertexCount = *reinterpret_cast<uint16_t*>(buffer);
			currentOffset += sizeof(vertexCount);
			Graphics::VertexFormats::sVertex_mesh* vertexData = reinterpret_cast<Graphics::VertexFormats::sVertex_mesh*>(currentOffset);
			currentOffset += sizeof(Graphics::VertexFormats::sVertex_mesh) * vertexCount;
			uint16_t indexCount = *reinterpret_cast<uint16_t*>(currentOffset);
			currentOffset += sizeof(indexCount);
			uint16_t* indexData = reinterpret_cast<uint16_t*>(currentOffset);

			auto tick_after = std::chrono::high_resolution_clock::now();
			auto second = std::chrono::duration_cast<std::chrono::microseconds>(tick_after - tick_before);

			Logging::OutputMessage("Time used to load binary file:\n %s\n %d (ms)", meshPath.c_str(), second.count());

			result = Load(vertexData, indexData, vertexCount, indexCount, o_mesh);
				
			infile.close();
			delete[] buffer;

			return result;
		}

		eae6320::cResult Mesh::LoadTableValues(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int& vertexCount, unsigned int& indexCount)
		{
			auto result = eae6320::Results::Success;

			if (!(result = LoadTableValues_Vertex(io_luaState, vertexData, vertexCount)))
			{
				return result;
			}
			if (!(result = LoadTableValues_Index(io_luaState, indexData, indexCount)))
			{
				return result;
			}

			return result;
		}

		eae6320::cResult Mesh::LoadTableValues_Vertex(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& vertexData, unsigned int& vertexCount)
		{
			auto result = eae6320::Results::Success;
			constexpr auto* const key = "vertex";

			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			eae6320::cScopeGuard scopeGuard_popTextures([&io_luaState]
				{
					lua_pop(&io_luaState, 1);
				});

			if (lua_istable(&io_luaState, -1))
			{
				vertexCount = (unsigned int)luaL_len(&io_luaState, -1);
				vertexData = new Graphics::VertexFormats::sVertex_mesh[vertexCount];

				for (unsigned int i = 1; i <= vertexCount; ++i)
				{
					lua_pushinteger(&io_luaState, i);
					lua_gettable(&io_luaState, -2);

					eae6320::cScopeGuard scopeGuard_popTexturePath([&io_luaState]
						{
							lua_pop(&io_luaState, 1);
						});

					lua_pushinteger(&io_luaState, 1);
					lua_gettable(&io_luaState, -2);
					vertexData[i - 1].x = (float)lua_tonumber(&io_luaState, -1);
					lua_pop(&io_luaState, 1);

					lua_pushinteger(&io_luaState, 2);
					lua_gettable(&io_luaState, -2);
					vertexData[i - 1].y = (float)lua_tonumber(&io_luaState, -1);
					lua_pop(&io_luaState, 1);

					lua_pushinteger(&io_luaState, 3);
					lua_gettable(&io_luaState, -2);
					vertexData[i - 1].z = (float)lua_tonumber(&io_luaState, -1);
					lua_pop(&io_luaState, 1);
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				eae6320::Logging::OutputError("Create mesh failed! The value at vertex must be a table (instead of a %s)", luaL_typename(&io_luaState, -1));
				return result;
			}

			return result;
		}

		eae6320::cResult Mesh::LoadTableValues_Index(lua_State& io_luaState, uint16_t*& indexData, unsigned int& indexCount)
		{
			auto result = eae6320::Results::Success;
			constexpr auto* const key = "index";

			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

			eae6320::cScopeGuard scopeGuard_popTextures([&io_luaState]
				{
					lua_pop(&io_luaState, 1);
				});

			if (lua_istable(&io_luaState, -1))
			{
				indexCount = (unsigned int)luaL_len(&io_luaState, -1);
				indexData = new uint16_t[indexCount];

				for (unsigned int i = 1; i <= indexCount; ++i)
				{
					lua_pushinteger(&io_luaState, i);
					lua_gettable(&io_luaState, -2);

					eae6320::cScopeGuard scopeGuard_popTexturePath([&io_luaState]
						{
							lua_pop(&io_luaState, 1);
						});

					indexData[i - 1] = (uint16_t)lua_tonumber(&io_luaState, -1);
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				eae6320::Logging::OutputError("Create mesh failed! The value at index must be a table (instead of a %s)", luaL_typename(&io_luaState, -1));
				return result;
			}

			return result;
		}

        void Mesh::EnsureRightHandedIndexOrder(const VertexFormats::sVertex_mesh* vertexData, uint16_t* indexData, unsigned int indexCount)
        {
            // Iterate through each triangle (3 indices per triangle)
            for (unsigned int i = 0; i < indexCount; i += 3)
            {
                // Get the three vertices for the current triangle
                const VertexFormats::sVertex_mesh& v0 = vertexData[indexData[i]];
                const VertexFormats::sVertex_mesh& v1 = vertexData[indexData[i + 1]];
                const VertexFormats::sVertex_mesh& v2 = vertexData[indexData[i + 2]];

                // Compute two edge vectors of the triangle
                Math::sVector edge1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
                Math::sVector edge2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

                // Compute the normal using cross product
                Math::sVector normal = Math::Cross(edge1, edge2);

                // Check if the normal is pointing in the positive or negative Z direction
                if (normal.z < 0.0f)
                {
                    // The normal is pointing in the negative Z direction (indicating a left-handed winding order)
                    // Swap the second and third indices to correct the order (make it right-handed)
                    std::swap(indexData[i + 1], indexData[i + 2]);
                }
            }
        }

		Mesh::Mesh() 
		{
			s_meshTracker.AddMesh(this);
		}

		Mesh::~Mesh() 
		{
			EAE6320_ASSERT(m_referenceCount == 0);
			s_meshTracker.RemoveMesh(this);
			const auto result = CleanUp();
			EAE6320_ASSERT(result);
		}
    }
}

namespace
{
	// Interface
	//----------

	// Access

	void cMeshTracker::AddMesh(const eae6320::Graphics::Mesh* const i_mesh)
	{
		EAE6320_ASSERT(i_mesh != nullptr);
		eae6320::Concurrency::cMutex::cScopeLock scopeLock(m_mutex);
#ifdef EAE6320_ASSERTS_AREENABLED
		for (const auto* const mesh : m_meshes)
		{
			EAE6320_ASSERTF(mesh != i_mesh, "This mesh is already being tracked");
		}
#endif
		m_meshes.push_back(i_mesh);
	}

	void cMeshTracker::RemoveMesh(const eae6320::Graphics::Mesh* const i_mesh)
	{
		eae6320::Concurrency::cMutex::cScopeLock scopeLock(m_mutex);
		const auto meshCount = m_meshes.size();
		for (std::remove_const<decltype(meshCount)>::type i = 0; i < meshCount; ++i)
		{
			if (m_meshes[i] == i_mesh)
			{
				m_meshes.erase(m_meshes.begin() + i);
				return;
			}
		}
		EAE6320_ASSERTF(false, "This mesh wasn't being tracked");
	}

	// Initialize / Clean Up

	cMeshTracker::~cMeshTracker()
	{
		eae6320::Concurrency::cMutex::cScopeLock scopeLock(m_mutex);
		const auto haveAnyMeshesNotBeenDeleted = !m_meshes.empty();
		if (haveAnyMeshesNotBeenDeleted)
		{
			EAE6320_ASSERTF(!haveAnyMeshesNotBeenDeleted,
				"If you are seeing this message there are %i meshes that haven't been deleted. You probably have a reference counting bug!", m_meshes.size());
			eae6320::Logging::OutputError("Not every mesh was released. The program will now crash!");
			constexpr int* const invalidAddress = 0x0;
			*invalidAddress = 123;	// Intentional crash! You have a bug that must be fixed!
		}
	}
}
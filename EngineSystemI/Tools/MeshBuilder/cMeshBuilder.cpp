
#include "cMeshBuilder.h"

#include <iostream>
#include <fstream>
#include <Engine/Platform/Platform.h>
#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/ScopeGuard/cScopeGuard.h>

namespace eae6320
{
	namespace Assets
	{
		cResult cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
		{
			std::string errorMessage;
			if (eae6320::Platform::DoesFileExist(m_path_source, &errorMessage)) 
			{
				Graphics::VertexFormats::sVertex_mesh* vertexData = nullptr;
				uint16_t* indexData = nullptr;
				unsigned int vertexCount = 0;
				unsigned int indexCount = 0;

				if (Load(m_path_source, vertexData, indexData, vertexCount, indexCount)) 
				{
					WriteToBinary(m_path_target, vertexData, indexData, vertexCount, indexCount);

					if (vertexData != nullptr)
					{
						delete[] vertexData;
					}
					if (indexData != nullptr)
					{
						delete[] indexData;
					}
				}
				else
				{
					OutputErrorMessageWithFileInfo(__FILE__, __LINE__, errorMessage.c_str());
					return Results::Failure;
				}
			}
			else 
			{
				OutputErrorMessageWithFileInfo(__FILE__, __LINE__, errorMessage.c_str());
				return Results::Failure;
			}
			return Results::Success;
		}


		cResult cMeshBuilder::Load(const std::string& meshPath, Graphics::VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int& vertexCount, unsigned int& indexCount)
		{
			auto result = eae6320::Results::Success;

			// Create a new Lua state
			lua_State* luaState = nullptr;
			eae6320::cScopeGuard scopeGuard_onExit([&luaState, &meshPath]
				{
					if (luaState)
					{
						// If I haven't made any mistakes
						// there shouldn't be anything on the stack
						// regardless of any errors
						if (!(lua_gettop(luaState) == 0)) 
						{
							OutputErrorMessageWithFileInfo(meshPath.c_str(), "There should be nothing on the stack");
						}

						lua_close(luaState);
						luaState = nullptr;
					}
				});
			{
				luaState = luaL_newstate();
				if (!luaState)
				{
					result = eae6320::Results::OutOfMemory;
					OutputErrorMessageWithFileInfo(meshPath.c_str(), "Failed to create a new Lua state");
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
					OutputErrorMessageWithFileInfo(meshPath.c_str(), "Failed to load file");
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
							OutputErrorMessageWithFileInfo(meshPath.c_str(), "Asset files must return a table");
							// Pop the returned non-table value
							lua_pop(luaState, 1);
							return result;
						}
					}
					else
					{
						result = eae6320::Results::InvalidFile;
						OutputErrorMessageWithFileInfo(meshPath.c_str(), "Asset files must return a single table");
						// Pop every value that was returned
						lua_pop(luaState, returnedValueCount);
						return result;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					OutputErrorMessageWithFileInfo(meshPath.c_str(), lua_tostring(luaState, -1));
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

			return LoadTableValues(*luaState, vertexData, indexData, vertexCount, indexCount);
		}

		cResult cMeshBuilder::LoadTableValues(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int& vertexCount, unsigned int& indexCount)
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

		cResult cMeshBuilder::LoadTableValues_Vertex(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& vertexData, unsigned int& vertexCount)
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
				OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "The value at vertex must be a table");
				return result;
			}

			return result;
		}

		cResult cMeshBuilder::LoadTableValues_Index(lua_State& io_luaState, uint16_t*& indexData, unsigned int& indexCount)
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
				OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "The value at index must be a table"); 
				return result;
			}

			return result;
		}

		void cMeshBuilder::WriteToBinary(const std::string& targetPath, Graphics::VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int& vertexCount, unsigned int& indexCount)
		{
			size_t vertexBufferSize = sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh) * vertexCount;
			size_t indexBufferSize = sizeof(uint16_t) * indexCount;

			std::ofstream outfile(targetPath, std::ofstream::binary);
			outfile.write((const char*)&vertexCount, sizeof(uint16_t));
			outfile.write((const char*)vertexData, vertexBufferSize);
			outfile.write((const char*)&indexCount, sizeof(uint16_t));
			outfile.write((const char*)indexData, indexBufferSize);
			outfile.close();
		}
	}
}
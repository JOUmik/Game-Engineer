#pragma once
/*
	This class builds meshes
*/

#ifndef EAE6320_CSHADERBUILDER_H
#define EAE6320_CSHADERBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Graphics/Configuration.h>
#include <External/Lua/Includes.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cMeshBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build(const std::vector<std::string>& i_arguments) final;

			cResult Load(const std::string& meshPath, Graphics::VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int& vertexCount, unsigned int& indexCount);

			cResult LoadTableValues(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int& vertexCount, unsigned int& indexCount);

			cResult LoadTableValues_Vertex(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& vertexData, unsigned int& vertexCount);

			cResult LoadTableValues_Index(lua_State& io_luaState, uint16_t*& indexData, unsigned int& indexCount);

			void WriteToBinary(const std::string& targetPath, Graphics::VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int& vertexCount, unsigned int& indexCount);
			// Implementation
			//===============
		};
	}
}

#endif	// EAE6320_CSHADERBUILDER_H

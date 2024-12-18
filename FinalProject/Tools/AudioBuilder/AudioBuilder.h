#pragma once
#ifndef EAE6320_CAUDIOBUILDER_H
#define EAE6320_CAUDIOBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>

#include <Engine/Graphics/Configuration.h>
#include <Engine/Platform/Platform.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class AudioBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build(const std::vector<std::string>& i_arguments) final;
		};
	}
}

#endif	// EAE6320_CAUDIOBUILDER_H
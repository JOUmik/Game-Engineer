// Includes
//=========

#include "AudioBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
//#include <Engine/Asserts/Asserts.h>
#include <Engine/Windows/Functions.h>

// Build
//------

eae6320::cResult eae6320::Assets::AudioBuilder::Build(const std::vector<std::string>& i_arguments)
{
	eae6320::cResult result = Results::Success;


	if (!(result = Windows::CopyFileW(m_path_source, m_path_target, false, true)))
	{
		OutputErrorMessageWithFileInfo(m_path_source, "Fail to copy audio file!");
	}

	return result;
}

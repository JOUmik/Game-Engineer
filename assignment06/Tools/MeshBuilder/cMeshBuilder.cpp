// MeshBuilder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "cMeshBuilder.h"
#include <Engine/Platform/Platform.h>
#include <Tools/AssetBuildLibrary/Functions.h>

namespace eae6320
{
	namespace Assets
	{
		cResult cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
		{
			std::string errorMessage;
			if (eae6320::Platform::DoesFileExist(m_path_source, &errorMessage)) 
			{
				if (!eae6320::Platform::CopyFile(m_path_source, m_path_target, false, true, &errorMessage)) 
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
	}
}
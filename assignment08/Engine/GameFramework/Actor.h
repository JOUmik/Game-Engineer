#pragma once

#include "Object.h"

#include<Engine/Graphics/cRenderableObject.h>

namespace eae6320
{
	namespace GameFramework
	{
		class AActor : public UObject, public eae6320::Graphics::cRenderableObject
		{
		public:
			AActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect);
			~AActor();

			virtual void CleanUp();
		};
	}
}
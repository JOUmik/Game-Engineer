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

			void SetPosition(Math::sVector i_Position);
			Math::sVector GetPosition();

			virtual void Begin() {}
			virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate) {}

			void Draw();
			void Draw(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

			virtual void CleanUp();

			bool bIsDestroyed = false;
		};
	}
}
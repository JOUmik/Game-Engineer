#pragma once
#include "Actor.h"

namespace eae6320
{
	namespace GameFramework
	{
		class ACharacter : public AActor
		{
		public:
			ACharacter(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : AActor(i_mesh, i_effect) {}
			~ACharacter() {};

			virtual void Begin() {}
			virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate) {}

			virtual void CleanUp() {}

		private:
		};
	}
}
#pragma once
#include "Mesh.h"
#include "Effect.h"

#include <Engine/Physics/sRigidBodyState.h>


namespace eae6320
{
	namespace Graphics
	{
		class cRenderableObject 
		{
		public:
			eae6320::Physics::sRigidBodyState* rigidBodyState = nullptr;

			void CleanUp();

			void ChangeMesh(Mesh* new_mesh);
			void ChangeEffect(Effect* new_effect);

			//Draw the mesh with current orientation and position
			void SubmitMeshWithEffectToDraw();

			//Draw the mesh with predicted orientation and position
			void SubmitMeshWithEffectToDraw(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		protected:
			inline void SetMesh(Mesh* i_mesh) 
			{
				mesh = i_mesh; 
				mesh->IncrementReferenceCount();
			}
			inline void SetEffect(Effect* i_effect) 
			{ 
				effect = i_effect; 
				effect->IncrementReferenceCount();
			}

		private:
			Mesh* mesh = nullptr;
			Effect* effect = nullptr;
		};

	}
}
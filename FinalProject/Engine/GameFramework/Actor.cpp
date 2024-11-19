#include "Actor.h"

namespace eae6320
{
	namespace GameFramework
	{
		AActor::AActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : UObject(){
			if (!i_mesh) 
			{
				EAE6320_ASSERTF(false, "Can't initialize actor because i_mesh is nullptr");
			}
			if (!i_effect) 
			{
				EAE6320_ASSERTF(false, "Can't initialize actor because i_effect is nullptr");
			}
			SetMesh(i_mesh);
			SetEffect(i_effect);
			rigidBodyState = new eae6320::Physics::sRigidBodyState();
		}

		void AActor::SetPosition(Math::sVector i_Position) 
		{
			rigidBodyState->position = i_Position;
		}

		Math::sVector AActor::GetPosition()
		{
			return rigidBodyState->position;
		}

		AActor::~AActor(){}

		void AActor::CleanUp()
		{
			eae6320::Graphics::cRenderableObject::CleanUp();
			UObject::CleanUp();
		}

		void AActor::Draw()
		{
			if (!bIsDestroyed) 
			{
				SubmitMeshWithEffectToDraw();
			}
		}
		void AActor::Draw(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
		{
			if (!bIsDestroyed) 
			{
				SubmitMeshWithEffectToDraw(i_elapsedSecondCount_sinceLastSimulationUpdate);
			}
		}
	}
}
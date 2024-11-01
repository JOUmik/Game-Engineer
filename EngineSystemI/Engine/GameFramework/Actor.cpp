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

		AActor::~AActor(){}

		void AActor::CleanUp()
		{
			eae6320::Graphics::cRenderableObject::CleanUp();
			UObject::CleanUp();
		}


	}
}
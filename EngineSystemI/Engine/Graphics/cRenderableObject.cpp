#include "cRenderableObject.h"
#include "Graphics.h"

#include <Engine/Math/cMatrix_transformation.h>
namespace eae6320
{
	namespace Graphics
	{
		void cRenderableObject::CleanUp()
		{
			if (mesh)
			{
				mesh->DecrementReferenceCount();
				mesh = nullptr;
			}
			if (effect)
			{
				effect->DecrementReferenceCount();
				effect = nullptr;
			}
			if (rigidBodyState)
			{
				delete rigidBodyState;
			}
		}

		void cRenderableObject::ChangeMesh(Mesh* new_mesh)
		{
			if (!new_mesh) 
			{
				EAE6320_ASSERTF(false, "Can't change mesh because input is nullptr");
			}
			mesh->DecrementReferenceCount();
			mesh = new_mesh;
			mesh->IncrementReferenceCount();
		}

		void cRenderableObject::ChangeEffect(Effect* new_effect)
		{
			if (!new_effect)
			{
				EAE6320_ASSERTF(false, "Can't change effect because input is nullptr");
			}
			effect->DecrementReferenceCount();
			effect = new_effect;
			effect->IncrementReferenceCount();
		}

		void cRenderableObject::SubmitMeshWithEffectToDraw() 
		{
			if (!mesh)
			{
				EAE6320_ASSERTF(false, "Can't draw actor because i_mesh is nullptr");
			}
			if (!effect)
			{
				EAE6320_ASSERTF(false, "Can't draw actor because i_effect is nullptr");
			}
			eae6320::Graphics::BindMeshWithEffect(mesh, effect, Math::cMatrix_transformation(rigidBodyState->orientation, rigidBodyState->position));
		}

		void cRenderableObject::SubmitMeshWithEffectToDraw(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
		{
			if (!mesh)
			{
				EAE6320_ASSERTF(false, "Can't draw actor because i_mesh is nullptr");
			}
			if (!effect)
			{
				EAE6320_ASSERTF(false, "Can't draw actor because i_effect is nullptr");
			}
			eae6320::Graphics::BindMeshWithEffect(mesh, effect, rigidBodyState->PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}
	}
}
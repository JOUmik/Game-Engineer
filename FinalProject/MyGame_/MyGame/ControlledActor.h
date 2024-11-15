#pragma once

#include <Engine/GameFramework/Actor.h>
#include <Engine/Collision/SphereCollisionComponent.h>
#include <Engine/Graphics/Graphics.h>

namespace eae6320
{
	class AControlledActor : public GameFramework::AActor
	{
	public:
		AControlledActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect);
		~AControlledActor();

		virtual void Begin();
		virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void UpdatePosition(const Math::sVector& safePosition);

		void OnComponentHit(const Collision::BaseCollisionComponent&);

		inline Collision::SphereCollisionComponent* GetSphereComp() { return SphereComp; }

		virtual void CleanUp();

	private:
		Graphics::Effect* effect = nullptr;
		Collision::SphereCollisionComponent* SphereComp;
	};
}

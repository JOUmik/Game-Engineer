#pragma once
#pragma once

#include <Engine/GameFramework/Actor.h>
#include <Engine/Collision/BoxCollisionComponent.h>
#include <Engine/Graphics/Graphics.h>

namespace eae6320
{
	class ALaserBullet : public GameFramework::AActor
	{
	public:
		ALaserBullet(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect);
		~ALaserBullet();

		virtual void Begin();
		virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void UpdatePosition(const Math::sVector& safePosition);

		void OnComponentHit(const Collision::BaseCollisionComponent&);

		inline Collision::BoxCollisionComponent* GetBoxComp() { return BoxComp; }

		virtual void CleanUp();

	private:
		Collision::BoxCollisionComponent* BoxComp;
	};
}
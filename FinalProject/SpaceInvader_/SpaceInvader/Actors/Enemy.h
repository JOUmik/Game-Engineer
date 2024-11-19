#pragma once

#include <Engine/GameFramework/Actor.h>
#include <Engine/Collision/BoxCollisionComponent.h>
#include <Engine/Graphics/Graphics.h>

namespace eae6320
{
	class AEnemy : public GameFramework::AActor
	{
	public:
		Delegate<> Destoryed;
		AEnemy(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect);
		~AEnemy();

		virtual void Begin();
		virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void UpdatePosition(const Math::sVector& safePosition);

		void OnComponentHit(const Collision::BaseCollisionComponent&);
		void OnComponentBeginOverlap(const Collision::BaseCollisionComponent&);

		inline Collision::BoxCollisionComponent* GetBoxComp() { return boxComp; }

		virtual void CleanUp();

		void HideInTheGame();
		bool bIsShow = true;

		static bool OverlapEventHappened;

	private:
		Collision::BoxCollisionComponent* boxComp;
	};
}
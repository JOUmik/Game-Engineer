#pragma once
#include <Engine/GameFramework/Actor.h>
#include <Engine/Collision/BoxCollisionComponent.h>
#include <Engine/Graphics/Graphics.h>

namespace eae6320
{
	class ABlock : public GameFramework::AActor
	{
	public:
		Delegate<> EnemyOverlapWithBlock;
		ABlock(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect);
		~ABlock();

		virtual void Begin();
		virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void OnComponentBeginOverlap(const Collision::BaseCollisionComponent&);

		inline Collision::BoxCollisionComponent* GetBoxComp() { return boxComp; }

		virtual void CleanUp();

	private:
		Collision::BoxCollisionComponent* boxComp;
	};
}

#pragma once
#include <Engine/GameFramework/Actor.h>
#include <Engine/Collision/BoxCollisionComponent.h>
#include <Engine/Graphics/Graphics.h>

namespace eae6320
{
	class AOverlapBeginTestActor : public GameFramework::AActor
	{
	public:
		AOverlapBeginTestActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect);
		~AOverlapBeginTestActor();

		virtual void Begin();
		virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void OnComponentBeginOverlap(const Collision::BaseCollisionComponent&);

		inline Collision::BoxCollisionComponent* GetBoxComp() { return boxComp; }

		virtual void CleanUp();

	private:
		Graphics::Effect* effect = nullptr;
		Collision::BoxCollisionComponent* boxComp;
	};
}

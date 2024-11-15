#pragma once
#include <Engine/GameFramework/Actor.h>
#include <Engine/Collision/BoxCollisionComponent.h>
#include <Engine/Graphics/Graphics.h>

namespace eae6320
{
	class AOverlapEndTestActor : public GameFramework::AActor
	{
	public:
		AOverlapEndTestActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect);
		~AOverlapEndTestActor();

		virtual void Begin();
		virtual void Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate);

		void OnComponentEndOverlap(const Collision::BaseCollisionComponent&);

		inline Collision::BoxCollisionComponent* GetBoxComp() { return boxComp; }

		virtual void CleanUp();

		bool show = true;

	private:
		Collision::BoxCollisionComponent* boxComp;
	};
}

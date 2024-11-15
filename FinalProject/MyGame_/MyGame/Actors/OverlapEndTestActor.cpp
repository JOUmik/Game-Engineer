#include "OverlapEndTestActor.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::AOverlapEndTestActor::AOverlapEndTestActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    boxComp = new Collision::BoxCollisionComponent();
    boxComp->SetCollisionEvent(Collision::CollisionEvent::Overlap);
    boxComp->SetCollisionComponentType(Collision::CollisionComponentType::Static);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*boxComp);

    //Bind Event
    boxComp->OnComponentEndOverlap.Add(this, &AOverlapEndTestActor::OnComponentEndOverlap);
}

eae6320::AOverlapEndTestActor::~AOverlapEndTestActor()
{
}

void eae6320::AOverlapEndTestActor::Begin()
{
    boxComp->SetPosition(rigidBodyState->position);
}

void eae6320::AOverlapEndTestActor::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
    if (rigidBodyState->velocity.GetLength() != 0)
    {
        boxComp->TryMoveTo(rigidBodyState->PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
    }
}

void eae6320::AOverlapEndTestActor::OnComponentEndOverlap(const Collision::BaseCollisionComponent&)
{
    show = false;
}


void eae6320::AOverlapEndTestActor::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    delete boxComp;
}

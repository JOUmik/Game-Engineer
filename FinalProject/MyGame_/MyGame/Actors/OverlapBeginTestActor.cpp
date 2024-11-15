#include "OverlapBeginTestActor.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::AOverlapBeginTestActor::AOverlapBeginTestActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedColor.shader", effect);

    boxComp = new Collision::BoxCollisionComponent();
    boxComp->SetCollisionEvent(Collision::CollisionEvent::Overlap);
    boxComp->SetCollisionComponentType(Collision::CollisionComponentType::Static);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*boxComp);

    //Bind Event
    boxComp->OnComponentBeginOverlap.Add(this, &AOverlapBeginTestActor::OnComponentBeginOverlap);
}

eae6320::AOverlapBeginTestActor::~AOverlapBeginTestActor()
{
}

void eae6320::AOverlapBeginTestActor::Begin()
{
    boxComp->SetPosition(rigidBodyState->position);
}

void eae6320::AOverlapBeginTestActor::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
    if (rigidBodyState->velocity.GetLength() != 0)
    {
        boxComp->TryMoveTo(rigidBodyState->PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
    }
}

void eae6320::AOverlapBeginTestActor::OnComponentBeginOverlap(const Collision::BaseCollisionComponent&)
{
    if (boxComp->GetCollisionComponentType() == Collision::CollisionComponentType::Static)
    {
        ChangeEffect(effect);
    }
}


void eae6320::AOverlapBeginTestActor::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    delete boxComp;
    if (effect != nullptr)
    {
        effect->DecrementReferenceCount();
    }
}

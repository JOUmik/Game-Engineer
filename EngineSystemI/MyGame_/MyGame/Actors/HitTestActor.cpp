#include "HitTestActor.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::AHitTestActor::AHitTestActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Gray.shader", effect);

    boxComp = new Collision::BoxCollisionComponent();
    boxComp->SetCollisionEvent(Collision::CollisionEvent::Hit);
    boxComp->SetCollisionComponentType(Collision::CollisionComponentType::Static);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*boxComp);

    //Bind Event
    boxComp->UpdatePositionAfterCollision.Add(this, &AHitTestActor::UpdatePosition);
    boxComp->OnComponentHit.Add(this, &AHitTestActor::OnComponentHit);
}

eae6320::AHitTestActor::~AHitTestActor()
{
}

void eae6320::AHitTestActor::Begin()
{
    boxComp->SetPosition(rigidBodyState->position);
}

void eae6320::AHitTestActor::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
    if (rigidBodyState->velocity.GetLength() != 0) 
    {
        boxComp->TryMoveTo(rigidBodyState->PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
    }
}

void eae6320::AHitTestActor::UpdatePosition(const Math::sVector& safePosition)
{
    SetPosition(safePosition);
}

void eae6320::AHitTestActor::OnComponentHit(const Collision::BaseCollisionComponent&)
{
    if (boxComp->GetCollisionComponentType() == Collision::CollisionComponentType::Static) 
    {
        ChangeEffect(effect);
    }
}


void eae6320::AHitTestActor::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    delete boxComp;
    if (effect != nullptr) 
    {
        effect->DecrementReferenceCount();
    }
}

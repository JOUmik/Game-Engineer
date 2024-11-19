#include "ControlledActor.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::AControlledActor::AControlledActor(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Gray.shader", effect);

    SphereComp = new Collision::SphereCollisionComponent();
    SphereComp->SetCollisionEvent(Collision::CollisionEvent::Hit);
    SphereComp->SetCollisionComponentType(Collision::CollisionComponentType::Static);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*SphereComp);

    //Bind Event
    SphereComp->UpdatePositionAfterCollision.Add(this, &AControlledActor::UpdatePosition);
    SphereComp->OnComponentHit.Add(this, &AControlledActor::OnComponentHit);
}

eae6320::AControlledActor::~AControlledActor()
{
}

void eae6320::AControlledActor::Begin()
{
    SphereComp->SetPosition(rigidBodyState->position);
}

void eae6320::AControlledActor::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
    if (rigidBodyState->velocity.GetLength() != 0)
    {
        SphereComp->TryMoveTo(rigidBodyState->PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
    }
}

void eae6320::AControlledActor::UpdatePosition(const Math::sVector& safePosition)
{
    SetPosition(safePosition);
}

void eae6320::AControlledActor::OnComponentHit(const Collision::BaseCollisionComponent&)
{
    if (SphereComp->GetCollisionComponentType() == Collision::CollisionComponentType::Static)
    {
        ChangeEffect(effect);
    }
    bHitEventGeneratedCurrentFrame = true;
}


void eae6320::AControlledActor::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*SphereComp);
    delete SphereComp;
    if (effect != nullptr)
    {
        effect->DecrementReferenceCount();
    }
}

#include "Enemy.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::AEnemy::AEnemy(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    boxComp = new Collision::BoxCollisionComponent();
    boxComp->SetCollisionEvent(Collision::CollisionEvent::Hit);
    boxComp->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*boxComp);

    rigidBodyState->velocity.x -= 1.f;

    //Bind Event
    boxComp->UpdatePositionAfterCollision.Add(this, &AEnemy::UpdatePosition);
    boxComp->OnComponentHit.Add(this, &AEnemy::OnComponentHit);
}

eae6320::AEnemy::~AEnemy()
{
}

void eae6320::AEnemy::Begin()
{
    boxComp->SetPosition(rigidBodyState->position);
}

void eae6320::AEnemy::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
    if (rigidBodyState->velocity.GetLength() != 0)
    {
        boxComp->TryMoveTo(rigidBodyState->PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
    }
}

void eae6320::AEnemy::UpdatePosition(const Math::sVector& safePosition)
{
    SetPosition(safePosition);
}

void eae6320::AEnemy::OnComponentHit(const Collision::BaseCollisionComponent&)
{
    Destoryed.Broadcast();
    HideInTheGame();
}

void eae6320::AEnemy::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    delete boxComp;
    boxComp = nullptr;
}

void eae6320::AEnemy::HideInTheGame()
{
    rigidBodyState->velocity = Math::sVector(0.f, 0.f, 0.f);
    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    bIsShow = false;
}
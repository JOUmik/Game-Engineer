#include "LaserBullet.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::ALaserBullet::ALaserBullet(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    BoxComp = new Collision::BoxCollisionComponent();
    BoxComp->SetCollisionEvent(Collision::CollisionEvent::Hit);
    BoxComp->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*BoxComp);

    rigidBodyState->velocity.y = 7.f;

    //Bind Event
    BoxComp->UpdatePositionAfterCollision.Add(this, &ALaserBullet::UpdatePosition);
    BoxComp->OnComponentHit.Add(this, &ALaserBullet::OnComponentHit);
}

eae6320::ALaserBullet::~ALaserBullet()
{
}

void eae6320::ALaserBullet::Begin()
{
    BoxComp->SetPosition(rigidBodyState->position);
}

void eae6320::ALaserBullet::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
    currentLifeTime += i_elapsedSecondCount_sinceLastSimulationUpdate;

    if (rigidBodyState->velocity.GetLength() != 0)
    {
        BoxComp->TryMoveTo(rigidBodyState->PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
    }

    if (currentLifeTime >= maxLifeTime) 
    {
        HideInTheGame();
    }
}

void eae6320::ALaserBullet::UpdatePosition(const Math::sVector& safePosition)
{
    SetPosition(safePosition);
}

void eae6320::ALaserBullet::OnComponentHit(const Collision::BaseCollisionComponent&)
{
    HideInTheGame();
}


void eae6320::ALaserBullet::CleanUp()
{
    if (!bIsDestroyed) 
    {
        bIsDestroyed = true;
        AActor::CleanUp();

        Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*BoxComp);
        delete BoxComp;
        BoxComp = nullptr;
    }
}

void eae6320::ALaserBullet::HideInTheGame()
{
    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*BoxComp);
    bIsShow = false;
}

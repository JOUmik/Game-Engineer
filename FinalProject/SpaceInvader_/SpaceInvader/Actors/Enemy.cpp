#include "Enemy.h"
#include <Engine/Collision/CollisionManager.h>

bool eae6320::AEnemy::OverlapEventHappened = false;

eae6320::AEnemy::AEnemy(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    boxComp = new Collision::BoxCollisionComponent();
    boxComp->SetCollisionEvent(Collision::CollisionEvent::Hit);
    boxComp->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*boxComp);

    //Audio
    ExplosionAudio = new AudioSystem::cAudio();
    ExplosionAudio->AudioConstructor("data/Audio/Explosion.mp3", "Explosion", 2000, false);
    ExplosionAudio->SubmitAudioToBePlayed();

    //Bind Event
    boxComp->UpdatePositionAfterCollision.Add(this, &AEnemy::UpdatePosition);
    boxComp->OnComponentHit.Add(this, &AEnemy::OnComponentHit);
    boxComp->OnComponentBeginOverlap.Add(this, &AEnemy::OnComponentBeginOverlap);
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
    ExplosionAudio->PlayIndependent();
    HideInTheGame();
}

void eae6320::AEnemy::OnComponentBeginOverlap(const Collision::BaseCollisionComponent&)
{
    OverlapEventHappened = true;
}


void eae6320::AEnemy::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    delete boxComp;
    boxComp = nullptr;
    delete ExplosionAudio;
    ExplosionAudio = nullptr;
}

void eae6320::AEnemy::HideInTheGame()
{
    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    bIsShow = false;
}
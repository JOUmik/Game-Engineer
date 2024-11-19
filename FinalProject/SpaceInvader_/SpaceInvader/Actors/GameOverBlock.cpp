#include "GameOverBlock.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::AGameOverBlock::AGameOverBlock(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Red.shader", effect);

    boxComp = new Collision::BoxCollisionComponent();
    boxComp->SetCollisionEvent(Collision::CollisionEvent::Overlap);
    boxComp->SetCollisionComponentType(Collision::CollisionComponentType::Static);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*boxComp);

    //Bind Event
    boxComp->OnComponentBeginOverlap.Add(this, &AGameOverBlock::OnComponentBeginOverlap);
}

eae6320::AGameOverBlock::~AGameOverBlock()
{
}

void eae6320::AGameOverBlock::Begin()
{
    boxComp->SetPosition(rigidBodyState->position);
}

void eae6320::AGameOverBlock::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
}

void eae6320::AGameOverBlock::OnComponentBeginOverlap(const Collision::BaseCollisionComponent&)
{
    ChangeEffect(effect);
    EnemyOverlapWithGameOverBlock.Broadcast();
}


void eae6320::AGameOverBlock::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    delete boxComp;
    if (effect != nullptr)
    {
        effect->DecrementReferenceCount();
    }
}

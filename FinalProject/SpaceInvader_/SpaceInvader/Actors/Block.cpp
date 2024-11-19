#include "Block.h"
#include <Engine/Collision/CollisionManager.h>

eae6320::ABlock::ABlock(eae6320::Graphics::Mesh* i_mesh, eae6320::Graphics::Effect* i_effect) : GameFramework::AActor(i_mesh, i_effect)
{
    Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedColor.shader", effect);

    boxComp = new Collision::BoxCollisionComponent();
    boxComp->SetCollisionEvent(Collision::CollisionEvent::Overlap);
    boxComp->SetCollisionComponentType(Collision::CollisionComponentType::Static);

    Collision::CollisionManager::GetCollisionManager()->AddCollisionComponent(*boxComp);

    //Bind Event
    boxComp->OnComponentBeginOverlap.Add(this, &ABlock::OnComponentBeginOverlap);
}

eae6320::ABlock::~ABlock()
{
}

void eae6320::ABlock::Begin()
{
    boxComp->SetPosition(rigidBodyState->position);
}

void eae6320::ABlock::Update(const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
}

void eae6320::ABlock::OnComponentBeginOverlap(const Collision::BaseCollisionComponent&)
{
    ChangeEffect(effect);
}


void eae6320::ABlock::CleanUp()
{
    AActor::CleanUp();

    Collision::CollisionManager::GetCollisionManager()->RemoveCollisionComponent(*boxComp);
    delete boxComp;
    if (effect != nullptr)
    {
        effect->DecrementReferenceCount();
    }
}

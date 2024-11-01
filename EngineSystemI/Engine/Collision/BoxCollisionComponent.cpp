#include "BoxCollisionComponent.h"
#include "SphereCollisionComponent.h"
#include "CollisionManager.h"

eae6320::Collision::BoxCollisionComponent::BoxCollisionComponent()
{
    collisionType = CollisionType::Box;
}

eae6320::Collision::BoxCollisionComponent::~BoxCollisionComponent()
{
}

void eae6320::Collision::BoxCollisionComponent::CheckCollision()
{
    BaseCollisionComponent::CheckCollision();
    std::unordered_set<BaseCollisionComponent&> collisionComponentSet = CollisionManager::GetCollisionManager()->collisionComponentSet;
    for (BaseCollisionComponent& collisionComp : collisionComponentSet) 
    {
        //TODO: use BVH structrue to speed up detect efficiency
        if (collisionComp.GetCollisionType() == CollisionType::Box) 
        {
            const BoxCollisionComponent& otherBox = dynamic_cast<const BoxCollisionComponent&>(collisionComp);
            if ((std::abs(position.x - otherBox.GetPosition().x) <= (extend.x + otherBox.GetExtend().x)) &&
                (std::abs(position.y - otherBox.GetPosition().y) <= (extend.y + otherBox.GetExtend().y)) &&
                (std::abs(position.z - otherBox.GetPosition().z) <= (extend.z + otherBox.GetExtend().z)))
            {
                CollisionManager::GetCollisionManager()->CheckAndBroadcast_OnHit(*this, collisionComp);
            }
        }
        else if (collisionComp.GetCollisionType() == CollisionType::Sphere) 
        {
            const auto& otherSphere = dynamic_cast<const SphereCollisionComponent&>(collisionComp);
            Math::sVector min = position - extend;
            Math::sVector max = position + extend;
            Math::sVector closestPoint = Math::sVector(
                std::max(min.x, std::min(otherSphere.GetPosition().x, max.x)),
                std::max(min.y, std::min(otherSphere.GetPosition().y, max.y)),
                std::max(min.z, std::min(otherSphere.GetPosition().z, max.z))
            );
            float distanceSquared = (closestPoint - otherSphere.GetPosition()).GetLength();
            if (distanceSquared <= otherSphere.GetRadius())
            {
                CollisionManager::GetCollisionManager()->CheckAndBroadcast_OnHit(*this, collisionComp);
            }
        }
    }
}

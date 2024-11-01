#include "SphereCollisionComponent.h"
#include "BoxCollisionComponent.h"
#include "CollisionManager.h"

eae6320::Collision::SphereCollisionComponent::SphereCollisionComponent()
{
    collisionType = CollisionType::Sphere;
}

eae6320::Collision::SphereCollisionComponent::~SphereCollisionComponent()
{
}

void eae6320::Collision::SphereCollisionComponent::CheckCollision()
{
    BaseCollisionComponent::CheckCollision();
    std::unordered_set<BaseCollisionComponent&> collisionComponentSet = CollisionManager::GetCollisionManager()->collisionComponentSet;
    for (BaseCollisionComponent& collisionComp : collisionComponentSet)
    {
        //TODO: use BVH structrue to speed up detect efficiency
        if (collisionComp.GetCollisionType() == CollisionType::Box)
        {
            const auto& otherBox = dynamic_cast<const BoxCollisionComponent&>(collisionComp);
            Math::sVector min = otherBox.GetPosition() - otherBox.GetExtend();
            Math::sVector max = otherBox.GetPosition() + otherBox.GetExtend();
            Math::sVector closestPoint = Math::sVector(
                std::max(min.x, std::min(position.x, max.x)),
                std::max(min.y, std::min(position.y, max.y)),
                std::max(min.z, std::min(position.z, max.z))
            );
            float distanceSquared = (closestPoint - position).GetLength();
            if (distanceSquared <= radius)
            {
                CollisionManager::GetCollisionManager()->CheckAndBroadcast_OnHit(*this, collisionComp);
            }
        }
        else if (collisionComp.GetCollisionType() == CollisionType::Sphere)
        {
            const auto& otherSphere = dynamic_cast<const SphereCollisionComponent&>(collisionComp);
            float combinedRadius = radius + otherSphere.GetRadius();
            if ((position - otherSphere.GetPosition()).GetLength() <= combinedRadius)
            {
                CollisionManager::GetCollisionManager()->CheckAndBroadcast_OnHit(*this, collisionComp);
            }
        }
    }
}

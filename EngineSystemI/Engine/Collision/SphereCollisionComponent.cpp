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
        if (&collisionComp == this) continue;
        if (DetectCollision(collisionComp))
        {
            CollisionManager::GetCollisionManager()->CheckAndBroadcast_OnHit(*this, collisionComp);
        }
    }

}

bool eae6320::Collision::SphereCollisionComponent::DetectCollision(BaseCollisionComponent& otherComp)
{
    if (otherComp.GetCollisionType() == CollisionType::Box)
    {
        const auto& otherBox = dynamic_cast<const BoxCollisionComponent&>(otherComp);
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
            return true;
        }
    }
    else if (otherComp.GetCollisionType() == CollisionType::Sphere)
    {
        const auto& otherSphere = dynamic_cast<const SphereCollisionComponent&>(otherComp);
        float combinedRadius = radius + otherSphere.GetRadius();
        if ((position - otherSphere.GetPosition()).GetLength() <= combinedRadius)
        {
            return true;
        }
    }

    return false;
}

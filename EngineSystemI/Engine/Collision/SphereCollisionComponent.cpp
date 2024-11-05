#include "SphereCollisionComponent.h"
#include "BoxCollisionComponent.h"
#include "CollisionManager.h"

eae6320::Collision::SphereCollisionComponent::SphereCollisionComponent()
{
    collisionShape = CollisionShape::Sphere;
}

eae6320::Collision::SphereCollisionComponent::~SphereCollisionComponent()
{
}

void eae6320::Collision::SphereCollisionComponent::CheckOverlap()
{
    BaseCollisionComponent::CheckOverlap();
    std::unordered_set<BaseCollisionComponent*> collisionComponentSet = CollisionManager::GetCollisionManager()->collisionComponentSet;
    for (auto& collisionComp : collisionComponentSet)
    {
        //TODO: use BVH structrue to speed up detect efficiency
        if (collisionComp == this) continue;
        if (DetectCollision(*collisionComp))
        {
            //If collision is detected, it may be ok to broadcast BeginOverlap event
            CollisionManager::GetCollisionManager()->CheckAndBroadcast_OnBeginOverlap(*this, *collisionComp);
        }
        else
        {
            //If collision is not detected, it may be ok to broadcast EndOverlap event
            CollisionManager::GetCollisionManager()->CheckAndBroadcast_OnEndOverlap(*this, *collisionComp);
        }
    }

}

bool eae6320::Collision::SphereCollisionComponent::DetectCollision(BaseCollisionComponent& otherComp)
{
    if (otherComp.GetCollisionShape() == CollisionShape::Box)
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
    else if (otherComp.GetCollisionShape() == CollisionShape::Sphere)
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

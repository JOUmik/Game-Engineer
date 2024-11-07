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

eae6320::Collision::BoundingBox eae6320::Collision::SphereCollisionComponent::GetBoundingBox() const
{
    Math::sVector radiusVec(radius, radius, radius);
    Math::sVector min = position - radiusVec;
    Math::sVector max = position + radiusVec;
    return BoundingBox(min, max);
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

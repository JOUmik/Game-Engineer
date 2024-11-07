#pragma once
#include <Engine/Math/sVector.h>
#include <unordered_set>
#include <vector>
#include <memory>

namespace eae6320
{
    namespace Collision
    {
        class BaseCollisionComponent;
        struct BoundingBox 
        {
            Math::sVector min;
            Math::sVector max;

            BoundingBox() : min(Math::sVector(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max())),
                max(Math::sVector(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max())) {}

            BoundingBox(const Math::sVector& min, const Math::sVector& max) : min(min), max(max) {}

            // check whether two bounding boxes are intersect with each other
            bool Intersects(const BoundingBox& other) const 
            {
                return (min.x <= other.max.x && max.x >= other.min.x) &&
                    (min.y <= other.max.y && max.y >= other.min.y) &&
                    (min.z <= other.max.z && max.z >= other.min.z);
            }

            // calculate the union bounding box for two bounding boxes
            static BoundingBox Union(const BoundingBox& a, const BoundingBox& b) 
            {
                Math::sVector newMin(std::min(a.min.x, b.min.x), std::min(a.min.y, b.min.y), std::min(a.min.z, b.min.z));
                Math::sVector newMax(std::max(a.max.x, b.max.x), std::max(a.max.y, b.max.y), std::max(a.max.z, b.max.z));
                return BoundingBox(newMin, newMax);
            }

            // get the center of bounding box
            Math::sVector Center() const 
            {
                return (min + max) * 0.5f;
            }

            // get the longest axis of bounding box
            int LongestAxis() const 
            {
                Math::sVector size = max - min;
                if (size.x > size.y && size.x > size.z) return 0;
                if (size.y > size.z) return 1;
                return 2;
            }
        };

        struct BVHNode 
        {
            BoundingBox bounds;
            std::unique_ptr<BVHNode> left;
            std::unique_ptr<BVHNode> right;
            std::vector<BaseCollisionComponent*> components;

            bool IsLeaf() const 
            {
                return left == nullptr && right == nullptr;
            }
        };
    }
}
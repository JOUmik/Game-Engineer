#pragma once
#include "BaseCollisionComponent.h"
#include "BVHNode.h"

#include <Engine/Math/sVector.h>
#include <unordered_set>
#include <vector>
#include <memory>

namespace eae6320
{
    namespace Collision
    {
        struct CollisionPair {
            BaseCollisionComponent* componentA;
            BaseCollisionComponent* componentB;

            bool operator==(const CollisionPair& other) const {
                return (componentA == other.componentA && componentB == other.componentB) ||
                    (componentA == other.componentB && componentB == other.componentA);
            }
        };

        // custom hash function
        struct CollisionPairHash {
            std::size_t operator()(const CollisionPair& pair) const {
                return std::hash<void*>()(pair.componentA) ^ std::hash<void*>()(pair.componentB);
            }
        };

        class CollisionManager 
        {
        public:
            CollisionManager(const CollisionManager& p) = delete;
            CollisionManager(CollisionManager&& p) = delete;
            CollisionManager& operator=(const CollisionManager& p) = delete;
            

            std::unordered_set<BaseCollisionComponent*> collisionComponentSet;

            void AddCollisionComponent(BaseCollisionComponent& comp);
            void RemoveCollisionComponent(BaseCollisionComponent& comp);
            //If the collision between compA and compB is not handled before in current frame, this function would call 
            void CheckAndBroadcast_OnHit(BaseCollisionComponent& compA, BaseCollisionComponent& compB);
            void CheckAndBroadcast_OnBeginOverlap(BaseCollisionComponent& compA, BaseCollisionComponent& compB);
            void CheckAndBroadcast_OnEndOverlap(BaseCollisionComponent& comp);
            static CollisionManager* GetCollisionManager();
            void Begin();
            void Update();
            void Destroy();

        private:
            static CollisionManager* collisionManager;
            std::unordered_set<CollisionPair, CollisionPairHash> hitCache;
            std::unordered_set<CollisionPair, CollisionPairHash> overlapBeginCache;
            std::unordered_set<CollisionPair, CollisionPairHash> overlapEndCache;
            std::unordered_set<CollisionPair, CollisionPairHash> currentOverlaps;
            std::unordered_set<CollisionPair, CollisionPairHash> previousOverlaps;
            std::unique_ptr<BVHNode> staticBVH;
            std::unique_ptr<BVHNode> dynamicBVH;
            CollisionManager();
            ~CollisionManager();
            void BuildStaticBVH();
            void BuildDynamicBVH();
            std::unique_ptr<BVHNode> BuildBVH(std::vector<BaseCollisionComponent*>& comps, int start, int end);
            void DetectCollisionsWithBVH(BaseCollisionComponent* target, BVHNode* node, bool& hitOccurred);
            void ClearCache();
            Math::sVector Lerp(const Math::sVector& start, const Math::sVector& end, float t);
            Math::sVector PerformBinarySearch(const Math::sVector& start, const Math::sVector& end, float low, float high, BaseCollisionComponent& comp);
        };
    }
}
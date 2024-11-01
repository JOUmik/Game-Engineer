#pragma once
#include "BaseCollisionComponent.h"

#include <unordered_set>

namespace eae6320
{
    namespace Collision
    {
        struct CollisionPair {
            void* componentA;
            void* componentB;

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
            

            static std::unordered_set<BaseCollisionComponent&> collisionComponentSet;

            //If the collision between compA and compB is not handled before, this function would call 
            void CheckAndBroadcast_OnHit(BaseCollisionComponent& compA, BaseCollisionComponent& compB);
            static CollisionManager* GetCollisionManager();
            void Update();
            void Destroy();

        private:
            static CollisionManager* collisionManager;
            std::unordered_set<CollisionPair, CollisionPairHash> collisionCache;
            CollisionManager();
            ~CollisionManager();

            void ClearCache();
        };
    }
}
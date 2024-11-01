#pragma once
#include "Delegate.h"

#include <Engine/Math/sVector.h>
#include <string>


namespace eae6320
{
    namespace Collision
    {
        // HitResult
        struct HitResult
        {
            std::string OtherComponentName;
        };

        enum class CollisionType
        {
            None,
            Box,
            Sphere
        };

        class BaseCollisionComponent
        {
        public:
            Delegate<const BaseCollisionComponent&> OnComponentHit;
            Delegate<const BaseCollisionComponent&> OnComponentBeginOverlap;
            Delegate<const BaseCollisionComponent&> OnComponentEndOverlap;

            virtual void CheckCollision() {}

            void OnHit(const BaseCollisionComponent& other)
            {
                OnComponentHit.Broadcast(other);
            }

            void OnBeginOverlap(const BaseCollisionComponent& other)
            {
                OnComponentBeginOverlap.Broadcast(other);
            }

            void OnEndOverlap(const BaseCollisionComponent& other)
            {
                OnComponentEndOverlap.Broadcast(other);
            }

            virtual bool DetectCollision(BaseCollisionComponent& other) {};

            inline Math::sVector GetPosition() const { return position;  }
            inline CollisionType GetCollisionType() const { return collisionType; }

        protected:
            CollisionType collisionType = CollisionType::None;
            Math::sVector position = Math::sVector();
        };
    }
}
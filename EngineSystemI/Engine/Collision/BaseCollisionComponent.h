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
                if (!bHitEventEnabled) return;
                OnComponentHit.Broadcast(other);
            }

            void OnBeginOverlap(const BaseCollisionComponent& other)
            {
                if (!bOverlapEventEnabled) return;
                OnComponentBeginOverlap.Broadcast(other);
            }

            void OnEndOverlap(const BaseCollisionComponent& other)
            {
                if (!bOverlapEventEnabled) return;
                OnComponentEndOverlap.Broadcast(other);
            }

            virtual bool DetectCollision(BaseCollisionComponent& other) {};

            //This function would set the targetPosition of component to i_targetPosition, but you still need to 
            void TryMoveTo(Math::sVector i_targetPosition) { targetPosition = i_targetPosition; }

            void EnableHitEvent(bool enable = true) { bHitEventEnabled = enable; }
            void EnableOverlapEvent(bool enable = true) { bOverlapEventEnabled = enable; }
            inline bool IsHitEventEnabled() const { return bHitEventEnabled; }
            inline bool IsOverlapEventEnabled() const { return bOverlapEventEnabled; }
            inline Math::sVector GetPosition() const { return position;  }
            inline CollisionType GetCollisionType() const { return collisionType; }

        protected:
            CollisionType collisionType = CollisionType::None;
            Math::sVector position = Math::sVector();
            Math::sVector targetPosition = Math::sVector();

        protected:
            bool bHitEventEnabled = true;
            bool bOverlapEventEnabled = true;
        };
    }
}
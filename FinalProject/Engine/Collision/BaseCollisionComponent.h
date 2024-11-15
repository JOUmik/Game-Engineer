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

        enum class CollisionShape
        {
            None,
            Box,
            Sphere
        };

        enum class CollisionComponentType
        {
            Static,
            Dynamic
        };

        enum class CollisionEvent 
        {
            NoCollision,
            Overlap,
            Hit
        };
        struct BoundingBox;
        class BaseCollisionComponent
        {
        public:
            Delegate<const BaseCollisionComponent&> OnComponentHit;
            Delegate<const BaseCollisionComponent&> OnComponentBeginOverlap;
            Delegate<const BaseCollisionComponent&> OnComponentEndOverlap;
            Delegate<const Math::sVector&> UpdatePositionAfterCollision;

            virtual BoundingBox GetBoundingBox() const = 0;

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

            void UpdateActorPostion(const Math::sVector& pos) 
            {
                UpdatePositionAfterCollision.Broadcast(pos);
            }

            virtual bool DetectCollision(BaseCollisionComponent& other) { return true; };

            //This function would set the targetPosition of component to i_targetPosition, but you still need to 
            void TryMoveTo(Math::sVector i_targetPosition) 
            { 
                targetPosition = i_targetPosition;
                bIsMoving = true;
            }

            inline void SetCollisionEvent(CollisionEvent i_collisionEvent = CollisionEvent::Hit) { collisionEvent = i_collisionEvent; }
            inline void SetCollisionComponentType(CollisionComponentType i_collisionComponentType = CollisionComponentType::Dynamic) { collisionComponentType = i_collisionComponentType; }
            inline bool IsHitEventEnabled() const { return collisionEvent >= CollisionEvent::Hit; }
            inline bool IsOverlapEventEnabled() const { return collisionEvent >= CollisionEvent::Overlap;}
            inline void SetPosition(Math::sVector i_position) { position = i_position; }
            inline Math::sVector GetPosition() const { return position;  }
            inline Math::sVector GetTargetPosition() const { return targetPosition; }
            inline CollisionShape GetCollisionShape() const { return collisionShape; }
            inline CollisionComponentType GetCollisionComponentType() const { return collisionComponentType; }

            bool bIsMoving = false;
        protected:
            CollisionShape collisionShape = CollisionShape::None;
            CollisionComponentType collisionComponentType = CollisionComponentType::Dynamic;
            CollisionEvent collisionEvent = CollisionEvent::Hit;
            Math::sVector position = Math::sVector();
            Math::sVector targetPosition = Math::sVector();

        private:
            bool bHitEventEnabled = true;
            bool bOverlapEventEnabled = true;
        };
    }
}
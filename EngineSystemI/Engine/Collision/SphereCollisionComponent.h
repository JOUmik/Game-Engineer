#pragma once
#include "BaseCollisionComponent.h"

namespace eae6320
{
    namespace Collision
    {
        class SphereCollisionComponent : public BaseCollisionComponent
        {
        public:
            SphereCollisionComponent();
            ~SphereCollisionComponent();

            virtual void CheckOverlap();
            virtual bool DetectCollision(BaseCollisionComponent& otherComp);

            inline float GetRadius() const { return radius; }

        private:
            float radius = 5.f;
        };
    }
}

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

            virtual BoundingBox GetBoundingBox() const override;
            virtual bool DetectCollision(BaseCollisionComponent& otherComp) override;

            inline float GetRadius() const { return radius; }

        private:
            float radius = 5.f;
        };
    }
}

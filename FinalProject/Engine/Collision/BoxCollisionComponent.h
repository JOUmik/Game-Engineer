#pragma once
#include "BaseCollisionComponent.h"

namespace eae6320
{
    namespace Collision
    {
        class BoxCollisionComponent : public BaseCollisionComponent 
        {
        public:
            BoxCollisionComponent();
            ~BoxCollisionComponent();

            virtual BoundingBox GetBoundingBox() const override;
            virtual bool DetectCollision(BaseCollisionComponent& otherComp) override;

            inline void SetExtend(Math::sVector i_extend) { extend = i_extend; }
            inline Math::sVector GetExtend() const { return extend; }

        protected:

            Math::sVector extend = Math::sVector(10.f, 10.f, 10.f);
        };
    }
}

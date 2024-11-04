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

            virtual void CheckOverlap();
            virtual bool DetectCollision(BaseCollisionComponent& otherComp);

            inline Math::sVector GetExtend() const { return extend; }

        protected:

            Math::sVector extend = Math::sVector(10.f, 10.f, 10.f);
        };
    }
}

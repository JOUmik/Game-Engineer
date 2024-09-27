#include "CameraActor.h"

#include <Engine/Graphics/Graphics.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>

namespace eae6320
{
    namespace GameFramework
    {
        ACameraActor::ACameraActor()
        {
            rigidBodyState = new eae6320::Physics::sRigidBodyState();

            //init camera data
            verticalFieldOfView_inRadians = eae6320::Math::ConvertDegreesToRadians(45.f);
            aspectRatio = 1.f;
            z_nearPlane = 0.f;
            z_farPlane = 20.f;
            rigidBodyState->position = Math::sVector(0.f, 0.f, 10.f);
        }

        void ACameraActor::SubmitCameraData(const float i_secondCountToExtrapolate)
        {
            // Calculate Camera Data
            auto cameraToProjected = Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(verticalFieldOfView_inRadians, aspectRatio, z_nearPlane, z_farPlane);
            auto worldToCamera = Math::cMatrix_transformation::CreateWorldToCameraTransform(
                rigidBodyState->PredictFutureTransform(i_secondCountToExtrapolate)
            );
            // Submit Camera Data to Graphics
            Graphics::BindCameraData(cameraToProjected, worldToCamera);
        }

        void ACameraActor::Update(const float i_elapsedSecondCount_sinceLastUpdate)
        {
            rigidBodyState->Update(i_elapsedSecondCount_sinceLastUpdate);
        }

        void ACameraActor::CleanUp()
        {
            delete rigidBodyState;
        }
    }
}

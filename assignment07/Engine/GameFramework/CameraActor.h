#pragma once

#include <Engine/Math/sVector.h>
#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320
{
	namespace GameFramework
	{
		class ACameraActor
		{
		public:
			ACameraActor();
			~ACameraActor() = default;
			void SubmitCameraData(const float i_secondCountToExtrapolate);
			void Update(const float i_elapsedSecondCount_sinceLastUpdate);
			void CleanUp();

			inline void SetVelocity(Math::sVector velocity) { rigidBodyState->velocity = velocity; }
			inline Math::sVector GetVelocity() { return rigidBodyState->velocity; }

			inline void SetPosition(Math::sVector position) { rigidBodyState->position = position; }

			float verticalFieldOfView_inRadians;
			float aspectRatio;
			float z_nearPlane;
			float z_farPlane;

		private:
			eae6320::Physics::sRigidBodyState* rigidBodyState = nullptr;
		};
	}
}
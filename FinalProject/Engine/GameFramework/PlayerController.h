#pragma once

namespace eae6320
{
	namespace GameFramework
	{
		class ACameraActor;
		class APlayerController 
		{
		public:
			void Update(const float i_elapsedSecondCount_sinceLastUpdate);
			void SetCurrentCamera(ACameraActor* newCamera);
			void SubmitDataToGraphics(const float i_secondCountToExtrapolate);

		private:
			ACameraActor* currentCamera = nullptr;
		};
	}
}
#include "PlayerController.h"

#include "CameraActor.h"

void eae6320::GameFramework::APlayerController::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
    if (currentCamera) 
    {
        currentCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
    }
}


void eae6320::GameFramework::APlayerController::SetCurrentCamera(ACameraActor* newCamera)
{
    //TODO: camera add the ability to point counter
    currentCamera = newCamera;
}

void eae6320::GameFramework::APlayerController::SubmitDataToGraphics(const float i_secondCountToExtrapolate)
{
    if (currentCamera) 
    {
        currentCamera->SubmitCameraData(i_secondCountToExtrapolate);
    }
}

// Includes
//=========

#include "cSpaceInvader.h"
#include "Actors/HitTestActor.h"
#include "Actors/OverlapBeginTestActor.h"
#include "Actors/OverlapEndTestActor.h"
#include "ControlledActor.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/Effect.h>
#include <Engine/Graphics/Mesh.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/GameFramework/Actor.h>
#include <Engine/GameFramework/CameraActor.h>
#include <Engine/GameFramework/PlayerController.h>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>
#include <Engine/Collision/CollisionManager.h>
#include <Engine/Audio/cAudioSystem.h>

// Inherited Implementation
//=========================

// Run
//----
void eae6320::cSpaceInvader::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	//animated background
	float simulateTime = static_cast<float>(GetElapsedSecondCount_simulation());
	float r = (std::cos(9.0f * simulateTime) * 0.1f) + 0.15f;
	float g = (std::sin(2.0f * simulateTime) * 0.1f) + 0.15f;
	float b = (-std::cos(5.0f * simulateTime) * 0.2f) + 0.25f;
	Graphics::UpdateBackgroundColor(r, g, b, backgroundColor.a);
	
	// Player Controller send the binded camera date to graphics
	{
		playerController->SubmitDataToGraphics(i_elapsedSecondCount_sinceLastSimulationUpdate);
	}

	//Draw Actors
	{
		overlapBeginActor->SubmitMeshWithEffectToDraw();
		if(overlapEndActor->show) overlapEndActor->SubmitMeshWithEffectToDraw();

		controlledActor->SubmitMeshWithEffectToDraw();

		hitTestActor->SubmitMeshWithEffectToDraw();
	}
}

void eae6320::cSpaceInvader::UpdateBasedOnInput()
{
	static bool F1Pressed = false;
	static bool F2Pressed = false;
	static bool F3Pressed = false;
	static bool WPressed = false;
	static bool SPressed = false;
	static bool APressed = false;
	static bool DPressed = false;
	static bool IPressed = false;
	static bool KPressed = false;
	static bool JPressed = false;
	static bool LPressed = false;
	static bool UpArrowPressed = false;
	static bool DownArrowPressed = false;
	static bool LeftArrowPressed = false;
	static bool RightArrowPressed = false;
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}

	// If the user press space, slow the time
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		//slow the time
		SetTimeRate(0.2f);
	}

	// If the user release space, restore the time
	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		//restore the time
		SetTimeRate(1.0f);
	}

	// If the user press F1, show/hide specific mesh
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::F1) && !F1Pressed)
	{
		isShow = !isShow;
		F1Pressed = true;
	}

	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::F1) && F1Pressed)
	{
		F1Pressed = false;
	}

	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::F2) && F2Pressed)
	{
		F2Pressed = false;
	}

	//Actor Movement
	{
		//Up
		if (UserInput::IsKeyPressed('W') && !WPressed)
		{
			controlledActor->rigidBodyState->velocity.y += 1.5f;
			WPressed = true;
			laserAudio->PlayIndependent();
		}
		if (!UserInput::IsKeyPressed('W') && WPressed)
		{
			controlledActor->rigidBodyState->velocity.y -= 1.5f;
			WPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed('S') && !SPressed)
		{
			controlledActor->rigidBodyState->velocity.y -= 1.5f;
			SPressed = true;
			laserAudio->PlayIndependent();
		}
		if (!UserInput::IsKeyPressed('S') && SPressed)
		{
			controlledActor->rigidBodyState->velocity.y += 1.5f;
			SPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed('D') && !DPressed)
		{
			controlledActor->rigidBodyState->velocity.x += 1.5f;
			DPressed = true;
			laserAudio->PlayIndependent();
		}
		if (!UserInput::IsKeyPressed('D') && DPressed)
		{
			controlledActor->rigidBodyState->velocity.x -= 1.5f;
			DPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed('A') && !APressed)
		{
			controlledActor->rigidBodyState->velocity.x -= 1.5f;
			APressed = true;
			laserAudio->PlayIndependent();
		}
		if (!UserInput::IsKeyPressed('A') && APressed)
		{
			controlledActor->rigidBodyState->velocity.x += 1.5f;
			APressed = false;
		}
	}

	//Actor Movement
	{
		//Up
		if (UserInput::IsKeyPressed('I') && !IPressed)
		{
			hitTestActor->rigidBodyState->velocity.y += 1.5f;
			IPressed = true;
		}
		if (!UserInput::IsKeyPressed('I') && IPressed)
		{
			hitTestActor->rigidBodyState->velocity.y -= 1.5f;
			IPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed('K') && !KPressed)
		{
			hitTestActor->rigidBodyState->velocity.y -= 1.5f;
			KPressed = true;
		}
		if (!UserInput::IsKeyPressed('K') && KPressed)
		{
			hitTestActor->rigidBodyState->velocity.y += 1.5f;
			KPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed('L') && !LPressed)
		{
			hitTestActor->rigidBodyState->velocity.x += 1.5f;
			LPressed = true;
		}
		if (!UserInput::IsKeyPressed('L') && LPressed)
		{
			hitTestActor->rigidBodyState->velocity.x -= 1.5f;
			LPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed('J') && !JPressed)
		{
			hitTestActor->rigidBodyState->velocity.x -= 1.5f;
			JPressed = true;
		}
		if (!UserInput::IsKeyPressed('J') && JPressed)
		{
			hitTestActor->rigidBodyState->velocity.x += 1.5f;
			JPressed = false;
		}
	}

	//Camera Movement
	{
		//Up
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && !UpArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.5f, 0));
			UpArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && UpArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.5f, 0));
			UpArrowPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && !DownArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.5f, 0));
			DownArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && DownArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.5f, 0));
			DownArrowPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && !RightArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.5f, 0, 0));
			RightArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && RightArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.5f, 0, 0));
			RightArrowPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && !LeftArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.5f, 0, 0));
			LeftArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && LeftArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.5f, 0, 0));
			LeftArrowPressed = false;
		}
	}
}

void eae6320::cSpaceInvader::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	controlledActor->Update(i_elapsedSecondCount_sinceLastUpdate);
	hitTestActor->Update(i_elapsedSecondCount_sinceLastUpdate);
	camera->Update(i_elapsedSecondCount_sinceLastUpdate);

	//Update Collision
	Collision::CollisionManager::GetCollisionManager()->Update();
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cSpaceInvader::Initialize()
{
	//background color init
	backgroundColor.r = 0.6f;
	backgroundColor.g = 0.1f;
	backgroundColor.b = 0.7f;
	//Graphics::CreateMesh(vertexData01, indexData01, 7, 9, mesh01);
	Graphics::CreateMesh("data/Meshes/Sphere.lua", mesh01);
	Graphics::CreateMesh("data/Meshes/cube.lua", mesh02);
	Graphics::CreateMesh("data/Meshes/cube.lua", mesh03);
	Graphics::CreateMesh("data/Meshes/cube.lua", mesh04);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedColor.shader", effect01);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader", effect02);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Green.shader", effect03);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Red.shader", effect04);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Oriange.shader", effect05);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Gray.shader", effect06);

	//Player Controller
	playerController = new eae6320::GameFramework::APlayerController();

	//Actor
	controlledActor = new eae6320::AControlledActor(mesh01, effect02);
	hitTestActor = new eae6320::AHitTestActor(mesh02, effect03);
	overlapBeginActor = new eae6320::AOverlapBeginTestActor(mesh03, effect04);
	overlapEndActor = new eae6320::AOverlapEndTestActor(mesh04, effect05);

	//Audio
	laserAudio = new eae6320::AudioSystem::cAudio();
	laserAudio->AudioConstructor("data/Audio/Laser.mp3", "Laser", 500, false);
	laserAudio->SubmitAudioToBePlayed();

	hitTestActor->SetPosition(Math::sVector(-3.f, 3.f, -0.1f));
	overlapBeginActor->SetPosition(Math::sVector(0.f, 3.f, -0.1f));
	overlapEndActor->SetPosition(Math::sVector(3.f, 3.f, -0.1f));

	controlledActor->GetSphereComp()->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);

	//Set the extend of collision comp
	controlledActor->GetSphereComp()->SetRadius(0.5f);
	hitTestActor->GetBoxComp()->SetExtend(Math::sVector(0.5f, 0.5f, 0.5f));
	overlapBeginActor->GetBoxComp()->SetExtend(Math::sVector(0.5f, 0.5f, 0.5f));
	overlapEndActor->GetBoxComp()->SetExtend(Math::sVector(0.5f, 0.5f, 0.5f));

	//Call Begin function of actors
	controlledActor->Begin();
	hitTestActor->Begin();
	overlapBeginActor->Begin();
	overlapEndActor->Begin();

	//Call CollisionManager::Begin to build BVH
	Collision::CollisionManager::GetCollisionManager()->Begin();

	//Camera
	camera = new eae6320::GameFramework::ACameraActor();
	camera->SetPosition(Math::sVector(0, 0, 15.f));


	playerController->SetCurrentCamera(camera);

	return Results::Success;
}

eae6320::cResult eae6320::cSpaceInvader::CleanUp()
{
	controlledActor->CleanUp();
	hitTestActor->CleanUp();
	overlapBeginActor->CleanUp();
	overlapEndActor->CleanUp();
	camera->CleanUp();
	delete controlledActor;
	delete hitTestActor;
	delete overlapBeginActor;
	delete overlapEndActor;
	delete camera;
	delete playerController;
	delete laserAudio;

	mesh01->DecrementReferenceCount();
	mesh02->DecrementReferenceCount();
	mesh03->DecrementReferenceCount();
	mesh04->DecrementReferenceCount();
	effect01->DecrementReferenceCount();
	effect02->DecrementReferenceCount();
	effect03->DecrementReferenceCount();
	effect04->DecrementReferenceCount();
	effect05->DecrementReferenceCount();
	effect06->DecrementReferenceCount();

	Collision::CollisionManager::GetCollisionManager()->Destroy();
	return Results::Success;
}


//input interaction
//-----------------
void eae6320::cSpaceInvader::SwitchShader()
{
	if (!isDiffShader) 
	{
		controlledActor->ChangeEffect(effect02);
	}
	else 
	{
		controlledActor->ChangeEffect(effect01);
	}
}

void eae6320::cSpaceInvader::SwitchMesh()
{
	if (isCubeMesh) 
	{
		controlledActor->ChangeMesh(mesh03);
	}
	else 
	{
		controlledActor->ChangeMesh(mesh01);
	}
}

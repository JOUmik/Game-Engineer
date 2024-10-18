// Includes
//=========

#include "cMyGame.h"

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

// Inherited Implementation
//=========================

// Run
//----
void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
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
		plain->SubmitMeshWithEffectToDraw(i_elapsedSecondCount_sinceLastSimulationUpdate);
		house->SubmitMeshWithEffectToDraw(i_elapsedSecondCount_sinceLastSimulationUpdate);

		if (isShow)
		{
			chimney->SubmitMeshWithEffectToDraw(i_elapsedSecondCount_sinceLastSimulationUpdate);
		}
	}
}

void eae6320::cMyGame::UpdateBasedOnInput()
{
	static bool F1Pressed = false;
	static bool F2Pressed = false;
	static bool F3Pressed = false;
	static bool WPressed = false;
	static bool SPressed = false;
	static bool APressed = false;
	static bool DPressed = false;
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

	// If the user press F2, change shader of specific mesh
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::F2) && !F2Pressed)
	{
		isDiffShader = !isDiffShader;
		SwitchShader();
		F2Pressed = true;
	}

	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::F2) && F2Pressed)
	{
		F2Pressed = false;
	}

	//Change Mesh
	{
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::F3) && !F3Pressed)
		{
			isCubeMesh = !isCubeMesh;
			SwitchMesh();
			F3Pressed = true;
		}

		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::F3) && F3Pressed)
		{
			F3Pressed = false;
		}
	}

	//Actor Movement
	{
		//Up
		if (UserInput::IsKeyPressed('W') && !WPressed)
		{
			house->rigidBodyState->velocity.y += 1.f;
			WPressed = true;
		}
		if (!UserInput::IsKeyPressed('W') && WPressed)
		{
			house->rigidBodyState->velocity.y -= 1.f;
			WPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed('S') && !SPressed)
		{
			house->rigidBodyState->velocity.y -= 1.f;
			SPressed = true;
		}
		if (!UserInput::IsKeyPressed('S') && SPressed)
		{
			house->rigidBodyState->velocity.y += 1.f;
			SPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed('D') && !DPressed)
		{
			house->rigidBodyState->velocity.x += 1.f;
			DPressed = true;
		}
		if (!UserInput::IsKeyPressed('D') && DPressed)
		{
			house->rigidBodyState->velocity.x -= 1.f;
			DPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed('A') && !APressed)
		{
			house->rigidBodyState->velocity.x -= 1.f;
			APressed = true;
		}
		if (!UserInput::IsKeyPressed('A') && APressed)
		{
			house->rigidBodyState->velocity.x += 1.f;
			APressed = false;
		}
	}

	//Camera Movement
	{
		//Up
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && !UpArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.f, 0));
			UpArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && UpArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.f, 0));
			UpArrowPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && !DownArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.f, 0));
			DownArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && DownArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.f, 0));
			DownArrowPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && !RightArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.f, 0, 0));
			RightArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && RightArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.f, 0, 0));
			RightArrowPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && !LeftArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.f, 0, 0));
			LeftArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && LeftArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.f, 0, 0));
			LeftArrowPressed = false;
		}
	}
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	house->rigidBodyState->Update(i_elapsedSecondCount_sinceLastUpdate);
	chimney->rigidBodyState->Update(i_elapsedSecondCount_sinceLastUpdate);
	camera->Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	//background color init
	backgroundColor.r = 0.6f;
	backgroundColor.g = 0.1f;
	backgroundColor.b = 0.7f;
	//Graphics::CreateMesh(vertexData01, indexData01, 7, 9, mesh01);
	Graphics::CreateMesh("data/Meshes/Tree.lua", mesh01);
	Graphics::CreateMesh("data/Meshes/Donut.lua", mesh02);
	Graphics::CreateMesh("data/Meshes/cube.lua", mesh03);
	Graphics::CreateMesh("data/Meshes/Ground.lua", mesh04);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedColor.shader", effect01);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader", effect02);

	//Player Controller
	playerController = new eae6320::GameFramework::APlayerController();

	//Actor
	house = new eae6320::GameFramework::AActor(mesh01, effect01);
	chimney = new eae6320::GameFramework::AActor(mesh02, effect02);
	plain = new eae6320::GameFramework::AActor(mesh04, effect02);

	//Camera
	camera = new eae6320::GameFramework::ACameraActor();
	camera->SetPosition(Math::sVector(0, 0, 6.f));


	playerController->SetCurrentCamera(camera);

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	house->CleanUp();
	chimney->CleanUp();
	plain->CleanUp();
	camera->CleanUp();
	delete house;
	delete chimney;
	delete camera;
	delete playerController;

	mesh01->DecrementReferenceCount();
	mesh02->DecrementReferenceCount();
	mesh03->DecrementReferenceCount();
	mesh04->DecrementReferenceCount();
	effect01->DecrementReferenceCount();
	effect02->DecrementReferenceCount();
	return Results::Success;
}


//input interaction
//-----------------
void eae6320::cMyGame::SwitchShader()
{
	if (!isDiffShader) 
	{
		house->ChangeEffect(effect02);
	}
	else 
	{
		house->ChangeEffect(effect01);
	}
}

void eae6320::cMyGame::SwitchMesh()
{
	if (isCubeMesh) 
	{
		house->ChangeMesh(mesh03);
	}
	else 
	{
		house->ChangeMesh(mesh01);
	}
}

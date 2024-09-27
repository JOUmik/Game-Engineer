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
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>

// Inherited Implementation
//=========================

// Run
//----

namespace 
{
	//mesh data
	//---------
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData01[] =
	{
		//right - handed
		{ -0.4f, -0.5f, 0.0f },
		{  0.4f,  0.3f, 0.0f },
		{  0.4f, -0.5f, 0.0f },
		{ -0.4f,  0.3f, 0.0f },
		{ -0.6f,  0.3f, 0.0f },
		{  0.0f,  0.6f, 0.0f },
		{  0.6f,  0.3f, 0.0f },
	};

	uint16_t indexData01[] =
	{
		0, 2, 1,
		0, 1, 3,
		4, 6, 5
	};

	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData02[] =
	{
		//right - handed
		{  0.3f,  0.45f, 0.0f },
		{  0.5f,  0.35f, 0.0f },
		{  0.5f,  0.6f, 0.0f },
		{  0.3f,  0.6f, 0.0f }
	};

	uint16_t indexData02[] =
	{
		0, 1, 2,
		0, 2, 3
	};
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	//animated background
	float simulateTime = static_cast<float>(GetElapsedSecondCount_simulation());
	float r = (std::cos(9.0f * simulateTime) * 0.1f) + 0.15f;
	float g = (std::sin(2.0f * simulateTime) * 0.1f) + 0.15f;
	float b = (-std::cos(5.0f * simulateTime) * 0.2f) + 0.25f;
	Graphics::UpdateBackgroundColor(r, g, b, backgroundColor.a);
	
	//Draw Actors
	{
		house->SubmitMeshWithEffectToDraw(i_elapsedSecondCount_sinceLastSimulationUpdate);

		if (isShow)
		{
			chimney->SubmitMeshWithEffectToDraw(i_elapsedSecondCount_sinceLastSimulationUpdate);
		}
	}

	// Camera
	{
		// Calculate Camera Data
		auto cameraToProjected = Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(eae6320::Math::ConvertDegreesToRadians(45.f), 1.0f, 0.1f, 50.0f);
		auto worldToCamera = Math::cMatrix_transformation::CreateWorldToCameraTransform(
			camera->PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate)
		);
		// Submit Camera Data
		Graphics::BindCameraData(cameraToProjected, worldToCamera);
	}
}

void eae6320::cMyGame::UpdateBasedOnInput()
{
	static bool F1Pressed = false;
	static bool F2Pressed = false;
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
			camera->velocity.y += 1.f;
			UpArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && UpArrowPressed)
		{
			camera->velocity.y -= 1.f;
			UpArrowPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && !DownArrowPressed)
		{
			camera->velocity.y -= 1.f;
			DownArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && DownArrowPressed)
		{
			camera->velocity.y += 1.f;
			DownArrowPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && !RightArrowPressed)
		{
			camera->velocity.x += 1.f;
			RightArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && RightArrowPressed)
		{
			camera->velocity.x -= 1.f;
			RightArrowPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && !LeftArrowPressed)
		{
			camera->velocity.x -= 1.f;
			LeftArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && LeftArrowPressed)
		{
			camera->velocity.x += 1.f;
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
	Graphics::CreateMesh(vertexData01, indexData01, 7, 9, mesh01);
	Graphics::CreateMesh(vertexData02, indexData02, 4, 6, mesh02);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedColor.shader", effect01);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader", effect02);

	house = new eae6320::GameFramework::AActor(mesh01, effect01);
	chimney = new eae6320::GameFramework::AActor(mesh02, effect02);

	//Camera
	camera = new eae6320::Physics::sRigidBodyState();
	camera->position = Math::sVector(0.f, 0.f, 6.f);

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	house->CleanUp();
	chimney->CleanUp();
	delete house;
	delete chimney;
	delete camera;

	mesh01->DecrementReferenceCount();
	mesh02->DecrementReferenceCount();
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

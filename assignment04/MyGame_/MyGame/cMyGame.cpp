// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/Effect.h>
#include <Engine/Graphics/Mesh.h>
#include <Engine/Graphics/VertexFormats.h>

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
	Graphics::UpdateBackgroundColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	
	Graphics::BindMeshWithEffect(mesh01, effect01);
	if (isDiffShader) 
	{
		Graphics::BindMeshWithEffect(mesh01, effect01);
	}
	else 
	{
		Graphics::BindMeshWithEffect(mesh01, effect02);
	}
	if (isShow) 
	{
		Graphics::BindMeshWithEffect(mesh02, effect02);
	}
	
}

void eae6320::cMyGame::UpdateBasedOnInput()
{
	static bool F1Pressed = false;
	static bool F2Pressed = false;
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
		F2Pressed = true;
	}

	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::F2) && F2Pressed)
	{
		F2Pressed = false;
	}
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

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	mesh01->DecrementReferenceCount();
	mesh02->DecrementReferenceCount();
	effect01->DecrementReferenceCount();
	effect02->DecrementReferenceCount();
	return Results::Success;
}

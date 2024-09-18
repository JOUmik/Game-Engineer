// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Graphics/Graphics.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Graphics::UpdateBackgroundColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
}

void eae6320::cMyGame::UpdateBasedOnInput()
{
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
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	//background color init
	backgroundColor.r = 0.6f;
	backgroundColor.g = 0.1f;
	backgroundColor.b = 0.7f;


	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	return Results::Success;
}

/*
	WinMain() is the entry point of a Windows program
	(instead of the familiar main() in a console program)
*/

// Includes
//=========

#include "cExampleGame.h"

// Entry Point
//============

int WINAPI WinMain( HINSTANCE i_thisInstanceOfTheApplication, HINSTANCE, char* i_commandLineArguments, int i_initialWindowDisplayState )
{
	const std::string vertexShaderPath = "data/Shaders/Vertex/standard.shader";
	const std::string fragmentShaderPath = "data/Shaders/Fragment/animatedColor.shader";
	return eae6320::Application::Run<eae6320::cExampleGame>( i_thisInstanceOfTheApplication, i_commandLineArguments, i_initialWindowDisplayState, vertexShaderPath, fragmentShaderPath);
}

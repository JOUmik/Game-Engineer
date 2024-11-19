/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

// Entry Point
//============

fragment_main
{
	// Output solid white
	o_color = vec4(
		// RGB (color)
		1.0, 0.98, 0.80,
		// Alpha (opacity)
		1.0 );
}
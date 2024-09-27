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
		0.0, 1.0, 1.0,
		// Alpha (opacity)
		1.0 );
	{
		o_color.r = (cos( 4.0 * g_elapsedSecondCount_simulationTime )  * 0.5 ) + 0.5;
		o_color.g = ( sin( 3.0 * g_elapsedSecondCount_simulationTime ) * 0.5 ) + 0.5;
		o_color.b = ( -cos( 2.0 * g_elapsedSecondCount_simulationTime ) * 0.5 ) + 0.5;
	}
}
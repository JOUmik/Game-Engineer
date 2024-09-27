/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif

//Types
//=====

#if defined( EAE6320_PLATFORM_D3D )

	#define mat4 float4x4
	#define mat3 float3x3
	#define mat2 float2x2
	#define vec4 float4
	#define vec3 float3
	#define vec2 float2
	#define gl_Position o_vertexPosition_projected
	
#endif

// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )

#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name

#endif
	
	
DeclareConstantBuffer(g_constantBuffer_frame, 0)
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For vec4 alignment
	vec2 g_padding;
};

//Functions
//=========

#if defined( EAE6320_PLATFORM_D3D )

	#define vertex_main \
	void main( \
		in const vec3 i_vertexPosition_local : POSITION, \
		out vec4 o_vertexPosition_projected : SV_POSITION \
	)

	#define fragment_main \
	void main( \
		in const vec4 i_fragmentPosition : SV_POSITION, \
		out vec4 o_color : SV_TARGET \
	)


#elif defined( EAE6320_PLATFORM_GL )

	#define mul(i_matrix, i_vector) i_matrix * i_vector

	#define vertex_main \
	layout( location = 0 ) in vec3 i_vertexPosition_local; \
	void main()\

	#define fragment_main \
	out vec4 o_color; \
	void main()\


#endif

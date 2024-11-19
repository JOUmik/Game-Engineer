/*
	This class is your specific game
*/

#ifndef EAE6320_CEXAMPLEGAME_H
#define EAE6320_CEXAMPLEGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>


#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif

// Class Declaration
//==================

namespace eae6320
{
	namespace GameFramework
	{
		class AActor;
		class ACameraActor;
		class APlayerController;
	}
	namespace Physics
	{
		struct sRigidBodyState;
	}

	struct sColor
	{
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 1.f;
	};

	namespace Graphics 
	{
		class Mesh;
		class Effect;
	}

	class AControlledActor;
	class AHitTestActor;
	class AOverlapBeginTestActor;
	class AOverlapEndTestActor;
	class cMyGame final : public Application::iApplication
	{
	public:
		sColor backgroundColor;
		Graphics::Mesh* mesh01 = nullptr;
		Graphics::Mesh* mesh02 = nullptr;
		Graphics::Mesh* mesh03 = nullptr;
		Graphics::Mesh* mesh04 = nullptr;
		Graphics::Effect* effect01 = nullptr;
		Graphics::Effect* effect02 = nullptr;
		Graphics::Effect* effect03 = nullptr;
		Graphics::Effect* effect04 = nullptr;
		Graphics::Effect* effect05 = nullptr;
		Graphics::Effect* effect06 = nullptr;

		//Player Controller
		eae6320::GameFramework::APlayerController* playerController = nullptr;

		//Actors
		eae6320::AControlledActor* controlledActor = nullptr;
		eae6320::AHitTestActor* hitTestActor = nullptr;
		eae6320::AOverlapBeginTestActor* overlapBeginActor = nullptr;
		eae6320::AOverlapEndTestActor* overlapEndActor = nullptr;

		//Cameras
		eae6320::GameFramework::ACameraActor* camera = nullptr;

		//Input Control
		bool isShow = true;
		bool isDiffShader = true;
		bool isCubeMesh = false;

		// Inherited Implementation
		//=========================

	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "Yan's Game"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
			;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "Yan's Game Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_VSDEFAULT_LARGE; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_VSDEFAULT_SMALL; return &iconId_small; }
#endif

		// Run
		//----

		void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate) final;
		void UpdateBasedOnInput() final;
		void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) final;

		// Initialize / Clean Up
		//----------------------

		cResult Initialize() final;
		cResult CleanUp() final;

		//input interaction
		void SwitchShader();
		void SwitchMesh();
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CEXAMPLEGAME_H

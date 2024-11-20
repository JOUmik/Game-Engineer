/*
	This class is your specific game
*/

#ifndef EAE6320_CEXAMPLEGAME_H
#define EAE6320_CEXAMPLEGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>
#include <vector>


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
	namespace AudioSystem 
	{
		class cAudio;
	}

	namespace Graphics 
	{
		class Mesh;
		class Effect;
	}

	class AControlledActor;
	class ALaserBullet;
	class AEnemy;
	class ABlock;
	class AGameOverBlock;
	class cSpaceInvader final : public Application::iApplication
	{
	public:
		Graphics::Mesh* playerMesh = nullptr;
		Graphics::Mesh* enemyMesh = nullptr;
		Graphics::Mesh* blockMesh = nullptr;
		Graphics::Mesh* gameOverBlockMesh = nullptr;
		Graphics::Mesh* bulletMesh = nullptr;
		Graphics::Effect* darkEffect = nullptr;
		Graphics::Effect* playerEffect = nullptr;
		Graphics::Effect* greenEffect = nullptr;
		Graphics::Effect* grayEffect = nullptr;
		Graphics::Effect* bulletEffect = nullptr;
		AudioSystem::cAudio* laserAudio = nullptr;
		AudioSystem::cAudio* ExplosionAudio = nullptr;
		AudioSystem::cAudio* winAudio = nullptr;
		AudioSystem::cAudio* failAudio = nullptr;
		AudioSystem::cAudio* backgroundAudio = nullptr;

		//Player Controller
		GameFramework::APlayerController* playerController = nullptr;

		//Actors
		AControlledActor* controlledActor = nullptr;
		ABlock* leftBlock = nullptr;
		ABlock* rightBlock = nullptr;
		AGameOverBlock* gameOverBlock = nullptr;

		//Cameras
		GameFramework::ACameraActor* camera = nullptr;

	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "Yan's Space Invader Game"
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
		const char* GetMainWindowClassName() const final { return "Yan's Space Invader Game Window Class"; }
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

		//Game Play
		void Reset();
		void EnemyMovement(const float i_elapsedSecondCount_sinceLastUpdate);
		void SpawnBullet();
		void CreateEnemies();
		void EnemyDestroyed();
		void EnemyOverlapWithBlock();
		void EnemyOverlapWithGameOverBlock();

		std::vector<ALaserBullet*> bulletSet;
		float bulletSpawnGap = 0.6f;
		float currentSpawnGap = 0.6f;

		std::vector<AEnemy*> enemySet;

		bool bFirstFrame = true;
		bool EnemyHasOverlapedWithBlock = false;
		bool bEnemyMoveForward = false;
		float currentMoveForwardTime = 0.f;
		float maxMoveForwardTime = 0.7f;
		bool bIsMovingLeft = true;
		bool GameOver = false;
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

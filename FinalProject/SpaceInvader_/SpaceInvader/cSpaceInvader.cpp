// Includes
//=========

#include "cSpaceInvader.h"
#include "Actors/Enemy.h"
#include "Actors/ControlledActor.h"
#include "Actors/LaserBullet.h"
#include "Actors/Block.h"
#include "Actors/GameOverBlock.h"

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
#include <Engine/Collision/CollisionManager.h>
#include <Engine/Audio/cAudioSystem.h>

// Inherited Implementation
//=========================

// Run
//----
void eae6320::cSpaceInvader::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	//animated background
	float simulateTime = static_cast<float>(GetElapsedSecondCount_simulation());
	//float r = (std::cos(9.0f * simulateTime) * 0.1f) + 0.15f;
	//float g = (std::sin(2.0f * simulateTime) * 0.1f) + 0.15f;
	//float b = (-std::cos(5.0f * simulateTime) * 0.2f) + 0.25f;
	float r = 0.6f;
	float g = 0.8f;
	float b = 1.f;
	Graphics::UpdateBackgroundColor(r, g, b, backgroundColor.a);
	
	// Player Controller send the binded camera date to graphics
	{
		playerController->SubmitDataToGraphics(i_elapsedSecondCount_sinceLastSimulationUpdate);
	}

	//Draw Actors
	{
		if (!GameOver) 
		{
			controlledActor->Draw(i_elapsedSecondCount_sinceLastSimulationUpdate);

			for (auto enemy : enemySet)
			{
				if (enemy->bIsShow)
				{
					enemy->Draw(i_elapsedSecondCount_sinceLastSimulationUpdate);
				}
			}

			for (auto bullet : bulletSet)
			{
				if (bullet->bIsShow)
				{
					bullet->Draw(i_elapsedSecondCount_sinceLastSimulationUpdate);
				}

			}
		}

		if (GameOver) 
		{
			controlledActor->Draw();

			for (auto enemy : enemySet)
			{
				if (enemy->bIsShow)
				{
					enemy->Draw();
				}
			}

			for (auto bullet : bulletSet)
			{
				if (bullet->bIsShow)
				{
					bullet->Draw();
				}

			}
		}
	}

	//Debug
	{
		//leftBlock->Draw();
		//rightBlock->Draw();
		gameOverBlock->Draw();
	}
}

void eae6320::cSpaceInvader::UpdateBasedOnInput()
{
	static bool SpacePressed = false;
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

	// If the user press Space, spawn bullet
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space) && !SpacePressed)
	{
		if (currentSpawnGap >= bulletSpawnGap) 
		{
			SpawnBullet();
			laserAudio->PlayIndependent();
			currentSpawnGap = 0.f;
		}
		
		SpacePressed = true;
		
	}
	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Space) && SpacePressed)
	{
		SpacePressed = false;
	}


	//Actor Movement
	{
		////Up
		//if (UserInput::IsKeyPressed('W') && !WPressed)
		//{
		//	controlledActor->rigidBodyState->velocity.y += 2.5f;
		//	WPressed = true;
		//}
		//if (!UserInput::IsKeyPressed('W') && WPressed)
		//{
		//	controlledActor->rigidBodyState->velocity.y -= 2.5f;
		//	WPressed = false;
		//}

		////Down
		//if (UserInput::IsKeyPressed('S') && !SPressed)
		//{
		//	controlledActor->rigidBodyState->velocity.y -= 2.5f;
		//	SPressed = true;
		//}
		//if (!UserInput::IsKeyPressed('S') && SPressed)
		//{
		//	controlledActor->rigidBodyState->velocity.y += 2.5f;
		//	SPressed = false;
		//}

		//Right
		if (UserInput::IsKeyPressed('D') && !DPressed)
		{
			controlledActor->rigidBodyState->velocity.x += 3.f;
			DPressed = true;
		}
		if (!UserInput::IsKeyPressed('D') && DPressed)
		{
			controlledActor->rigidBodyState->velocity.x -= 3.f;
			DPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed('A') && !APressed)
		{
			controlledActor->rigidBodyState->velocity.x -= 3.f;
			APressed = true;
		}
		if (!UserInput::IsKeyPressed('A') && APressed)
		{
			controlledActor->rigidBodyState->velocity.x += 3.f;
			APressed = false;
		}
	}

	//Camera Movement
	//{
	//	//Up
	//	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && !UpArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.5f, 0));
	//		UpArrowPressed = true;
	//	}
	//	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && UpArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.5f, 0));
	//		UpArrowPressed = false;
	//	}

	//	//Down
	//	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && !DownArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.5f, 0));
	//		DownArrowPressed = true;
	//	}
	//	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && DownArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.5f, 0));
	//		DownArrowPressed = false;
	//	}

	//	//Right
	//	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && !RightArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.5f, 0, 0));
	//		RightArrowPressed = true;
	//	}
	//	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && RightArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.5f, 0, 0));
	//		RightArrowPressed = false;
	//	}

	//	//Left
	//	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && !LeftArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.5f, 0, 0));
	//		LeftArrowPressed = true;
	//	}
	//	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && LeftArrowPressed)
	//	{
	//		camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.5f, 0, 0));
	//		LeftArrowPressed = false;
	//	}
	//}
}

void eae6320::cSpaceInvader::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	EnemyMovement(i_elapsedSecondCount_sinceLastUpdate);

	if (GameOver) return;
	currentSpawnGap += i_elapsedSecondCount_sinceLastUpdate;
	controlledActor->Update(i_elapsedSecondCount_sinceLastUpdate);
	camera->Update(i_elapsedSecondCount_sinceLastUpdate);
	bool allEnemyDied = true;
	for (auto enemy : enemySet) 
	{
		if (enemy->bIsShow) 
		{
			allEnemyDied = false;
			enemy->Update(i_elapsedSecondCount_sinceLastUpdate);
		}
	}
	if (allEnemyDied) 
	{
		gameOverBlock->ChangeEffect(greenEffect);
		GameOver = true;
	}
	for (auto bullet : bulletSet) 
	{
		if (bullet->bIsShow) 
		{
			bullet->Update(i_elapsedSecondCount_sinceLastUpdate);
		}
	}

	//Update Collision
	Collision::CollisionManager::GetCollisionManager()->Update();
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cSpaceInvader::Initialize()
{
	//background color init
	backgroundColor.r = 0.6f;
	backgroundColor.g = 0.1f;
	backgroundColor.b = 0.7f;
	//Graphics::CreateMesh(vertexData01, indexData01, 7, 9, mesh01);
	Graphics::CreateMesh("data/Meshes/Player.lua", playerMesh);
	Graphics::CreateMesh("data/Meshes/Enemy.lua", enemyMesh);
	Graphics::CreateMesh("data/Meshes/Block.lua", blockMesh);
	Graphics::CreateMesh("data/Meshes/GameOverBlock.lua", gameOverBlockMesh);
	Graphics::CreateMesh("data/Meshes/LaserBullet.lua", bulletMesh);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Dark.shader", darkEffect);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/PlayerShader.shader", playerEffect);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Green.shader", greenEffect);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Gray.shader", grayEffect);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/BulletShader.shader", bulletEffect);

	//Player Controller
	playerController = new GameFramework::APlayerController();

	//Actor
	controlledActor = new AControlledActor(playerMesh, playerEffect);
	leftBlock = new ABlock(blockMesh, greenEffect);
	rightBlock = new ABlock(blockMesh, greenEffect);
	gameOverBlock = new AGameOverBlock(gameOverBlockMesh, darkEffect);
	CreateEnemies();

	//Audio
	laserAudio = new AudioSystem::cAudio();
	laserAudio->AudioConstructor("data/Audio/Laser.mp3", "Laser", 400, false);
	laserAudio->SubmitAudioToBePlayed();
	ExplosionAudio = new AudioSystem::cAudio();
	ExplosionAudio->AudioConstructor("data/Audio/Explosion.mp3", "Explosion", 2400, false);
	ExplosionAudio->SubmitAudioToBePlayed();

	controlledActor->SetPosition(Math::sVector(0.f, -4.f, 0.f));
	leftBlock->SetPosition(Math::sVector(-6.3f, 7.f, 0.f));
	rightBlock->SetPosition(Math::sVector(6.3f, 7.f, 0.f));
	gameOverBlock->SetPosition(Math::sVector(0.f, -3.4f, 0.f));

	//Set the extend of collision comp
	controlledActor->GetSphereComp()->SetRadius(0.4f);
	leftBlock->GetBoxComp()->SetExtend(Math::sVector(0.5f, 10.f, 0.5f));
	rightBlock->GetBoxComp()->SetExtend(Math::sVector(0.5f, 10.f, 0.5f));
	gameOverBlock->GetBoxComp()->SetExtend(Math::sVector(8.f, 0.02f, 0.1f));

	//Bind Event
	leftBlock->EnemyOverlapWithBlock.Add(this, &cSpaceInvader::EnemyOverlapWithBlock);
	rightBlock->EnemyOverlapWithBlock.Add(this, &cSpaceInvader::EnemyOverlapWithBlock);
	gameOverBlock->EnemyOverlapWithGameOverBlock.Add(this, &cSpaceInvader::EnemyOverlapWithGameOverBlock);

	//Call Begin function of actors
	controlledActor->Begin();
	leftBlock->Begin();
	rightBlock->Begin();
	gameOverBlock->Begin();

	//Call CollisionManager::Begin to build BVH
	Collision::CollisionManager::GetCollisionManager()->Begin();

	//Camera
	camera = new GameFramework::ACameraActor();
	camera->SetPosition(Math::sVector(0, 0, 15.f));


	playerController->SetCurrentCamera(camera);

	return Results::Success;
}

eae6320::cResult eae6320::cSpaceInvader::CleanUp()
{
	controlledActor->CleanUp();
	leftBlock->CleanUp();
	rightBlock->CleanUp();
	camera->CleanUp();
	gameOverBlock->CleanUp();
	delete controlledActor;
	delete camera;
	delete playerController;
	delete laserAudio;
	delete leftBlock;
	delete rightBlock;
	delete gameOverBlock;
	for (auto enemy : enemySet) 
	{
		enemy->CleanUp();
		delete enemy;
	}
	for (auto bullet : bulletSet) 
	{
		bullet->CleanUp();
		delete bullet;
	}
	bulletSet.clear();

	playerMesh->DecrementReferenceCount();
	enemyMesh->DecrementReferenceCount();
	blockMesh->DecrementReferenceCount();
	gameOverBlockMesh->DecrementReferenceCount();
	bulletMesh->DecrementReferenceCount();
	darkEffect->DecrementReferenceCount();
	playerEffect->DecrementReferenceCount();
	greenEffect->DecrementReferenceCount();
	grayEffect->DecrementReferenceCount();
	bulletEffect->DecrementReferenceCount();

	Collision::CollisionManager::GetCollisionManager()->Destroy();
	return Results::Success;
}


//input interaction
//-----------------
void eae6320::cSpaceInvader::SwitchShader()
{
	if (!isDiffShader) 
	{
		controlledActor->ChangeEffect(playerEffect);
	}
	else 
	{
		controlledActor->ChangeEffect(darkEffect);
	}
}

void eae6320::cSpaceInvader::SwitchMesh()
{
	if (isCubeMesh) 
	{
		controlledActor->ChangeMesh(blockMesh);
	}
	else 
	{
		controlledActor->ChangeMesh(playerMesh);
	}
}

void eae6320::cSpaceInvader::EnemyMovement(const float i_elapsedSecondCount_sinceLastUpdate)
{
	if (EnemyHasOverlapedWithBlock) 
	{
		bEnemyMoveForward = true;
		EnemyHasOverlapedWithBlock = false;
		bIsMovingLeft = !bIsMovingLeft;
		for (auto enemy : enemySet)
		{
			if (enemy->bIsShow)
			{
				enemy->rigidBodyState->velocity.y = -0.6f;
				enemy->rigidBodyState->velocity.x = 0.f;
			}
		}
		
	}
	if (bEnemyMoveForward) 
	{
		currentMoveForwardTime += i_elapsedSecondCount_sinceLastUpdate;
	}
	if (currentMoveForwardTime >= maxMoveForwardTime) 
	{
		bEnemyMoveForward = false;
		currentMoveForwardTime = 0.f;
		for (auto enemy : enemySet) 
		{
			if (enemy->bIsShow) 
			{
				if (bIsMovingLeft) 
				{
					enemy->rigidBodyState->velocity.y = 0.f;
					enemy->rigidBodyState->velocity.x = -1.5f;
				}
				else 
				{
					enemy->rigidBodyState->velocity.y = 0.f;
					enemy->rigidBodyState->velocity.x = 1.5f;
				}
			}
		}
	}
}

void eae6320::cSpaceInvader::SpawnBullet()
{
	ALaserBullet* laserBullet = new ALaserBullet(bulletMesh, bulletEffect);
	Math::sVector pos = controlledActor->GetPosition();
	pos.y += 1.f;
	laserBullet->SetPosition(pos);
	laserBullet->GetBoxComp()->SetExtend(Math::sVector(0.05f, 0.16f, 0.05f));
	laserBullet->Begin();

	bulletSet.push_back(laserBullet);
}

void eae6320::cSpaceInvader::CreateEnemies()
{
	//01
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-4.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//02
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-3.f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//03
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-1.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//04
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(1.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//05
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(3.f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//06
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(4.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//07
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-4.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//08
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-3.f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//09
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-1.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//10
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(1.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//11
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(3.f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//12
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(4.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//13
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-3.f, 1.4f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//14
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(-1.5f, 1.4f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//15
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(1.5f, 1.4f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//16
	{
		AEnemy* enemy = new AEnemy(enemyMesh, grayEffect);
		enemy->SetPosition(Math::sVector(3.f, 1.4f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
}

void eae6320::cSpaceInvader::EnemyDestroyed()
{
	ExplosionAudio->PlayIndependent();
}

void eae6320::cSpaceInvader::EnemyOverlapWithBlock()
{
	EnemyHasOverlapedWithBlock = true;
}

void eae6320::cSpaceInvader::EnemyOverlapWithGameOverBlock()
{
	GameOver = true;
}

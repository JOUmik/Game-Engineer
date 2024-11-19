// Includes
//=========

#include "cSpaceInvader.h"
#include "Actors/Enemy.h"
#include "Actors/ControlledActor.h"
#include "Actors/LaserBullet.h"

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
	float r = 0.7f;
	float g = 0.7f;
	float b = 0.9f;
	Graphics::UpdateBackgroundColor(r, g, b, backgroundColor.a);
	
	// Player Controller send the binded camera date to graphics
	{
		playerController->SubmitDataToGraphics(i_elapsedSecondCount_sinceLastSimulationUpdate);
	}

	//Draw Actors
	{

		if (controlledActor->bHitEventGeneratedCurrentFrame)
		{
			controlledActor->Draw();
			controlledActor->bHitEventGeneratedCurrentFrame = false;
		}
		else
		{
			controlledActor->Draw(i_elapsedSecondCount_sinceLastSimulationUpdate);
		}

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
}

void eae6320::cSpaceInvader::UpdateBasedOnInput()
{
	static bool SpacePressed = false;
	static bool WPressed = false;
	static bool SPressed = false;
	static bool APressed = false;
	static bool DPressed = false;
	static bool IPressed = false;
	static bool KPressed = false;
	static bool JPressed = false;
	static bool LPressed = false;
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
		//Up
		if (UserInput::IsKeyPressed('W') && !WPressed)
		{
			controlledActor->rigidBodyState->velocity.y += 2.5f;
			WPressed = true;
		}
		if (!UserInput::IsKeyPressed('W') && WPressed)
		{
			controlledActor->rigidBodyState->velocity.y -= 2.5f;
			WPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed('S') && !SPressed)
		{
			controlledActor->rigidBodyState->velocity.y -= 2.5f;
			SPressed = true;
		}
		if (!UserInput::IsKeyPressed('S') && SPressed)
		{
			controlledActor->rigidBodyState->velocity.y += 2.5f;
			SPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed('D') && !DPressed)
		{
			controlledActor->rigidBodyState->velocity.x += 2.5f;
			DPressed = true;
		}
		if (!UserInput::IsKeyPressed('D') && DPressed)
		{
			controlledActor->rigidBodyState->velocity.x -= 2.5f;
			DPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed('A') && !APressed)
		{
			controlledActor->rigidBodyState->velocity.x -= 2.5f;
			APressed = true;
		}
		if (!UserInput::IsKeyPressed('A') && APressed)
		{
			controlledActor->rigidBodyState->velocity.x += 2.5f;
			APressed = false;
		}
	}

	//Camera Movement
	{
		//Up
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && !UpArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.5f, 0));
			UpArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Up) && UpArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.5f, 0));
			UpArrowPressed = false;
		}

		//Down
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && !DownArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(0, 1.5f, 0));
			DownArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Down) && DownArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(0, 1.5f, 0));
			DownArrowPressed = false;
		}

		//Right
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && !RightArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.5f, 0, 0));
			RightArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Right) && RightArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.5f, 0, 0));
			RightArrowPressed = false;
		}

		//Left
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && !LeftArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() - Math::sVector(1.5f, 0, 0));
			LeftArrowPressed = true;
		}
		if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Left) && LeftArrowPressed)
		{
			camera->SetVelocity(camera->GetVelocity() + Math::sVector(1.5f, 0, 0));
			LeftArrowPressed = false;
		}
	}
}

void eae6320::cSpaceInvader::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	if (GameOver) return;
	currentSpawnGap += i_elapsedSecondCount_sinceLastUpdate;
	controlledActor->Update(i_elapsedSecondCount_sinceLastUpdate);
	camera->Update(i_elapsedSecondCount_sinceLastUpdate);
	for (auto enemy : enemySet) 
	{
		if (enemy->bIsShow) 
		{
			enemy->Update(i_elapsedSecondCount_sinceLastUpdate);
		}
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
	Graphics::CreateMesh("data/Meshes/Player.lua", mesh01);
	Graphics::CreateMesh("data/Meshes/Enemy.lua", mesh02);
	Graphics::CreateMesh("data/Meshes/cube.lua", mesh03);
	Graphics::CreateMesh("data/Meshes/cube.lua", mesh04);
	Graphics::CreateMesh("data/Meshes/LaserBullet.lua", bulletMesh);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedColor.shader", effect01);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/PlayerShader.shader", effect02);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Green.shader", effect03);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Red.shader", effect04);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Oriange.shader", effect05);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/Gray.shader", effect06);
	Graphics::CreateEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/BulletShader.shader", bulletEffect);

	//Player Controller
	playerController = new GameFramework::APlayerController();

	//Actor
	controlledActor = new AControlledActor(mesh01, effect02);
	CreateEnemies();

	//Audio
	laserAudio = new AudioSystem::cAudio();
	laserAudio->AudioConstructor("data/Audio/Laser.mp3", "Laser", 400, false);
	laserAudio->SubmitAudioToBePlayed();
	ExplosionAudio = new AudioSystem::cAudio();
	ExplosionAudio->AudioConstructor("data/Audio/Explosion.mp3", "Explosion", 2400, false);
	ExplosionAudio->SubmitAudioToBePlayed();

	controlledActor->SetPosition(Math::sVector(0.f, -4.f, 0.f));

	controlledActor->GetSphereComp()->SetCollisionComponentType(Collision::CollisionComponentType::Dynamic);

	//Set the extend of collision comp
	controlledActor->GetSphereComp()->SetRadius(0.5f);

	//Call Begin function of actors
	controlledActor->Begin();

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
	camera->CleanUp();
	delete controlledActor;
	delete camera;
	delete playerController;
	delete laserAudio;
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

	mesh01->DecrementReferenceCount();
	mesh02->DecrementReferenceCount();
	mesh03->DecrementReferenceCount();
	mesh04->DecrementReferenceCount();
	bulletMesh->DecrementReferenceCount();
	effect01->DecrementReferenceCount();
	effect02->DecrementReferenceCount();
	effect03->DecrementReferenceCount();
	effect04->DecrementReferenceCount();
	effect05->DecrementReferenceCount();
	effect06->DecrementReferenceCount();
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
		controlledActor->ChangeEffect(effect02);
	}
	else 
	{
		controlledActor->ChangeEffect(effect01);
	}
}

void eae6320::cSpaceInvader::SwitchMesh()
{
	if (isCubeMesh) 
	{
		controlledActor->ChangeMesh(mesh03);
	}
	else 
	{
		controlledActor->ChangeMesh(mesh01);
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
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-4.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//02
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-3.f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//03
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-1.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//04
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(1.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//05
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(3.f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//06
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(4.5f, 3.f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//07
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-4.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//08
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-3.f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//09
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-1.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//10
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(1.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//11
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(3.f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//12
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(4.5f, 4.6f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//13
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-3.f, 1.4f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//14
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(-1.5f, 1.4f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//15
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
		enemy->SetPosition(Math::sVector(1.5f, 1.4f, -0.1f));
		enemy->GetBoxComp()->SetExtend(Math::sVector(0.3f, 0.4f, 0.2f));
		enemy->Begin();
		enemy->Destoryed.Add(this, &cSpaceInvader::EnemyDestroyed);
		enemySet.push_back(enemy);
	}
	//16
	{
		AEnemy* enemy = new AEnemy(mesh02, effect06);
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

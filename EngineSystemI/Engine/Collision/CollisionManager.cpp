#include "CollisionManager.h"

eae6320::Collision::CollisionManager::CollisionManager(){}


eae6320::Collision::CollisionManager::~CollisionManager(){}

void eae6320::Collision::CollisionManager::CheckAndBroadcast_OnHit(BaseCollisionComponent& compA, BaseCollisionComponent& compB)
{
	CollisionPair pair{ &compA, &compB };

	// check whether the collision between compA and compB has been handled before
	if (collisionCache.find(pair) == collisionCache.end()) {
		// if not, insert to collisionCache to avoid repeating handle
		collisionCache.insert(pair);

		//broadcast
		compA.OnHit(compB);
		compB.OnHit(compA);
	}
}

void eae6320::Collision::CollisionManager::ClearCache()
{
	collisionCache.clear();
}

eae6320::Collision::CollisionManager* eae6320::Collision::CollisionManager::GetCollisionManager()
{
	if (collisionManager == nullptr) 
	{
		collisionManager = new CollisionManager();
	}
	return collisionManager;
}

void eae6320::Collision::CollisionManager::Update()
{
	for(auto collisionComponent : collisionComponentSet)
	{
		collisionComponent.CheckCollision();
	}

	ClearCache();
}

void eae6320::Collision::CollisionManager::Destroy()
{
	delete collisionManager;
}

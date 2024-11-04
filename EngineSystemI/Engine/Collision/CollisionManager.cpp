#include "CollisionManager.h"

eae6320::Collision::CollisionManager::CollisionManager(){}


eae6320::Collision::CollisionManager::~CollisionManager(){}

void eae6320::Collision::CollisionManager::AddCollisionComponent(BaseCollisionComponent& comp)
{
	collisionComponentSet.insert(comp);
}

void eae6320::Collision::CollisionManager::CheckAndBroadcast_OnHit(BaseCollisionComponent& compA, BaseCollisionComponent& compB)
{
	CollisionPair pair{ &compA, &compB };

	// check whether the hit between compA and compB has been handled before
	if (hitCache.find(pair) == hitCache.end())
	{
		// if not, insert to hitCache to avoid repeating handle
		hitCache.insert(pair);

		compA.OnHit(compB);
		compB.OnHit(compA);
	}
}

void eae6320::Collision::CollisionManager::CheckAndBroadcast_OnBeginOverlap(BaseCollisionComponent& compA, BaseCollisionComponent& compB)
{
	CollisionPair pair{ &compA, &compB };

	if (overlapBeginCache.find(pair) == overlapBeginCache.end())
	{
		overlapBeginCache.insert(pair);
		currentOverlaps.insert(pair);
		// if the pair not in the previousOverlaps, it means they have just overlapped with each other
		if (previousOverlaps.find(pair) == previousOverlaps.end())
		{
			compA.OnBeginOverlap(compB);
			compB.OnBeginOverlap(compA);
		}
	}
}

void eae6320::Collision::CollisionManager::CheckAndBroadcast_OnEndOverlap(BaseCollisionComponent& compA, BaseCollisionComponent& compB)
{
	CollisionPair pair{ &compA, &compB };

	if (overlapEndCache.find(pair) == overlapBeginCache.end())
	{
		overlapEndCache.insert(pair);

		// if the pair was in the currentOverlaps or previousOverlaps, it means they have just finished overlapping with each other
		if (currentOverlaps.erase(pair) || previousOverlaps.find(pair) != previousOverlaps.end())
		{
			compA.OnEndOverlap(compB);
			compB.OnEndOverlap(compA);
		}
	}
}

void eae6320::Collision::CollisionManager::PerformMovementAndHitDetection()
{
}

void eae6320::Collision::CollisionManager::PerformOverlapDetection()
{
}

void eae6320::Collision::CollisionManager::ClearCache()
{
	hitCache.clear();
	overlapBeginCache.clear();
	overlapEndCache.clear();
	currentOverlaps.clear();
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
	for (auto& comp : collisionComponentSet) 
	{
		// only update the dynamic components that are moving in this update
		if (comp.GetCollisionComponentType() != CollisionComponentType::Dynamic || !comp.bIsMoving) continue;

		Math::sVector startPosition = comp.GetPosition();
		Math::sVector endPosition = comp.GetTargetPosition();

		bool hitOccurred = false;

		float step = 0.1f;
		for (float t = 0; t <= 1; t += step)
		{
			Math::sVector interpolatedPosition = Lerp(startPosition, endPosition, t);
			comp.SetPosition(interpolatedPosition);

			for (auto& otherComp : collisionComponentSet) 
			{
				if (&comp == &otherComp) continue;

				bool isColliding = comp.DetectCollision(otherComp);

				// Handle hit event
				if (comp.IsHitEventEnabled() && otherComp.IsHitEventEnabled() && isColliding) 
				{
					CollisionPair pair{ &comp, &otherComp };
					hitOccurred = true;

					// prevent repeated handle
					CheckAndBroadcast_OnHit(comp, otherComp);
					break;
				}

				// Handle overlap event	
				// to generate overlap event, the collision event relation should be overlap -- overlap, overlap -- hit, or hit -- overlap
				if (comp.IsOverlapEventEnabled() && otherComp.IsOverlapEventEnabled() && (!comp.IsHitEventEnabled() || !otherComp.IsHitEventEnabled())) 
				{
					if (isColliding)
					{
						CheckAndBroadcast_OnBeginOverlap(comp, otherComp);
						
					}
					else
					{
						CheckAndBroadcast_OnEndOverlap(comp, otherComp);
						
					}
				}
			}

			if (hitOccurred) 
			{
				Math::sVector safePosition = PerformBinarySearch(startPosition, endPosition, std::max(t - step, 0.f), t, comp);
				comp.SetPosition(safePosition);
				//Update and synchronize location to the actor the collision component bind with
				comp.UpdateActorPostion(safePosition);
				break;
			}
		}

		if (!hitOccurred) 
		{
			//Update and synchronize location to the actor the collision component bind with
			comp.UpdateActorPostion(endPosition);
		}
		
		// comp's movement has been updated
		comp.bIsMoving = false;
	}

	// update overlap cache
	std::swap(currentOverlaps, previousOverlaps);
	ClearCache();
}

void eae6320::Collision::CollisionManager::Destroy()
{
	delete collisionManager;
}

eae6320::Math::sVector eae6320::Collision::CollisionManager::Lerp(const Math::sVector& start, const Math::sVector& end, float t)
{
	return Math::sVector(
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t,
		start.z + (end.z - start.z) * t
	);
}

eae6320::Math::sVector eae6320::Collision::CollisionManager::PerformBinarySearch(const Math::sVector& start, const Math::sVector& end, float low, float high, BaseCollisionComponent& comp)
{
	const float tolerance = 0.01f;
	Math::sVector safePosition = start;

	while ((high - low) > tolerance) {
		float mid = (low + high) / 2.0f;
		Math::sVector midPosition = Lerp(start, end, mid);
		comp.SetPosition(midPosition);

		bool hasCollision = false;
		for (auto& otherComp : collisionComponentSet) {
			if (&comp == &otherComp) continue;

			if (comp.DetectCollision(otherComp)) {
				hasCollision = true;
				break;
			}
		}

		if (hasCollision) 
		{
			high = mid;
		}
		else 
		{
			low = mid;
			safePosition = midPosition;
		}
	}

	return safePosition;
}
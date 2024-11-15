#include "CollisionManager.h"
#include <algorithm>
#include <stack>

eae6320::Collision::CollisionManager* eae6320::Collision::CollisionManager::collisionManager = nullptr;

eae6320::Collision::CollisionManager::CollisionManager(){}


eae6320::Collision::CollisionManager::~CollisionManager(){}

void eae6320::Collision::CollisionManager::BuildStaticBVH()
{
	std::vector<BaseCollisionComponent*> staticComponents;
	for (const auto& comp : collisionComponentSet) 
	{
		if (comp->GetCollisionComponentType() == CollisionComponentType::Static) 
		{
			staticComponents.push_back(comp);
		}
	}
	staticBVH = BuildBVH(staticComponents, 0, static_cast<int>(staticComponents.size()));
}

void eae6320::Collision::CollisionManager::BuildDynamicBVH()
{
	std::vector<BaseCollisionComponent*> dynamicComponents;
	for (const auto& comp : collisionComponentSet) 
	{
		if (comp->GetCollisionComponentType() == CollisionComponentType::Dynamic) 
		{
			dynamicComponents.push_back(comp);
		}
	}
	dynamicBVH = BuildBVH(dynamicComponents, 0, static_cast<int>(dynamicComponents.size()));
}

std::unique_ptr<eae6320::Collision::BVHNode> eae6320::Collision::CollisionManager::BuildBVH(std::vector<BaseCollisionComponent*>& comps, int start, int end)
{
	if (start >= end) return nullptr;

	auto node = std::make_unique<BVHNode>();

	// calculate the union box of all components
	BoundingBox bounds = comps[start]->GetBoundingBox();
	for (int i = start + 1; i < end; ++i) {
		bounds = BoundingBox::Union(bounds, comps[i]->GetBoundingBox());
	}
	node->bounds = bounds;

	// if only one component, then the node is a leaf node
	if (end - start == 1) {
		node->components.push_back(comps[start]);
		return node;
	}

	// sort by the longest axis
	int axis = bounds.LongestAxis();
	std::sort(comps.begin() + start, comps.begin() + end, 
		[axis](BaseCollisionComponent* a, BaseCollisionComponent* b) 
		{
			if (axis == 0) 
			{
				return a->GetBoundingBox().Center().x < b->GetBoundingBox().Center().x;
			}
			else if (axis == 1) 
			{
				return a->GetBoundingBox().Center().y < b->GetBoundingBox().Center().y;
			}
			else 
			{
				return a->GetBoundingBox().Center().z < b->GetBoundingBox().Center().z;
			}
		});

	// divide left and right node
	int mid = (start + end) / 2;
	node->left = BuildBVH(comps, start, mid);
	node->right = BuildBVH(comps, mid, end);
	return node;
}

void eae6320::Collision::CollisionManager::DetectCollisionsWithBVH(BaseCollisionComponent* target, BVHNode* root, bool& hitOccurred)
{
	if (!root || !root->bounds.Intersects(target->GetBoundingBox())) return;

	std::stack<BVHNode*> stack;
	stack.push(root);

	while (!stack.empty()) 
	{
		BVHNode* node = stack.top();
		stack.pop();

		// If the bounding box of current node not intersect with component's boundingbox, continue
		if (!node->bounds.Intersects(target->GetBoundingBox())) 
		{
			continue;
		}

		// if node is leaf node, detect the hit event and Overlap Begin Event between two components
		if (node->IsLeaf()) 
		{
			for (auto& comp : node->components) 
			{
				if (comp == target) continue;

				bool isColliding = comp->DetectCollision(*target);

				// Handle Hit Event
				if (comp->IsHitEventEnabled() && target->IsHitEventEnabled() && isColliding)
				{
					CollisionPair pair{ comp, target };
					hitOccurred = true;

					// prevent repeated handle
					CheckAndBroadcast_OnHit(*comp, *target);
					break;
				}

				// Handle Overlap Begin Event	
				// to generate Overlap Event, the collision event relation should be overlap -- overlap, overlap -- hit, or hit -- overlap
				if (comp->IsOverlapEventEnabled() && target->IsOverlapEventEnabled() && (!comp->IsHitEventEnabled() || !target->IsHitEventEnabled()))
				{
					if (isColliding)
					{
						CheckAndBroadcast_OnBeginOverlap(*comp, *target);

					}
				}
			}
		}
		else 
		{
			// push the child node to the stack and continue
			if (node->left) stack.push(node->left.get());
			if (node->right) stack.push(node->right.get());
		}
	}
}

void eae6320::Collision::CollisionManager::AddCollisionComponent(BaseCollisionComponent& comp)
{
	collisionComponentSet.insert(&comp);
}

void eae6320::Collision::CollisionManager::RemoveCollisionComponent(BaseCollisionComponent& comp)
{
	collisionComponentSet.erase(&comp);
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

void eae6320::Collision::CollisionManager::CheckAndBroadcast_OnEndOverlap(BaseCollisionComponent& comp)
{
	for (auto& pair : previousOverlaps)
	{
		if (overlapEndCache.find(pair) == overlapEndCache.end() && (pair.componentA == &comp || pair.componentB == &comp))
		{
			overlapEndCache.insert(pair);

			// if the pair was in the previousOverlaps and not in the currentOverlaps, it means they have just finished overlapping with each other this update
			if (currentOverlaps.find(pair) == currentOverlaps.end())
			{
				pair.componentA->OnEndOverlap(*pair.componentB);
				pair.componentB->OnEndOverlap(*pair.componentA);
			}
		}
	}

	for (auto& pair : currentOverlaps) 
	{
		if (overlapEndCache.find(pair) == overlapEndCache.end() && (pair.componentA == &comp || pair.componentB == &comp))
		{
			overlapEndCache.insert(pair);

			bool isColliding = pair.componentA->DetectCollision(*pair.componentB);

			// if the pair is in the currentOverlaps but they are not overlapping, it means in this single update they were overlapped and left each other
			if (!isColliding) 
			{
				pair.componentA->OnEndOverlap(*pair.componentB);
				pair.componentB->OnEndOverlap(*pair.componentA);
			}
		}
	}
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

void eae6320::Collision::CollisionManager::Begin() 
{
	BuildStaticBVH();
	BuildDynamicBVH();
}

void eae6320::Collision::CollisionManager::Update()
{
	for (auto& comp : collisionComponentSet) 
	{
		// only update the dynamic components that are moving in this update
		if (comp->GetCollisionComponentType() != CollisionComponentType::Dynamic || !comp->bIsMoving) continue;

		Math::sVector startPosition = comp->GetPosition();
		Math::sVector endPosition = comp->GetTargetPosition();

		bool hitOccurred = false;

		float step = 0.1f;

		{	// set the step based on the distance
			float distance = (endPosition - startPosition).GetLength();

			if (distance <= 0.5f)
			{
				step = 0.5f;
			}
			else if (distance <= 2.f)
			{
				step = 0.2f;
			}
			else
			{
				step = 0.1f;
			}
		}
		for (float t = 0; t <= 1; t += step)
		{
			Math::sVector interpolatedPosition = Lerp(startPosition, endPosition, t);
			comp->SetPosition(interpolatedPosition);

			
			DetectCollisionsWithBVH(comp, staticBVH.get(), hitOccurred);
			DetectCollisionsWithBVH(comp, dynamicBVH.get(), hitOccurred);

			if (hitOccurred) 
			{
				Math::sVector safePosition = PerformBinarySearch(startPosition, endPosition, std::max(t - step, 0.f), t, *comp);
				comp->SetPosition(safePosition);
				// Update and synchronize location to the actor the collision component bind with
				comp->UpdateActorPostion(safePosition);
				break;
			}
		}

		if (!hitOccurred) 
		{
			// Update and synchronize location to the actor the collision component bind with
			comp->UpdateActorPostion(endPosition);
		}

		CheckAndBroadcast_OnEndOverlap(*comp);
		
		// comp's movement has been updated
		comp->bIsMoving = false;
	}

	// update overlap cache
	std::swap(currentOverlaps, previousOverlaps);

	// update dynamic BVH
	BuildDynamicBVH();
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
			if (&comp == otherComp) continue;

			if (comp.DetectCollision(*otherComp)) {
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
#include "Mesh.h"
#include "VertexFormats.h"
#include "cVertexFormat.h"
#include "sContext.h"

#include <Engine/Logging/Logging.h>
#include <Engine/Math/sVector.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Concurrency/cMutex.h>
#include <new>
#include <vector>

namespace
{
	// This exists for our class to help you catch reference counting problems.
	// In a real game there would be more sophisticated methods that would work for all asset types.
	class cMeshTracker
	{
		// Interface
		//----------

	public:

		// Access
		void AddMesh(const eae6320::Graphics::Mesh* const i_mesh);
		void RemoveMesh(const eae6320::Graphics::Mesh* const i_mesh);
		// Initializate / Clean Up
		cMeshTracker() = default;
		~cMeshTracker();

		// Data
		//-----

	private:

		std::vector<const eae6320::Graphics::Mesh*> m_meshes;
		eae6320::Concurrency::cMutex m_mutex;

		// Implementation
		//---------------

	private:

		// Initializate / Clean Up
		cMeshTracker(const cMeshTracker&) = delete;
		cMeshTracker(cMeshTracker&&) = delete;
		cMeshTracker& operator =(const cMeshTracker&) = delete;
		cMeshTracker& operator =(cMeshTracker&&) = delete;
	} s_meshTracker;
}

namespace eae6320
{
    namespace Graphics
    {
        cResult Mesh::Load(VertexFormats::sVertex_mesh*& vertexData, uint16_t*& indexData, unsigned int vertexCount, unsigned int indexCount, Mesh*& o_mesh)
        {
			auto result = Results::Success;

			Mesh* newMesh = nullptr;
			cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
				{
					if (result)
					{
						EAE6320_ASSERT(newMesh != nullptr);
						o_mesh = newMesh;
					}
					else
					{
						if (newMesh)
						{
							newMesh->DecrementReferenceCount();
							newMesh = nullptr;
						}
						o_mesh = nullptr;
					}
				});

			// Allocate a new effect
			{
				newMesh = new (std::nothrow) Mesh();
				if (!newMesh)
				{
					result = Results::OutOfMemory;
					EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
					Logging::OutputError("Failed to allocate memory for the mesh");
					return result;
				}
			}
			// Initialize the platform-specific graphics API shader object
			if (!(result = newMesh->InitializeGeometry(vertexData, indexData, vertexCount, indexCount)))
			{
				EAE6320_ASSERTF(false, "Initialization of new mesh failed");
				return result;
			}

			return result;
        }
        void Mesh::EnsureRightHandedIndexOrder(const VertexFormats::sVertex_mesh* vertexData, uint16_t* indexData, unsigned int indexCount)
        {
            // Iterate through each triangle (3 indices per triangle)
            for (unsigned int i = 0; i < indexCount; i += 3)
            {
                // Get the three vertices for the current triangle
                const VertexFormats::sVertex_mesh& v0 = vertexData[indexData[i]];
                const VertexFormats::sVertex_mesh& v1 = vertexData[indexData[i + 1]];
                const VertexFormats::sVertex_mesh& v2 = vertexData[indexData[i + 2]];

                // Compute two edge vectors of the triangle
                Math::sVector edge1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
                Math::sVector edge2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

                // Compute the normal using cross product
                Math::sVector normal = Math::Cross(edge1, edge2);

                // Check if the normal is pointing in the positive or negative Z direction
                if (normal.z < 0.0f)
                {
                    // The normal is pointing in the negative Z direction (indicating a left-handed winding order)
                    // Swap the second and third indices to correct the order (make it right-handed)
                    std::swap(indexData[i + 1], indexData[i + 2]);
                }
            }
        }

		Mesh::Mesh() 
		{
			s_meshTracker.AddMesh(this);
		}

		Mesh::~Mesh() 
		{
			EAE6320_ASSERT(m_referenceCount == 0);
			s_meshTracker.RemoveMesh(this);
			const auto result = CleanUp();
			EAE6320_ASSERT(result);
		}
    }
}

namespace
{
	// Interface
	//----------

	// Access

	void cMeshTracker::AddMesh(const eae6320::Graphics::Mesh* const i_mesh)
	{
		EAE6320_ASSERT(i_mesh != nullptr);
		eae6320::Concurrency::cMutex::cScopeLock scopeLock(m_mutex);
#ifdef EAE6320_ASSERTS_AREENABLED
		for (const auto* const mesh : m_meshes)
		{
			EAE6320_ASSERTF(mesh != i_mesh, "This mesh is already being tracked");
		}
#endif
		m_meshes.push_back(i_mesh);
	}

	void cMeshTracker::RemoveMesh(const eae6320::Graphics::Mesh* const i_mesh)
	{
		eae6320::Concurrency::cMutex::cScopeLock scopeLock(m_mutex);
		const auto meshCount = m_meshes.size();
		for (std::remove_const<decltype(meshCount)>::type i = 0; i < meshCount; ++i)
		{
			if (m_meshes[i] == i_mesh)
			{
				m_meshes.erase(m_meshes.begin() + i);
				return;
			}
		}
		EAE6320_ASSERTF(false, "This mesh wasn't being tracked");
	}

	// Initialize / Clean Up

	cMeshTracker::~cMeshTracker()
	{
		eae6320::Concurrency::cMutex::cScopeLock scopeLock(m_mutex);
		const auto haveAnyMeshesNotBeenDeleted = !m_meshes.empty();
		if (haveAnyMeshesNotBeenDeleted)
		{
			EAE6320_ASSERTF(!haveAnyMeshesNotBeenDeleted,
				"If you are seeing this message there are %i meshes that haven't been deleted. You probably have a reference counting bug!", m_meshes.size());
			eae6320::Logging::OutputError("Not every mesh was released. The program will now crash!");
			constexpr int* const invalidAddress = 0x0;
			*invalidAddress = 123;	// Intentional crash! You have a bug that must be fixed!
		}
	}
}
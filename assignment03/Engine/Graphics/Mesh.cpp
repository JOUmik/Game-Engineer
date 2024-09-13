#include "Mesh.h"
#include "VertexFormats.h"
#include "cVertexFormat.h"
#include "sContext.h"

#include <Engine/Logging/Logging.h>
#include <Engine/Math/sVector.h>

namespace eae6320
{
    namespace Graphics
    {
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
    }
}
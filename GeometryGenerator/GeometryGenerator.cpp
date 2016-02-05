#include "GeometryGenerator.h"

#include <Utils/Assert.h>

using namespace DirectX;

namespace GeometryGenerator {
	void CreateSphere(const XMFLOAT3& centerPos, const float radius, const unsigned int sliceCount, const unsigned int stackCount, std::vector<XMFLOAT3>& positions, std::vector<unsigned int>& indices) {
		ASSERT(radius > 0.0f);
		ASSERT(sliceCount > 0);
		ASSERT(stackCount > 1);

		const unsigned int indexOffset = static_cast<unsigned int> (positions.size());

		//positions.reserve(positions.size() + (stackCount - 1) * (sliceCount + 1) + 2);
		//indices.reserve(indices.size() + 6 * sliceCount + 6 * (sliceCount + 1 * stackCount - 2));

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		const XMFLOAT3 topVertex(centerPos.x, centerPos.y + radius, centerPos.z);
		const XMFLOAT3 bottomVertex(centerPos.x, centerPos.y - radius, centerPos.z);

		positions.push_back(topVertex);

		const float phiStep = XM_PI / stackCount;
		const float thetaStep = 2.0f * XM_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (unsigned int i = 1; i < stackCount; ++i) 		{
			const float phi = i * phiStep;
			const float sinPhi = sinf(phi);
			const float cosPhi = cosf(phi);

			// Vertices of ring.
			for (unsigned int j = 0; j <= sliceCount; ++j) {
				const float theta = j * thetaStep;

				XMFLOAT3 v = centerPos;

				// spherical to cartesian
				v.x += radius * sinPhi * cosf(theta);
				v.y += radius * cosPhi;
				v.z += radius * sinPhi * sinf(theta);
								
				positions.push_back(v);
			}
		}

		positions.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (unsigned int i = 1; i <= sliceCount; ++i) {
			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + i + 1);
			indices.push_back(indexOffset + i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		unsigned int baseIndex = indexOffset + 1;
		const unsigned int ringVertexCount = sliceCount + 1;
		for (unsigned int i = 0; i < stackCount - 2; ++i) {
			for (unsigned int j = 0; j <= sliceCount; ++j) {
				indices.push_back(indexOffset + i * ringVertexCount + j);
				indices.push_back(indexOffset + i * ringVertexCount + j + 1);
				indices.push_back(indexOffset + (i + 1) * ringVertexCount + j);

				indices.push_back(indexOffset + (i + 1) * ringVertexCount + j);
				indices.push_back(indexOffset + i * ringVertexCount + j + 1);
				indices.push_back(indexOffset + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		const unsigned int southPoleIndex = static_cast<unsigned int>(positions.size()) - 1 - indexOffset;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (unsigned int i = 0; i < sliceCount; ++i) {
			indices.push_back(baseIndex + southPoleIndex);
			indices.push_back(baseIndex + baseIndex + i);
			indices.push_back(baseIndex + baseIndex + i + 1);
		}
	}
}
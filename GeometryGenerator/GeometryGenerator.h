#pragma once

#include <DirectXMath.h>
#include <vector>

namespace GeometryGenerator {
	void CreateSphere(const DirectX::XMFLOAT3& centerPos, float radius, const unsigned int sliceCount, const unsigned int stackCount, std::vector<DirectX::XMFLOAT3>& positions, std::vector<unsigned int>& indices);
}

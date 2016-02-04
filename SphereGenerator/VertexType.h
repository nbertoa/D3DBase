#pragma once

#include <DirectXMath.h>

namespace SphereGenerator {
	struct VertexData {
		VertexData() {}
		VertexData(const DirectX::XMFLOAT3& posL)
			: mPosL(posL)
		{
		}

		DirectX::XMFLOAT3 mPosL;
	};
}

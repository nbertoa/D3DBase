#pragma once

#include <DirectXMath.h>

namespace InstancingTriangleDrawer {
	struct VertexData {
		VertexData() {}
		VertexData(const DirectX::XMFLOAT3& posL)
			: mPosL(posL)
		{
		}

		DirectX::XMFLOAT3 mPosL;
	};
}

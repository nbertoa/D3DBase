#pragma once

#include <DirectXMath.h>

namespace TriangleDrawer {
	struct VertexData {
		VertexData() {}
		VertexData(const DirectX::XMFLOAT3& posL)
			: mPosL(posL)
		{
		}

		DirectX::XMFLOAT3 mPosL;
	};
}

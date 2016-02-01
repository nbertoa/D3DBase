#pragma once

#include <DirectXMath.h>

#include <DrawManager/IDrawer.h>

#include "PixelShaderData.h"
#include "VertexShaderData.h"

struct ID3D11Device1;
struct ID3D11DeviceContext1;

//
// Draws triangles. You must set each vertex position
//

namespace TriangleDrawer {
	class Drawer : public IDrawer {
	public:
		void Draw(ID3D11Device1& device,
			ID3D11DeviceContext1& context,
			const DirectX::XMMATRIX& view,
			const DirectX::XMMATRIX& proj) override;

		VertexShaderData& GetVertexShaderData() { return mVertexShaderData; }
		PixelShaderData& GetPixelShaderData() { return mPixelShaderData; }
		DirectX::XMFLOAT4X4& World() { return mWorld; }

	private:
		VertexShaderData mVertexShaderData;
		PixelShaderData mPixelShaderData;
		DirectX::XMFLOAT4X4 mWorld;
	};
}
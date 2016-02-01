#pragma once

#include <DirectXMath.h>

#include <DrawManager/IDrawer.h>

#include "GeometryShaderData.h"
#include "PixelShaderData.h"
#include "VertexShaderData.h"

struct ID3D11Device1;
struct ID3D11DeviceContext1;

//
// Drawer that expands a vertex in a cube in geometry shader.
// Vertex only must have a positions
//

namespace CubeGenerator {
	class Drawer : public IDrawer {
	public:
		void Draw(ID3D11Device1& device,
			ID3D11DeviceContext1& context,
			const DirectX::XMMATRIX& view,
			const DirectX::XMMATRIX& proj) override;

		VertexShaderData& GetVertexShaderData() { return mVertexShaderData; }
		GeometryShaderData& GetGeometryShaderData() { return mGeometryShaderData; }
		PixelShaderData& GetPixelShaderData() { return mPixelShaderData; }
		DirectX::XMFLOAT4X4& World() { return mWorld; }

	private:
		VertexShaderData mVertexShaderData;
		GeometryShaderData mGeometryShaderData;
		PixelShaderData mPixelShaderData;
		DirectX::XMFLOAT4X4 mWorld;
	};
}
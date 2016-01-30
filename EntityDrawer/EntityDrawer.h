#pragma once

#include <DirectXMath.h>

#include "GeometryShaderData.h"
#include "PixelShaderData.h"
#include "VertexShaderData.h"

struct ID3D11Device1;
struct ID3D11DeviceContext1;

class EntityDrawer {
public:
	void Draw(ID3D11Device1& device,
			  ID3D11DeviceContext1& context,
			  const DirectX::XMMATRIX& view,
			  const DirectX::XMMATRIX& proj);

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
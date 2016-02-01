#pragma once

#include <DirectXMath.h>

struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct ID3D11PixelShader;

namespace InstancingTriangleDrawer {
	class PixelShaderData {
	public:
		PixelShaderData();

		void PreDraw(ID3D11Device1& device, ID3D11DeviceContext1& context);
		void PostDraw(ID3D11DeviceContext1& context);

	private:
		ID3D11PixelShader* mShader;
	};
}
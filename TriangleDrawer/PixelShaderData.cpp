#include "PixelShaderData.h"

#include <d3d11_1.h>
#include <sstream>

#include <ShadersManager/ShadersManager.h>
#include <Utils/Assert.h>

namespace {
	const char* shader = "..\\..\\content\\shaders\\TriangleDrawer\\PixelShader.cso";
}

namespace TriangleDrawer {
	PixelShaderData::PixelShaderData() 
		: mShader(nullptr)
	{
		ShadersManager::gInstance->LoadPixelShader(shader, &mShader);
		ASSERT(mShader);
	}

	void PixelShaderData::PreDraw(ID3D11Device1& /*device*/, ID3D11DeviceContext1& context) {
		ASSERT(mShader);
		context.PSSetShader(mShader, nullptr, 0);
	}

	void PixelShaderData::PostDraw(ID3D11DeviceContext1& context) {
		context.PSSetShader(nullptr, nullptr, 0);
	}
}

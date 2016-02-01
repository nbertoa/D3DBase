#include "PixelShaderData.h"

#include <d3d11_1.h>
#include <sstream>

#include <ShadersManager/ShadersManager.h>
#include <Utils/Assert.h>

namespace {
	const char* shader = "..\\..\\content\\shaders\\EntityDrawer\\PixelShader.cso";
}

namespace EntityDrawer {
	PixelShaderData::PixelShaderData() {
		ShadersManager::gInstance->LoadPixelShader(shader, &mShader);
		ASSERT(mShader);
	}

	void PixelShaderData::PreDraw(ID3D11Device1& /*device*/, ID3D11DeviceContext1& context) {
		ASSERT(mShader);
		context.PSSetShader(mShader, nullptr, 0);

		ID3D11ShaderResourceView* const srvs[] = { mTextureSRV };
		context.PSSetShaderResources(0, ARRAYSIZE(srvs), srvs);

		ID3D11SamplerState* const samplerStates[] = { mSampler };
		context.PSSetSamplers(0, ARRAYSIZE(samplerStates), samplerStates);
	}

	void PixelShaderData::PostDraw(ID3D11DeviceContext1& context) {
		context.PSSetShader(nullptr, nullptr, 0);

		ID3D11ShaderResourceView* const srvs[] = { nullptr };
		context.PSSetShaderResources(0, ARRAYSIZE(srvs), srvs);

		ID3D11SamplerState* const samplerStates[] = { nullptr };
		context.PSSetSamplers(0, ARRAYSIZE(samplerStates), samplerStates);
	}
}
#pragma once

#include <DirectXMath.h>

struct ID3D11DepthStencilView;
struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct ID3D11PixelShader;
struct ID3D11RenderTargetView;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class PixelShaderData {
public:
	PixelShaderData();

	void PreDraw(ID3D11Device1& device, ID3D11DeviceContext1& context);
	void PostDraw(ID3D11DeviceContext1& context);

	ID3D11SamplerState* &SamplerState() { return mSampler; }
	ID3D11ShaderResourceView* &TextureSRV() { return mTextureSRV; }

private:
	ID3D11PixelShader* mShader;
	ID3D11SamplerState* mSampler;
	ID3D11ShaderResourceView* mTextureSRV;
};
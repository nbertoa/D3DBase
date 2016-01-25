#pragma once

#include <DirectXMath.h>
#include <vector>

#include <StringDrawer/StringDrawer.h>

struct ID3D11DepthStencilView;
struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct ID3D11RenderTargetView;
struct IDXGISwapChain1;

class DrawManager {
public:
	static DrawManager* gInstance;

	DrawManager(ID3D11Device1& device, ID3D11DeviceContext1& context, const unsigned int screenWidth, const unsigned int screenHeight);
	
	void DrawAll(ID3D11Device1& device, ID3D11DeviceContext1& context, IDXGISwapChain1& swapChain, ID3D11RenderTargetView& backBufferRTV, ID3D11DepthStencilView& depthStencilView, ID3D11ShaderResourceView& depthStencilSRV);

	StringDrawer& FrameRateDrawer() { return mFrameRateDrawer; }

private:
	StringDrawer mFrameRateDrawer;
};
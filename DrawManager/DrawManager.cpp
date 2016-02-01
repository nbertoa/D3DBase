#include "DrawManager.h"

#include <d3d11_1.h>
#include <DirectXColors.h>
#include <vector>

#include <Camera\Camera.h>
#include <Utils/Assert.h>
#include <Utils/RenderStateHelper.h>

using namespace DirectX;

DrawManager* DrawManager::gInstance = nullptr;

DrawManager::DrawManager(ID3D11Device1& device, ID3D11DeviceContext1& context, const unsigned int screenWidth, const unsigned int screenHeight)
	: mFrameRateDrawer(device, context)
{
}

DrawManager::~DrawManager() {
	for (IDrawer* drawer : mDrawers) {
		ASSERT(drawer);
		delete drawer;
	}
}

void DrawManager::DrawAll(ID3D11Device1& device, ID3D11DeviceContext1& context, IDXGISwapChain1& swapChain, ID3D11RenderTargetView& backBufferRTV, ID3D11DepthStencilView& depthStencilView, ID3D11ShaderResourceView& depthStencilSRV) {
	RenderStateHelper::gInstance->SaveAll();

	// Clear render target views
	ID3D11RenderTargetView* backBuffer = &backBufferRTV;
	context.ClearRenderTargetView(&backBufferRTV, reinterpret_cast<const float*>(&Colors::Black));
	context.ClearDepthStencilView(&depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context.OMSetRenderTargets(1, &backBuffer, &depthStencilView);

	const XMMATRIX view = Camera::gInstance->ViewMatrix();
	const XMMATRIX proj = Camera::gInstance->ProjectionMatrix();

	for (IDrawer* drawer : mDrawers) {
		ASSERT(drawer);
		drawer->Draw(device, context, view, proj);
	}

	mFrameRateDrawer.Draw();

	ASSERT_HR(swapChain.Present(0, 0));

	RenderStateHelper::gInstance->RestoreAll();
}
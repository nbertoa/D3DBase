#include "Drawer.h"

#include <ShaderResourcesManager\GlobalResources.h>

#include <Utils/Assert.h>
#include <Utils/Hash.h>

#include "VertexType.h"

using namespace DirectX;

namespace EntityDrawer {
	void Drawer::Draw(ID3D11Device1& device, ID3D11DeviceContext1& context, const XMMATRIX& view, const XMMATRIX& proj) {
		const XMMATRIX world = XMLoadFloat4x4(&mWorld);
		XMStoreFloat4x4(&mVertexShaderData.WorldView(), XMMatrixTranspose(world * view));
		XMStoreFloat4x4(&mGeometryShaderData.Projection(), XMMatrixTranspose(proj));

		context.OMSetBlendState(GlobalResources::gInstance->TransparentBlendState(), nullptr, 0xffffffff);

		mVertexShaderData.PreDraw(device, context);
		mGeometryShaderData.PreDraw(device, context);
		mPixelShaderData.PreDraw(device, context);
		mVertexShaderData.Draw(context);
		mVertexShaderData.PostDraw(context);
		mGeometryShaderData.PostDraw(context);
		mPixelShaderData.PostDraw(context);
	}
}
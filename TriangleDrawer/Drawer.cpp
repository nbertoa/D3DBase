#include "Drawer.h"

#include <ShaderResourcesManager\GlobalResources.h>
#include <Utils/Assert.h>
#include <Utils/Hash.h>

#include "VertexType.h"

using namespace DirectX;

namespace TriangleDrawer {
	void Drawer::Draw(ID3D11Device1& device,
					  ID3D11DeviceContext1& context,
					  const XMMATRIX& view,
					  const XMMATRIX& proj) {
		const XMMATRIX world = XMLoadFloat4x4(&mWorld);
		XMStoreFloat4x4(&mVertexShaderData.WorldViewProjection(), XMMatrixTranspose(world * view * proj));

		context.RSSetState(GlobalResources::gInstance->NoCullRS());

		mVertexShaderData.PreDraw(device, context);
		mPixelShaderData.PreDraw(device, context);
		mVertexShaderData.Draw(context);
		mVertexShaderData.PostDraw(context);
		mPixelShaderData.PostDraw(context);
	}
}
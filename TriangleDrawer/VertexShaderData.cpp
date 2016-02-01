#include "VertexShaderData.h"

#include <d3d11_1.h>
#include <sstream>

#include <ShadersManager/ShadersManager.h>
#include <Utils/Hash.h>

#include "VertexType.h"

using namespace DirectX;

namespace {
	const char* shader = "..\\..\\content\\shaders\\TriangleDrawer\\VertexShader.cso";
}

namespace TriangleDrawer {
	VertexShaderData::VertexShaderData()
		: mInputLayout(nullptr)
		, mShader(nullptr)
		, mVertexBuffer(nullptr)
		, mIndexBuffer(nullptr)
		, mIndicesCount(0)
	{
		InitializeShader();
		InitializeCBuffers();
	}

	void VertexShaderData::InitializeShader() {
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const unsigned int numElems = ARRAYSIZE(inputElementDescriptions);
		ShadersManager::gInstance->LoadVertexShader(shader, inputElementDescriptions, &numElems, &mShader);
		ASSERT(mShader);

		mInputLayout = ShadersManager::gInstance->InputLayout(Utils::Hash(shader));
		ASSERT(mInputLayout);
	}

	void VertexShaderData::InitializeCBuffers() {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.ByteWidth = sizeof(CBufferPerFrameData);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		std::stringstream str;
		str << "TriangleDrawer_VertexShaderData";
		str << rand() << rand();
		mCBuffer.InitializeBuffer(str.str().c_str(), bufferDesc);
	}

	void VertexShaderData::PreDraw(ID3D11Device1& device, ID3D11DeviceContext1& context) {
		ASSERT(mInputLayout);
		ASSERT(mShader);
		ASSERT(mVertexBuffer);
		ASSERT(mIndexBuffer);
		ASSERT(mIndicesCount > 0);

		context.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context.IASetInputLayout(mInputLayout);

		context.VSSetShader(mShader, nullptr, 0);

		const unsigned int stride = sizeof(VertexData);
		const unsigned int offset = 0;
		context.IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		context.IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set constant buffers
		ID3D11Buffer* const cBuffers[] = { mCBuffer.mBuffer };
		mCBuffer.CopyDataToBuffer(device);
		context.VSSetConstantBuffers(0, ARRAYSIZE(cBuffers), cBuffers);
	}

	void VertexShaderData::Draw(ID3D11DeviceContext1& context) {
		ASSERT(mInputLayout);
		ASSERT(mShader);
		ASSERT(mVertexBuffer);
		ASSERT(mIndexBuffer);
		ASSERT(mIndicesCount > 0);
		context.DrawIndexed(mIndicesCount, 0, 0);
	}

	void VertexShaderData::PostDraw(ID3D11DeviceContext1& context) {
		/*context.IASetInputLayout(nullptr);
		context.VSSetShader(nullptr, nullptr, 0);

		ID3D11Buffer* vertexBuffers[] = { nullptr };
		const unsigned int stride[] = { 0 };
		const unsigned int offset[] = { 0 };
		context.IASetVertexBuffers(0, 1, vertexBuffers, stride, offset);

		context.IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);

		ID3D11Buffer* const cBuffers[] = { nullptr };
		context.VSSetConstantBuffers(0, ARRAYSIZE(cBuffers), cBuffers);*/
	}
}
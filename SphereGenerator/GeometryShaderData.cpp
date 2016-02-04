#include "GeometryShaderData.h"

#include <d3d11_1.h>
#include <sstream>

#include <ShadersManager/ShadersManager.h>
#include <Utils/Assert.h>
#include <Utils/Hash.h>

using namespace DirectX;

namespace {
	const char* shader = "..\\..\\content\\shaders\\SphereGenerator\\GeometryShader.cso";
}

namespace SphereGenerator {
	GeometryShaderData::GeometryShaderData() {
		ShadersManager::gInstance->LoadGeometryShader(shader, &mShader);
		ASSERT(mShader);

		InitializeCBuffers();
	}

	void GeometryShaderData::InitializeCBuffers() {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.ByteWidth = sizeof(CBufferPerFrameData);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		std::stringstream str;
		str << "CubeGenerator_GeometryShaderData";
		str << rand() << rand();
		mCBuffer.InitializeBuffer(str.str().c_str(), bufferDesc);
	}

	void GeometryShaderData::PreDraw(ID3D11Device1& device, ID3D11DeviceContext1& context) {
		ASSERT(mShader);

		context.GSSetShader(mShader, nullptr, 0);

		// Set constant buffers
		ID3D11Buffer* const cBuffers[] = { mCBuffer.mBuffer };
		mCBuffer.CopyDataToBuffer(device);
		context.GSSetConstantBuffers(0, ARRAYSIZE(cBuffers), cBuffers);
	}

	void GeometryShaderData::PostDraw(ID3D11DeviceContext1& context) {
		context.GSSetShader(nullptr, nullptr, 0);
		ID3D11Buffer* const cBuffers[] = { nullptr };
		context.GSSetConstantBuffers(0, ARRAYSIZE(cBuffers), cBuffers);
	}
}
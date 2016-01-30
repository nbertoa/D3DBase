#include "VertexType.h"

#include <d3d11_1.h>

#include <DXUtils/DXUtils.h>
#include <ShaderResourcesManager/ShaderResourcesManager.h>
#include <Utils/Assert.h>
#include <Utils/Hash.h>

using namespace DirectX;

VertexData::VertexData() {}

VertexData::VertexData(const XMFLOAT4& posL)
		: mPosL(posL)
{
}

size_t VertexData::CreateVertexBuffer(const std::string& bufferName,
									  const DirectX::XMFLOAT4* const positions, 
									  const unsigned int count) {
	// Check if there is already a buffer for current model
	// and current vertex type.
	ASSERT(positions);
	const size_t bufferId = Utils::Hash(bufferName.c_str());
	if (ShaderResourcesManager::gInstance->Buffer(bufferId)) {
		return bufferId;
	}

	// Create buffer
	const unsigned int bufferSize = static_cast<unsigned int> (count * sizeof(VertexData));
	Utils::CreateInitializedBuffer(bufferName.c_str(), positions, bufferSize, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER);

	return bufferId;
}
#pragma once

#include <DirectXMath.h>
#include <string>

struct VertexData {
	VertexData();
	VertexData(const DirectX::XMFLOAT4& posL);

	static size_t CreateVertexBuffer(const std::string& bufferName,
									 const DirectX::XMFLOAT4* const positions,
									 const unsigned int count);

	DirectX::XMFLOAT4 mPosL;
};

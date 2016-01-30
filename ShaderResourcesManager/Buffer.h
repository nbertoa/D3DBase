#pragma once

#include <d3d11_1.h>

#include <DXUtils/DXUtils.h>
#include <Utils/Assert.h>

#include "ShaderResourcesManager.h"

template<typename T>
class Buffer {
public:
	size_t InitializeBuffer(const char* name, D3D11_BUFFER_DESC& desc) {
		ASSERT(name);
		const size_t id = ShaderResourcesManager::gInstance->AddBuffer(name, desc, nullptr, &mBuffer);
		ASSERT(mBuffer);
		return id;
	}

	void CopyDataToBuffer(ID3D11Device1& device) {
		Utils::CopyData(device, &mData, sizeof(mData), *mBuffer);
	}

	T mData;
	ID3D11Buffer* mBuffer;
};

#include "ShadersManager.h"

#include <d3d11_1.h>
#include <fstream>

#include <Utils/Assert.h>
#include <Utils/Hash.h>

ShadersManager* ShadersManager::gInstance = nullptr;

ShadersManager::ShadersManager(ID3D11Device1& device)
	: mDevice(device)
{
}

ShadersManager::~ShadersManager() {
	for (auto& elem : mInputLayouts) {
		elem.second->Release();
	}
	for (auto& elem : mVertexShaders) {
		elem.second->Release();
	}
	for (auto& elem : mPixelShaders) {
		elem.second->Release();
	}
	for (auto& elem : mGeometryShaders) {
		elem.second->Release();
	}
	for (auto& elem : mComputeShaders) {
		elem.second->Release();
	}
	for (auto& elem : mHullShaders) {
		elem.second->Release();
	}
	for (auto& elem : mDomainShaders) {
		elem.second->Release();
	}
}

size_t ShadersManager::LoadVertexShader(const char* filepath, const D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc, const unsigned int* descNumElems, ID3D11VertexShader* *shader) {
	ASSERT(filepath);
	const bool createInputLayout = inputLayoutDesc != nullptr && descNumElems != nullptr;
	ASSERT(!createInputLayout || (createInputLayout && mInputLayouts.find(*filepath) == mInputLayouts.end()));
	const size_t id = Utils::Hash(filepath);
	VertexShaders::const_iterator findIt = mVertexShaders.find(id);
	if (findIt != mVertexShaders.end()) {
		if (shader) *shader = findIt->second;
		return id;
	}
	std::vector<char> shaderByteCode;
	StoreShaderByteCode(filepath, shaderByteCode);
	if (createInputLayout) {
		ID3D11InputLayout* inputLayout;
		BuildVertexLayout(shaderByteCode, inputLayoutDesc, *descNumElems, inputLayout);
		mInputLayouts[id] = inputLayout;
	}
	ID3D11VertexShader* elem = nullptr;
	ASSERT_HR(mDevice.CreateVertexShader(&shaderByteCode[0], shaderByteCode.size(), nullptr, &elem));
	mVertexShaders[id] = elem;
	if (shader) *shader = elem;
	return id;
}

size_t ShadersManager::LoadPixelShader(const char* filepath, ID3D11PixelShader* *shader) {
	ASSERT(filepath);
	const size_t id = Utils::Hash(filepath);
	PixelShaders::const_iterator findIt = mPixelShaders.find(id);
	if (findIt != mPixelShaders.end()) {
		if (shader) *shader = findIt->second;
		return id;
	}
	std::vector<char> shaderByteCode;
	StoreShaderByteCode(filepath, shaderByteCode);
	ID3D11PixelShader* elem = nullptr;
	ASSERT_HR(mDevice.CreatePixelShader(&shaderByteCode[0], shaderByteCode.size(), nullptr, &elem));
	mPixelShaders[id] = elem;
	if (shader) *shader = elem;
	return id;
}

size_t ShadersManager::LoadHullShader(const char* filepath, ID3D11HullShader* *shader) {
	ASSERT(filepath);
	const size_t id = Utils::Hash(filepath);
	HullShaders::const_iterator findIt = mHullShaders.find(id);
	if (findIt != mHullShaders.end()) {
		if (shader) *shader = findIt->second;
		return id;
	}
	std::vector<char> shaderByteCode;
	StoreShaderByteCode(filepath, shaderByteCode);
	ID3D11HullShader* elem = nullptr;
	ASSERT_HR(mDevice.CreateHullShader(&shaderByteCode[0], shaderByteCode.size(), nullptr, &elem));
	mHullShaders[id] = elem;
	if (shader) *shader = elem;
	return id;
}

size_t ShadersManager::LoadDomainShader(const char* filepath, ID3D11DomainShader* *shader) {
	ASSERT(filepath);
	const size_t id = Utils::Hash(filepath);
	DomainShaders::const_iterator findIt = mDomainShaders.find(id);
	if (findIt != mDomainShaders.end()) {
		if (shader) *shader = findIt->second;
		return id;
	}
	std::vector<char> shaderByteCode;
	StoreShaderByteCode(filepath, shaderByteCode);
	ID3D11DomainShader* elem = nullptr;
	ASSERT_HR(mDevice.CreateDomainShader(&shaderByteCode[0], shaderByteCode.size(), nullptr, &elem));
	mDomainShaders[id] = elem;
	if (shader) *shader = elem;
	return id;
}

size_t ShadersManager::LoadGeometryShader(const char* filepath, ID3D11GeometryShader* *shader) {
	ASSERT(filepath);
	const size_t id = Utils::Hash(filepath);
	GeometryShaders::const_iterator findIt = mGeometryShaders.find(id);
	if (findIt != mGeometryShaders.end()) {
		if (shader) *shader = findIt->second;
		return id;
	}
	std::vector<char> shaderByteCode;
	StoreShaderByteCode(filepath, shaderByteCode);
	ID3D11GeometryShader* elem = nullptr;
	ASSERT_HR(mDevice.CreateGeometryShader(&shaderByteCode[0], shaderByteCode.size(), nullptr, &elem));
	mGeometryShaders[id] = elem;
	if (shader) *shader = elem;
	return id;
}

size_t ShadersManager::LoadComputeShader(const char* filepath, ID3D11ComputeShader* *shader) {
	ASSERT(filepath);
	const size_t id = Utils::Hash(filepath);
	ComputeShaders::const_iterator findIt = mComputeShaders.find(id);
	if (findIt != mComputeShaders.end()) {
		if (shader) *shader = findIt->second;
		return id;
	}
	std::vector<char> shaderByteCode;
	StoreShaderByteCode(filepath, shaderByteCode);
	ID3D11ComputeShader* elem = nullptr;
	ASSERT_HR(mDevice.CreateComputeShader(&shaderByteCode[0], shaderByteCode.size(), nullptr, &elem));
	mComputeShaders[id] = elem;
	return id;
}

ID3D11InputLayout* ShadersManager::InputLayout(const size_t id) const {
	InputLayouts::const_iterator findIt = mInputLayouts.find(id);
	ASSERT(findIt != mInputLayouts.end());
	return findIt->second;
}

void ShadersManager::StoreShaderByteCode(const char* fileName, std::vector<char>& buffer) const {
	ASSERT(fileName);
	std::ifstream file;
	file.open(fileName, std::ios::binary);
	ASSERT(file.is_open());
	file.seekg(0, std::ios_base::end);
	size_t size = static_cast<size_t> (file.tellg());
	file.seekg(0, std::ios_base::beg);
	buffer.resize(size);
	file.read(&buffer[0], size);
	file.close();
}

void ShadersManager::BuildVertexLayout(std::vector<char>& shaderByteCode, const D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc, const unsigned int inputLayoutDescSize, ID3D11InputLayout* &inputLayout) const {
	ASSERT(!shaderByteCode.empty());
	ASSERT_HR(mDevice.CreateInputLayout(inputLayoutDesc, inputLayoutDescSize, &shaderByteCode[0], shaderByteCode.size(), &inputLayout));
}
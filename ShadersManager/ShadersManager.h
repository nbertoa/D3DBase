#pragma once

#include <unordered_map>

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11ComputeShader;
struct ID3D11Device1;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

class ShadersManager {
public:
	static ShadersManager* gInstance;

	ShadersManager(ID3D11Device1& device);
	~ShadersManager();
	const ShadersManager& operator=(const ShadersManager& rhs) = delete;

	// Methods to get/load different kind of shaders.
	// If a shader was already loaded, it is only returned.
	// For vertex shader loading, you can optionally load a input layout too or not.
	// Note: Shader should be previously compiled to .cso

	size_t LoadVertexShader(const char* filepath, const D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = nullptr, const unsigned int* descNumElems = nullptr, ID3D11VertexShader* * shader = nullptr);
	size_t LoadPixelShader(const char* filepath, ID3D11PixelShader* *shader = nullptr);
	size_t LoadHullShader(const char* filepath, ID3D11HullShader* *shader = nullptr);
	size_t LoadDomainShader(const char* filepath, ID3D11DomainShader* *shader = nullptr);
	size_t LoadGeometryShader(const char* filepath, ID3D11GeometryShader* *shader = nullptr);
	size_t LoadComputeShader(const char* filepath, ID3D11ComputeShader* *shader = nullptr);

	ID3D11InputLayout* InputLayout(const size_t id) const;

private:
	void StoreShaderByteCode(const char* fileName, std::vector<char>& buffer) const;
	void BuildVertexLayout(std::vector<char>& shaderByteCode, const D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc, const unsigned int inputLayoutDescSize, ID3D11InputLayout* &inputLayout) const;

	ID3D11Device1& mDevice;

	typedef std::unordered_map<size_t, ID3D11InputLayout*> InputLayouts;
	InputLayouts mInputLayouts;

	typedef std::unordered_map<size_t, ID3D11VertexShader*> VertexShaders;
	VertexShaders mVertexShaders;

	typedef std::unordered_map<size_t, ID3D11PixelShader*> PixelShaders;
	PixelShaders mPixelShaders;

	typedef std::unordered_map<size_t, ID3D11HullShader*> HullShaders;
	HullShaders mHullShaders;

	typedef std::unordered_map<size_t, ID3D11DomainShader*> DomainShaders;
	DomainShaders mDomainShaders;

	typedef std::unordered_map<size_t, ID3D11ComputeShader*> ComputeShaders;
	ComputeShaders mComputeShaders;

	typedef std::unordered_map<size_t, ID3D11GeometryShader*> GeometryShaders;
	GeometryShaders mGeometryShaders;
};
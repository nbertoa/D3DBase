#pragma once

#include <DirectXMath.h>

#include <ShaderResourcesManager/Buffer.h>
#include <Utils/Assert.h>

struct ID3D11Buffer;
struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct ID3D11InputLayout;
struct ID3D11VertexShader;

namespace CubeGenerator {
	class VertexShaderData {
	public:
		VertexShaderData();

		void PreDraw(ID3D11Device1& device, ID3D11DeviceContext1& context);
		void Draw(ID3D11DeviceContext1& context);
		void PostDraw(ID3D11DeviceContext1& context);

		DirectX::XMFLOAT4X4& World() { return mCBuffer.mData.mWorld; }

		ID3D11Buffer* &VertexBuffer() { return mVertexBuffer; }
		void SetVertexCount(const unsigned int count) { mVertexCount = count; }

	private:
		void InitializeShader();
		void InitializeCBuffers();

		ID3D11InputLayout* mInputLayout;
		ID3D11VertexShader* mShader;

		struct CBufferPerFrameData {
			DirectX::XMFLOAT4X4 mWorld;
		};
		Buffer<CBufferPerFrameData> mCBuffer;

		ID3D11Buffer* mVertexBuffer;
		unsigned int mVertexCount;
	};
}

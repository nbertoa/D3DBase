#pragma once

#include <DirectXMath.h>

#include <ShaderResourcesManager/Buffer.h>
#include <Utils/Assert.h>

struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct ID3D11GeometryShader;

namespace SphereGenerator {
	class GeometryShaderData {
	public:
		GeometryShaderData();

		void PreDraw(ID3D11Device1& device, ID3D11DeviceContext1& context);
		void PostDraw(ID3D11DeviceContext1& context);

		DirectX::XMFLOAT4X4& ViewProjection() { return mCBuffer.mData.mViewProjection; }
		void SetQuadHalfSize(const float size) { mCBuffer.mData.mQuadHalfSize = size; }

	private:
		void InitializeCBuffers();
		ID3D11GeometryShader* mShader;

		struct CBufferPerFrameData {
			DirectX::XMFLOAT4X4 mViewProjection;
			float mQuadHalfSize;
		};
		Buffer<CBufferPerFrameData> mCBuffer;
	};
}
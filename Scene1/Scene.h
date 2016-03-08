#pragma once

#include <DirectXMath.h>
#include <thread>
#include <vector>

#include <App/Component.h> 

struct ID3D11Buffer;

namespace EntitiesScene {
	class Scene : public Component {
	public:
		Scene();
		~Scene();
		void Update(ID3D11Device1& device, const float elapsedTime) override;

	private:
		ID3D11Buffer* mVertexBuffer;
				
		DirectX::XMFLOAT3* mPositions1;
		DirectX::XMFLOAT3* mPositions2;
		DirectX::XMFLOAT3* mPositionsBufferPtr;
		DirectX::XMFLOAT3* mVelocities;

		std::thread mSimulationThread;

		float mElapsedTime;
	};
}

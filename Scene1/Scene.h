#pragma once

#include <DirectXMath.h>
#include <vector>

#include <App/Component.h> 

struct ID3D11Buffer;

class Scene : public Component {
public:
	Scene();
	void Update(ID3D11Device1& device, const float elapsedTime) override;

private:
	ID3D11Buffer* mVertexBuffer;
	std::vector<DirectX::XMFLOAT3> mPositions;
	std::vector<DirectX::XMFLOAT3> mVelocity;
};

#pragma once

#include <DirectXMath.h>

struct ID3D11Device1;
struct ID3D11DeviceContext1;

class IDrawer {
public:
	virtual ~IDrawer() {}
	virtual void Draw(ID3D11Device1& device, ID3D11DeviceContext1& context, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) = 0;
};

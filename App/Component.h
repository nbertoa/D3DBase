#pragma once

struct ID3D11Device1;

class Component {
public:
	virtual void Update(ID3D11Device1& device, const float elapsedTime) = 0;
};
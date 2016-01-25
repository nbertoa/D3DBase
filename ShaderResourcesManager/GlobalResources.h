#pragma once

struct ID3D11SamplerState;

class GlobalResources {
public:
	static GlobalResources* gInstance;

	GlobalResources();
	ID3D11SamplerState* MinMagMipPointSampler() { return mMinMagMipPointSS; }
private:
	ID3D11SamplerState* mMinMagMipPointSS;
};
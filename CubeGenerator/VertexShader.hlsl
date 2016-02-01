struct Input {
	float3 PosOS : POSITION;
};

struct Output {
	float4 PosWS : POSITION;
};

cbuffer CBufferPerFrame : register (b0) {
	float4x4 World;
}

Output main(const Input input) {
	Output output = (Output)0;
	output.PosWS = mul(float4(input.PosOS, 1.0f), World);
	return output;
}
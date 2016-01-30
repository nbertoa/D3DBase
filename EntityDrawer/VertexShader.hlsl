struct Input {
	float4 PosOS : POSITION;
};

struct Output {
	float4 PosVS : POSITION;
};

cbuffer CBufferPerFrame : register (b0) {
	float4x4 WorldView;
}

Output main(const Input input) {
	Output output = (Output)0;
	output.PosVS = mul(input.PosOS, WorldView);
	return output;
}
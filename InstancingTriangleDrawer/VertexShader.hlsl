struct Input {
	float3 PosOS : POSITION;
	float3 DirOS : DIRECTION;
	uint InstanceId: SV_InstanceID;
};

struct Output {
	float4 PosH : SV_POSITION;
	float3 Color : COLOR;
};

cbuffer CBufferPerFrame : register (b0) {
	float4x4 WorldViewProjection;
}

Output main(const Input input, const uint vertexId : SV_VertexId) {
	Output output = (Output)0;
	output.PosH = mul(float4(input.PosOS + input.DirOS, 1.0f), WorldViewProjection);
	const float colorComp = (vertexId % 3) * 0.5f;
	output.Color = float3(colorComp, 0.0f, colorComp);
	return output;
}
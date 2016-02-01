#define QUAD_VERTICES (4)

struct GSInput {
	float4 PosVS : POSITION;
};

cbuffer cbPerFrame : register (b0) {
	float4x4 Projection;
	float QuadHalfSize;
};

struct GSOutput {
	float4 PosH : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

[maxvertexcount(QUAD_VERTICES)]
void
main(const in point GSInput input[1], inout TriangleStream<GSOutput> triangleStream) {
	// Compute vertices positions and texture coordinates based on
	// a quad whose center position is mQuadCenterPosV
	GSOutput output;

	output.PosH = mul(input[0].PosVS + float4(-QuadHalfSize, QuadHalfSize, 0.0f, 0.0f), Projection);
	output.TexCoord = float2(0.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosVS + float4(QuadHalfSize, QuadHalfSize, 0.0f, 0.0f), Projection);
	output.TexCoord = float2(1.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosVS + float4(-QuadHalfSize, -QuadHalfSize, 0.0f, 0.0f), Projection);
	output.TexCoord = float2(0.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosVS + float4(QuadHalfSize, -QuadHalfSize, 0.0f, 0.0f), Projection);
	output.TexCoord = float2(1.0f, 0.0f);
	triangleStream.Append(output);

	triangleStream.RestartStrip();
}
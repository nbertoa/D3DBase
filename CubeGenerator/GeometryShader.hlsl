#define CUBE_VERTICES (24)

struct GSInput {
	float4 PosWS : POSITION;
};

cbuffer cbPerFrame : register (b0) {
	float4x4 ViewProjection;
	float QuadHalfSize;
};

struct GSOutput {
	float4 PosH : SV_POSITION;
	float3 Color : COLOR;
};

[maxvertexcount(CUBE_VERTICES)]
void
main(const in point GSInput input[1], inout TriangleStream<GSOutput> triangleStream) {
	// Generate cube triangles
	GSOutput output;

	// Front face 
	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(1.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 1.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, -QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, -QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	triangleStream.RestartStrip();

	// Back face 
	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(1.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 1.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, -QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, -QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	triangleStream.RestartStrip();

	// Left face 
	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(1.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 1.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, -QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, -QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	triangleStream.RestartStrip();

	// Right face 
	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(1.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 1.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, -QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, -QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	triangleStream.RestartStrip();

	// Top face 
	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(1.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 1.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	triangleStream.RestartStrip();

	// Bottom face 
	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, -QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(1.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, -QuadHalfSize, -QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 1.0f, 0.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(-QuadHalfSize, -QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 1.0f);
	triangleStream.Append(output);

	output.PosH = mul(input[0].PosWS + float4(QuadHalfSize, -QuadHalfSize, QuadHalfSize, 0.0f), ViewProjection);
	output.Color = float3(0.0f, 0.0f, 0.0f);
	triangleStream.Append(output);

	triangleStream.RestartStrip();
}
struct PSInput {
	float4 PosH : SV_POSITION;
	float3 Color : COLOR;
};

float4
main(in PSInput input) : SV_TARGET {
	return float4(input.Color, 1.0f);
}
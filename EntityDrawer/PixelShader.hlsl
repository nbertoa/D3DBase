struct PSInput {
	float4 PosH : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

SamplerState Sampler : register(s0);

Texture2D Texture : register(t0);

float4
main(in PSInput input) : SV_TARGET {
	return Texture.Sample(Sampler, input.TexCoord);
}
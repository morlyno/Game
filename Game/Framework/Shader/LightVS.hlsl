cbuffer cBuff
{
	matrix model;
	matrix modelviewtransform;
    float3 campos;
}

struct VSOUT
{
	float3 worldpos : WorldPos;
	float3 n : Normal;
    float3 campos : CamPos;
	float4 pos : SV_POSITION;
};

VSOUT main( float3 pos : Position, float3 n : Normal )
{
	VSOUT vso;
	vso.pos = mul(float4(pos, 1.0f), modelviewtransform);
	vso.worldpos = (float3) mul(float4(pos, 1.0f), model);
    vso.n = normalize(mul(n, (float3x3) model));
    vso.campos = campos;
	return vso;
}
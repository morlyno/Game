cbuffer cbuf
{
    matrix model;
    matrix modelviewtransform;
    float3 campos;
};

struct VSOut
{
    float3 worldpos : WorldPos;
    float3 n : Normal;
    float3 campos : CamPos;
    float2 tex : TexCoord;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tex : TexCoord)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), modelviewtransform);
    vso.worldpos = (float3) mul(float4(pos, 1.0f), model);
    vso.n = normalize(mul(n, (float3x3) model));
    vso.campos = campos;
    vso.tex = tex;
    return vso;
}
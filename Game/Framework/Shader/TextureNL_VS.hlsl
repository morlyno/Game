cbuffer cbuf
{
    matrix model;
    matrix modelviewtransform;
    float3 campos;
};

struct VSOut
{
    float3 worldpos : WorldPos;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 bitangent : Bitangent;
    float3 campos : CamPos;
    float2 tex : TexCoord;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : Position, float3 normal : Normal,float3 tangent : Tangent ,float3 bitangent : Bitangent, float2 tex : TexCoord)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), modelviewtransform);
    vso.worldpos = (float3) mul(float4(pos, 1.0f), model);
    vso.normal = normalize(mul(normal, (float3x3) model));
    vso.tangent = normalize(mul(tangent, (float3x3) model));
    vso.bitangent = normalize(mul(bitangent, (float3x3) model));
    vso.campos = campos;
    vso.tex = tex;
    return vso;
}
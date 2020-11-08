cbuffer LightcBuff
{
    float3 LightPos;
    float falloffmutiplier;
    float ambiant;
};

cbuffer cBuff
{
    float4 matirialColor;
    float specularIntesity;
    float specularPower;
    int normalmap;
};

Texture2D t2d : register(t0);
Texture2D nm : register(t1);

SamplerState smpl;

static const float4 specColor = { 1.0f, 1.0f, 1.0f, 1.0f };

float4 main(float3 WorlPos : WorldPos, float3 normal : Normal, float3 tangent : Tangent, float3 bitanget : Bitangent, float3 campos : CamPos, float2 tex : TexCoord) : SV_TARGET
{
    float3 n = nm.Sample(smpl, tex).xyz;
    n = (n - 0.5f) * 2.0f;
    n = normalize(n);
    
    const float3x3 TanToWord = float3x3(
        tangent,
        bitanget,
        normal
    );
    
    n = mul(n, TanToWord);
    
    float3 ToL = LightPos - WorlPos;
    float att = 1.0f / (length(ToL) * falloffmutiplier);
    float3 dirToL = normalize(ToL);
    float scal = dot(dirToL, n);
    float3 dirToC = normalize(campos - WorlPos);
    
    float3 w = n * dot(dirToL, n);
    float3 r = -(dirToL - w) + w;
    
    float4 sc = (pow(max(0.0f, dot(normalize(r), dirToC)), 1.0f / specularPower) * specColor) * specularIntesity;
    
    float4 srcColor = t2d.Sample(smpl, tex);
    
    
    float4 c = srcColor * max(saturate(scal * att), ambiant);
    return c + sc;
}
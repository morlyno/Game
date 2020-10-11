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
};

static const float4 specColor = { 1.0f, 1.0f, 1.0f, 1.0f };

float4 main(float3 WorlPos : WorldPos, float3 n : Normal, float3 campos : CamPos) : SV_TARGET
{
    float3 ToL = LightPos - WorlPos;
    float att = 1.0f / (length(ToL) * falloffmutiplier);
    float3 dirToL = normalize(ToL);
	float scal = dot(dirToL, n);
    float3 dirToC = normalize(campos - WorlPos);
    
    float3 w = n * dot(dirToL, n);
    float3 r = -(dirToL - w) + w;
    
    float sc = (pow(max(0.0f, dot(r, dirToC)), 1.0f / specularPower) * specColor) * specularIntesity;
    
    float4 c = matirialColor * max(saturate(scal * att), ambiant);
    return c + sc;
}
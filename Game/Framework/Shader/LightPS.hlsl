cbuffer LightcBuff
{
    float3 LightPos;
    float falloffmutiplier;
};

cbuffer cBuff
{
    float4 matirialColor;
};

float4 main(float3 WorlPos : WorldPos, float3 n : Normal) : SV_TARGET
{
    float3 ToL = LightPos - WorlPos;
    float att = 1.0f / (length(ToL) * falloffmutiplier);
    float3 dirToL = normalize(ToL);
	float scal = dot(dirToL, n);
    float4 c =  matirialColor * saturate(scal * att);
    return c;
}
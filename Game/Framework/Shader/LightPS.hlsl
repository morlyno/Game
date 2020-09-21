cbuffer cBuff
{
    float3 LightPos;
    float falloffmutiplier;
};

static const float4 matirialColor = { 0.7f, 0.1f, 0.4f, 1.0f };

float4 main(float3 WorlPos : WorldPos, float3 n : Normal) : SV_TARGET
{
	float3 ToL = WorlPos - LightPos;
    float att = 1.0f / (length(ToL) * falloffmutiplier);
    float3 dirToL = normalize(ToL);
	float scal = 1.0f - dot(dirToL, n);
    return matirialColor * saturate(scal * att);
}
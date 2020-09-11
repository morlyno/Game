static const float3 LightPos = { 0.0f, 0.0f, 0.0f };

static const float4 matirialColor = { 0.7f, 0.1f, 0.4f, 1.0f };

float4 main(float3 WorlPos : WorldPos, float3 n : Normal) : SV_TARGET
{
	float3 PToL = normalize(WorlPos - LightPos);
	float scal = 1.0f - dot(PToL, n);
	float4 c = matirialColor * scal;
	return c;
}